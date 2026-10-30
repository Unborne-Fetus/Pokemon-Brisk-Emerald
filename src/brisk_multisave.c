// Brisk Emerald: in-game dual profile snapshots.
// Snapshots are copy-on-write. Chunk 0 is committed LAST, after all other
// chunks have passed flash verification. A failed save leaves both prior
// snapshots (or the original 20-box legacy file) unmodified.
// On overflow, returns SAVE_STATUS_ERROR rather than discarding Pokemon.
#include "global.h"
#include "agb_flash.h"
#include "brisk_multisave.h"
#include "load_save.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "string.h"

#define BRISK_SECTOR_BYTES 4096
#define BRISK_CHUNK_BYTES SECTOR_DATA_SIZE
#define BRISK_SIGNATURE 0x32534B42 // 'BKS2' in little endian
#define BRISK_META_MAGIC 0x32504642 // 'BFP2'
#define BRISK_SECTOR_ID_BASE 0x200
#define BRISK_SECTOR_COUNT 32
#define BRISK_MAX_CHUNKS 32
#define BRISK_HASH_SIZE 1024
#define BRISK_WINDOW 4096
#define BRISK_MATCH_LIMIT 32

struct BriskChunkHeader
{
    u32 magic;
    u32 packedSize;
    u32 rawSize;
    u32 hash;
    u16 count;
    u8 slot;
    u8 reserved;
};

struct BriskChunkInfo
{
    u8 valid;
    u8 slot;
    u8 index;
    u8 count;
    u32 generation;
    u32 packedSize;
    u32 rawSize;
    u32 hash;
};

struct BriskProfile
{
    u8 valid;
    u8 count;
    u8 physical[BRISK_MAX_CHUNKS];
    u8 reserved;
    u32 generation;
    u32 packedSize;
    u32 hash;
};

static EWRAM_DATA struct BriskChunkInfo sChunkInfo[BRISK_SECTOR_COUNT] = {0};
static EWRAM_DATA struct BriskProfile sProfiles[2] = {0};
static EWRAM_DATA u32 sHashHeads[BRISK_HASH_SIZE] = {0};
static EWRAM_DATA u32 sHashPrev[BRISK_WINDOW] = {0};
static EWRAM_DATA u8 sFirstChunk[BRISK_CHUNK_BYTES] = {0};
static EWRAM_DATA u8 sRing[BRISK_WINDOW] = {0};
static EWRAM_DATA u8 sAllocated[BRISK_MAX_CHUNKS] = {0};
static EWRAM_DATA u8 sActiveSlot = 0;
static EWRAM_DATA bool8 sLegacyAvailable = FALSE;
static EWRAM_DATA u8 sLegacyBank = 0;
static EWRAM_DATA bool8 sReady = FALSE;

static u32 sPackedSize;
static u32 sPackedHash;
static u32 sExpectedSize;
static u32 sExpectedHash;
static u32 sGeneration;
static u8 sChunkCount;
static u8 sWritingSlot;
static bool8 sWriting;
static bool8 sWriteFailed;
static u8 sCachePhysical = 0xFF;

static u32 RawSize(void)
{
    return sizeof(*gSaveBlock2Ptr) + sizeof(*gSaveBlock1Ptr)
         + sizeof(*gPokemonStoragePtr) + sizeof(*gSaveBlock3Ptr);
}

static u8 *RawPointer(u32 position)
{
    u32 size = sizeof(*gSaveBlock2Ptr);
    if (position < size)
        return (u8 *)gSaveBlock2Ptr + position;
    position -= size;
    size = sizeof(*gSaveBlock1Ptr);
    if (position < size)
        return (u8 *)gSaveBlock1Ptr + position;
    position -= size;
    size = sizeof(*gPokemonStoragePtr);
    if (position < size)
        return (u8 *)gPokemonStoragePtr + position;
    return (u8 *)gSaveBlock3Ptr + position - size;
}

// Brisk already stores encrypted BoxPokemon secure data. XOR normalization
// allows repeated species to compress well without changing the on-disk Mon.
static u8 NormalizedByte(u32 position)
{
    u8 byte = *RawPointer(position);
    u32 offset = sizeof(*gSaveBlock2Ptr) + sizeof(*gSaveBlock1Ptr);
    u32 first = offsetof(struct PokemonStorage, boxes);
    u32 count = TOTAL_BOXES_COUNT * IN_BOX_COUNT;
    if (position >= offset + first
     && position < offset + first + count * sizeof(struct BoxPokemon))
    {
        u32 boxRelative = position - offset - first;
        u32 which = boxRelative / sizeof(struct BoxPokemon);
        u32 within = boxRelative % sizeof(struct BoxPokemon);
        if (within >= offsetof(struct BoxPokemon, secure))
        {
            const struct BoxPokemon *mon = &gPokemonStoragePtr->boxes[which / IN_BOX_COUNT][which % IN_BOX_COUNT];
            u32 key = mon->personality ^ mon->otId;
            byte ^= (key >> (8 * ((within - offsetof(struct BoxPokemon, secure)) & 3))) & 0xFF;
        }
    }
    return byte;
}

static void RestoreSecurePokemon(void)
{
    u32 b, m, j;
    for (b = 0; b < TOTAL_BOXES_COUNT; ++b)
        for (m = 0; m < IN_BOX_COUNT; ++m)
        {
            struct BoxPokemon *mon = &gPokemonStoragePtr->boxes[b][m];
            u32 key = mon->personality ^ mon->otId;
            for (j = 0; j < ARRAY_COUNT(mon->secure.raw); ++j)
                mon->secure.raw[j] ^= key;
        }
}

static u16 BriskChecksum(const u8 *data)
{
    u32 sum = 0;
    u32 i;
    for (i = 0; i < BRISK_CHUNK_BYTES; i += 4)
        sum += (u32)data[i] | ((u32)data[i+1] << 8)
             | ((u32)data[i+2] << 16) | ((u32)data[i+3] << 24);
    return (sum >> 16) + sum;
}

static u32 HashByte(u32 hash, u8 byte)
{
    return (hash ^ byte) * 16777619;
}

static void ReadPhysical(u8 physical)
{
    ReadFlash(physical, 0, (u8 *)&gSaveDataBuffer, BRISK_SECTOR_BYTES);
    sCachePhysical = physical;
}

static u8 PackedByte(const struct BriskProfile *profile, u32 at)
{
    u8 physical = profile->physical[at / BRISK_CHUNK_BYTES];
    if (sCachePhysical != physical)
        ReadPhysical(physical);
    return gSaveDataBuffer.data[at % BRISK_CHUNK_BYTES];
}

static bool8 IsNewer(u32 left, u32 right)
{
    return (s32)(left - right) > 0;
}

static bool8 ValidateCandidate(u8 physical, struct BriskProfile *out)
{
    struct BriskChunkInfo *first = &sChunkInfo[physical];
    u8 chunk;
    u32 i;
    u32 hash = 2166136261u;
    if (!first->valid || first->index != 0 || first->count == 0
     || first->count > BRISK_MAX_CHUNKS || first->rawSize != RawSize()
     || first->packedSize < 4
     || first->packedSize > first->count * BRISK_CHUNK_BYTES
     || first->packedSize <= (first->count - 1) * BRISK_CHUNK_BYTES)
        return FALSE;

    memset(out, 0, sizeof(*out));
    out->count = first->count;
    out->generation = first->generation;
    out->packedSize = first->packedSize;
    out->hash = first->hash;

    for (chunk = 0; chunk < first->count; ++chunk)
    {
        u8 found = FALSE;
        for (i = 0; i < BRISK_SECTOR_COUNT; ++i)
        {
            const struct BriskChunkInfo *candidate = &sChunkInfo[i];
            if (candidate->valid && candidate->slot == first->slot
             && candidate->index == chunk && candidate->count == first->count
             && candidate->generation == first->generation
             && candidate->packedSize == first->packedSize
             && candidate->rawSize == first->rawSize
             && candidate->hash == first->hash)
            {
                out->physical[chunk] = i;
                found = TRUE;
                break;
            }
        }
        if (!found)
            return FALSE;
    }
    sCachePhysical = 0xFF;
    for (i = 0; i < out->packedSize; ++i)
        hash = HashByte(hash, PackedByte(out, i));
    if (hash != out->hash)
        return FALSE;
    out->valid = TRUE;
    return TRUE;
}

void BriskMultiSave_Scan(void)
{
    u32 i;
    struct BriskProfile candidate;
    if (gFlashMemoryPresent != TRUE)
        return;
    memset(sProfiles, 0, sizeof(sProfiles));
    memset(sChunkInfo, 0, sizeof(sChunkInfo));
    sCachePhysical = 0xFF;
    for (i = 0; i < BRISK_SECTOR_COUNT; ++i)
    {
        struct BriskChunkHeader meta;
        u16 id;
        ReadPhysical(i);
        id = gSaveDataBuffer.id;
        if (gSaveDataBuffer.signature != BRISK_SIGNATURE
         || id < BRISK_SECTOR_ID_BASE
         || id >= BRISK_SECTOR_ID_BASE + 2 * BRISK_MAX_CHUNKS
         || gSaveDataBuffer.checksum != BriskChecksum(gSaveDataBuffer.data))
            continue;
        memcpy(&meta, gSaveDataBuffer.saveBlock3Chunk, sizeof(meta));
        if (meta.magic != BRISK_META_MAGIC || meta.slot > 1
         || meta.slot != (id - BRISK_SECTOR_ID_BASE) / BRISK_MAX_CHUNKS
         || meta.count == 0 || meta.count > BRISK_MAX_CHUNKS
         || (id - BRISK_SECTOR_ID_BASE) % BRISK_MAX_CHUNKS >= meta.count)
            continue;
        sChunkInfo[i].valid = TRUE;
        sChunkInfo[i].slot = meta.slot;
        sChunkInfo[i].index = (id - BRISK_SECTOR_ID_BASE) % BRISK_MAX_CHUNKS;
        sChunkInfo[i].count = meta.count;
        sChunkInfo[i].generation = gSaveDataBuffer.counter;
        sChunkInfo[i].packedSize = meta.packedSize;
        sChunkInfo[i].rawSize = meta.rawSize;
        sChunkInfo[i].hash = meta.hash;
    }
    for (i = 0; i < BRISK_SECTOR_COUNT; ++i)
    {
        if (!sChunkInfo[i].valid || sChunkInfo[i].index != 0)
            continue;
        if (ValidateCandidate(i, &candidate))
        {
            u8 slot = sChunkInfo[i].slot;
            if (!sProfiles[slot].valid || IsNewer(candidate.generation, sProfiles[slot].generation))
                sProfiles[slot] = candidate;
        }
    }
    if (sProfiles[0].valid)
        sLegacyAvailable = FALSE;
    sReady = TRUE;
    sCachePhysical = 0xFF;
}

void BriskMultiSave_NoticeLegacy(u8 status)
{
    if (sProfiles[0].valid)
        return;
    if (status == SAVE_STATUS_OK || status == SAVE_STATUS_ERROR)
    {
        sLegacyAvailable = TRUE;
        sLegacyBank = gSaveCounter % 2;
    }
}

bool8 BriskMultiSave_HasCompactCurrent(void)
{
    return sProfiles[sActiveSlot].valid;
}

bool8 BriskMultiSave_IsActive(void)
{
    return sReady;
}

bool8 BriskMultiSave_SlotOccupied(u8 slot)
{
    if (slot >= 2)
        return FALSE;
    return sProfiles[slot].valid || (slot == 0 && sLegacyAvailable);
}

bool8 BriskMultiSave_OtherSlotOccupied(void)
{
    return BriskMultiSave_SlotOccupied(sActiveSlot ^ 1);
}

static u8 GetPackedByte(const struct BriskProfile *profile, u32 *at, bool8 *ok)
{
    if (*at >= profile->packedSize)
    {
        *ok = FALSE;
        return 0;
    }
    return PackedByte(profile, (*at)++);
}

static void StoreRawByte(u32 at, u8 value)
{
    *RawPointer(at) = value;
}

u8 BriskMultiSave_LoadCurrent(void)
{
    const struct BriskProfile *profile = &sProfiles[sActiveSlot];
    u32 readAt = 0, outputAt = 0;
    u32 rawSize = RawSize();
    u32 i;
    bool8 ok = TRUE;
    if (!profile->valid)
        return SAVE_STATUS_EMPTY;
    sCachePhysical = 0xFF;
    if (GetPackedByte(profile, &readAt, &ok) != 0x10)
        return SAVE_STATUS_CORRUPT;
    {
        u32 advertised = 0;
        for (i = 0; i < 3; ++i)
            advertised |= (u32)GetPackedByte(profile, &readAt, &ok) << (i * 8);
        if (!ok || advertised != rawSize)
            return SAVE_STATUS_CORRUPT;
    }
    while (outputAt < rawSize && ok)
    {
        u8 flags = GetPackedByte(profile, &readAt, &ok);
        u8 bit;
        for (bit = 0; bit < 8 && outputAt < rawSize && ok; ++bit)
        {
            if (flags & (0x80 >> bit))
            {
                u8 hi = GetPackedByte(profile, &readAt, &ok);
                u8 lo = GetPackedByte(profile, &readAt, &ok);
                u32 length = (hi >> 4) + 3;
                u32 distance = (((u32)hi & 15) << 8 | lo) + 1;
                if (!ok || distance > outputAt || outputAt + length > rawSize)
                    return SAVE_STATUS_CORRUPT;
                while (length--)
                {
                    u8 value = sRing[(outputAt - distance) % BRISK_WINDOW];
                    sRing[outputAt % BRISK_WINDOW] = value;
                    StoreRawByte(outputAt++, value);
                }
            }
            else
            {
                u8 value = GetPackedByte(profile, &readAt, &ok);
                if (!ok)
                    break;
                sRing[outputAt % BRISK_WINDOW] = value;
                StoreRawByte(outputAt++, value);
            }
        }
    }
    if (!ok || outputAt != rawSize)
        return SAVE_STATUS_CORRUPT;
    RestoreSecurePokemon();
    return SAVE_STATUS_OK;
}

u8 BriskMultiSave_Select(u8 slot)
{
    u8 status;
    if (slot >= 2)
        return SAVE_STATUS_ERROR;
    sActiveSlot = slot;
    if (sProfiles[slot].valid)
    {
        status = BriskMultiSave_LoadCurrent();
        if (status == SAVE_STATUS_OK)
            CopyPartyAndObjectsFromSave();
    }
    else if (slot == 0 && sLegacyAvailable)
    {
        // The loader takes the original legacy path for Slot 1.
        status = LoadGameSave(SAVE_NORMAL);
        return status;
    }
    else
        status = SAVE_STATUS_EMPTY;
    gSaveFileStatus = status;
    return status;
}

static u32 HashAt(u32 at, u32 total)
{
    if (at + 2 >= total)
        return 0;
    return ((u32)NormalizedByte(at) * 13
          + (u32)NormalizedByte(at+1) * 17
          + (u32)NormalizedByte(at+2) * 31) & (BRISK_HASH_SIZE - 1);
}

static void IndexAt(u32 at, u32 total)
{
    u32 bucket;
    if (at + 2 >= total)
        return;
    bucket = HashAt(at, total);
    sHashPrev[at % BRISK_WINDOW] = sHashHeads[bucket];
    sHashHeads[bucket] = at + 1;
}

static u8 MatchLength(u32 pos, u32 previous, u32 total)
{
    u8 length = 0;
    u8 max = min(18, total - pos);
    while (length < max
     && NormalizedByte(previous + length) == NormalizedByte(pos + length))
        ++length;
    return length;
}

static void FindMatch(u32 at, u32 total, u8 *length, u16 *distance)
{
    u32 candidate;
    u32 hash;
    u32 attempts = 0;
    *length = 0;
    *distance = 0;
    if (at + 2 >= total)
        return;
    hash = HashAt(at, total);
    candidate = sHashHeads[hash];
    while (candidate && attempts++ < BRISK_MATCH_LIMIT)
    {
        u32 previous = candidate - 1;
        u8 matched;
        u32 next;
        if (previous >= at || at - previous > BRISK_WINDOW)
            break;
        matched = MatchLength(at, previous, total);
        if (matched > *length)
        {
            *length = matched;
            *distance = at - previous;
            if (matched == 18)
                break;
        }
        next = sHashPrev[previous % BRISK_WINDOW];
        if (next >= candidate)
            break;
        candidate = next;
    }
}

static void InitOutput(bool8 writing)
{
    sWriting = writing;
    sWriteFailed = FALSE;
    sPackedSize = 0;
    sPackedHash = 2166136261u;
    if (writing)
    {
        memset(sFirstChunk, 0, sizeof(sFirstChunk));
        memset(&gSaveDataBuffer, 0, sizeof(gSaveDataBuffer));
    }
}

static bool8 WriteChunk(u8 index, const u8 *data)
{
    struct BriskChunkHeader header;
    if (index >= sChunkCount)
        return FALSE;
    memset(&gSaveDataBuffer, 0, sizeof(gSaveDataBuffer));
    memcpy(gSaveDataBuffer.data, data, BRISK_CHUNK_BYTES);
    memset(&header, 0, sizeof(header));
    header.magic = BRISK_META_MAGIC;
    header.packedSize = sExpectedSize;
    header.rawSize = RawSize();
    header.hash = sExpectedHash;
    header.count = sChunkCount;
    header.slot = sWritingSlot;
    memcpy(gSaveDataBuffer.saveBlock3Chunk, &header, sizeof(header));
    gSaveDataBuffer.id = BRISK_SECTOR_ID_BASE + sWritingSlot * BRISK_MAX_CHUNKS + index;
    gSaveDataBuffer.checksum = BriskChecksum(gSaveDataBuffer.data);
    gSaveDataBuffer.signature = BRISK_SIGNATURE;
    gSaveDataBuffer.counter = sGeneration;
    return ProgramFlashSectorAndVerify(sAllocated[index], gSaveDataBuffer.data) == 0;
}

static void EmitByte(u8 value)
{
    u32 position = sPackedSize++;
    sPackedHash = HashByte(sPackedHash, value);
    if (!sWriting || sWriteFailed)
        return;
    if (position < BRISK_CHUNK_BYTES)
        sFirstChunk[position] = value;
    else
    {
        // The output buffer is the *data* portion of a full sector.
        // A completed sector is written immediately; sector zero waits.
        u32 within = position % BRISK_CHUNK_BYTES;
        static EWRAM_DATA u8 sOutputChunk[BRISK_CHUNK_BYTES] = {0};
        if (within == 0)
            memset(sOutputChunk, 0, sizeof(sOutputChunk));
        sOutputChunk[within] = value;
        if (within == BRISK_CHUNK_BYTES - 1)
            if (!WriteChunk(position / BRISK_CHUNK_BYTES, sOutputChunk))
                sWriteFailed = TRUE;
    }
}

// Each token group is assembled locally so no previously written flag byte
// ever needs to be patched across a sector boundary.
static void Compress(void)
{
    u8 group[24];
    u32 total = RawSize();
    u32 at = 0, i;
    memset(sHashHeads, 0, sizeof(sHashHeads));
    memset(sHashPrev, 0, sizeof(sHashPrev));
    EmitByte(0x10);
    EmitByte(total & 0xFF);
    EmitByte((total >> 8) & 0xFF);
    EmitByte((total >> 16) & 0xFF);
    while (at < total)
    {
        u8 flags = 0;
        u8 groupLen = 0;
        u8 token;
        for (token = 0; token < 8 && at < total; ++token)
        {
            u8 length;
            u16 distance;
            u32 step;
            FindMatch(at, total, &length, &distance);
            if (length >= 3)
            {
                u16 encoded = ((length - 3) << 12) | (distance - 1);
                flags |= 0x80 >> token;
                group[groupLen++] = encoded >> 8;
                group[groupLen++] = encoded & 0xFF;
                step = length;
            }
            else
            {
                group[groupLen++] = NormalizedByte(at);
                step = 1;
            }
            for (i = 0; i < step; ++i)
                IndexAt(at + i, total);
            at += step;
        }
        EmitByte(flags);
        for (i = 0; i < groupLen; ++i)
            EmitByte(group[i]);
        if (sWriteFailed)
            return;
    }
    while (sPackedSize % 4)
        EmitByte(0);
}

static bool8 ProtectedPhysical(u8 physical)
{
    u8 slot, chunk;
    for (slot = 0; slot < 2; ++slot)
        if (sProfiles[slot].valid)
            for (chunk = 0; chunk < sProfiles[slot].count; ++chunk)
                if (sProfiles[slot].physical[chunk] == physical)
                    return TRUE;
    if (sLegacyAvailable && !sProfiles[0].valid)
    {
        if (physical >= sLegacyBank * 14 && physical < sLegacyBank * 14 + 14)
            return TRUE;
        if (physical >= 28)
            return TRUE;
    }
    return FALSE;
}

static bool8 AllocateSectors(u8 count)
{
    u8 physical;
    u8 found = 0;
    for (physical = 0; physical < BRISK_SECTOR_COUNT; ++physical)
        if (!ProtectedPhysical(physical))
        {
            sAllocated[found++] = physical;
            if (found == count)
                return TRUE;
        }
    return FALSE;
}

u8 BriskMultiSave_Save(void)
{
    u32 savedSize, savedHash;
    u8 last;
    if (gFlashMemoryPresent != TRUE)
        return SAVE_STATUS_ERROR;
    CopyPartyAndObjectsToSave();
    InitOutput(FALSE);
    Compress();
    savedSize = sPackedSize;
    savedHash = sPackedHash;
    if (savedSize < 4 || savedSize > BRISK_SECTOR_COUNT * BRISK_CHUNK_BYTES)
        return SAVE_STATUS_ERROR;
    sChunkCount = (savedSize + BRISK_CHUNK_BYTES - 1) / BRISK_CHUNK_BYTES;
    if (!AllocateSectors(sChunkCount))
        return SAVE_STATUS_ERROR;
    sWritingSlot = sActiveSlot;
    sExpectedSize = savedSize;
    sExpectedHash = savedHash;
    sGeneration = gSaveCounter + 1;
    if (sProfiles[0].valid && (sGeneration == 0 || IsNewer(sProfiles[0].generation, sGeneration)))
        sGeneration = sProfiles[0].generation + 1;
    if (sProfiles[1].valid && (sGeneration == 0 || IsNewer(sProfiles[1].generation, sGeneration)))
        sGeneration = sProfiles[1].generation + 1;
    InitOutput(TRUE);
    Compress();
    if (sWriteFailed || sPackedSize != savedSize || sPackedHash != savedHash)
        return SAVE_STATUS_ERROR;
    // Write the final partial chunk if needed (index >=1).
    last = (savedSize - 1) / BRISK_CHUNK_BYTES;
    if (last > 0 && savedSize % BRISK_CHUNK_BYTES)
    {
        // EmitByte keeps the incomplete output chunk in the shared scratch.
        // It must be copied out before writing its footer.
        extern u8 BriskMultiSave_GetPendingOutputByte(u16);
        u8 scratch[BRISK_CHUNK_BYTES];
        u32 i;
        for (i = 0; i < BRISK_CHUNK_BYTES; ++i)
            scratch[i] = BriskMultiSave_GetPendingOutputByte(i);
        if (!WriteChunk(last, scratch))
            return SAVE_STATUS_ERROR;
    }
    // COMMIT: all non-zero chunks are verified. Only now write index zero.
    if (!WriteChunk(0, sFirstChunk))
        return SAVE_STATUS_ERROR;
    BriskMultiSave_Scan();
    if (!sProfiles[sActiveSlot].valid || sProfiles[sActiveSlot].generation != sGeneration)
        return SAVE_STATUS_ERROR;
    gSaveCounter = sGeneration;
    return SAVE_STATUS_OK;
}

// Shared last-chunk scratch, defined below; compressor writes into this buffer.
