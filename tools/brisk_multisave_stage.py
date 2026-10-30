#!/usr/bin/env python3
"""In-memory simulation of lossless, legacy-preserving migration staging.

No disk output. No ROM loader reads this proposed format. Research only.
"""
from __future__ import annotations
import struct
import zlib
from pathlib import Path
try:
    from .brisk_multisave_probe import (
        FLASH_BYTES, SECTOR_BYTES, SECTOR_DATA_BYTES, SECTORS_PER_BANK,
        STAGING_CAPACITY, LOGICAL_BYTES, SaveValidationError, get_logical_state,
        gba_lz77_compress, gba_lz77_decompress, reversible_box_xor,
        read_sector, sector_checksum,
    )
except ImportError:  # direct CLI execution instead of a module import
    from brisk_multisave_probe import (
        FLASH_BYTES, SECTOR_BYTES, SECTOR_DATA_BYTES, SECTORS_PER_BANK,
        STAGING_CAPACITY, LOGICAL_BYTES, SaveValidationError, get_logical_state,
        gba_lz77_compress, gba_lz77_decompress, reversible_box_xor,
        read_sector, sector_checksum,
    )

STAGE_MAGIC = 0x42525332  # '2SRB' little-endian; deliberately NOT legacy sector signature
HEADER_MAGIC = b'BR2P'
HEADER_STRUCT = '<4sHBBIIIIII'
HEADER_BYTES = struct.calcsize(HEADER_STRUCT)
assert HEADER_BYTES == 32


def make_profile(state: bytes, counter: int) -> bytes:
    """Test-only on-flash profile payload; rejects over-capacity before writing."""
    if not 0 <= counter < (1 << 32):
        raise SaveValidationError('invalid save counter')
    normalized = reversible_box_xor(state)
    options = ((0, gba_lz77_compress(state)),
               (1, gba_lz77_compress(normalized)))
    transform, compressed = min(options, key=lambda o: len(o[1]))
    if len(compressed) + HEADER_BYTES > STAGING_CAPACITY:
        raise SaveValidationError('profile cannot fit protected 14-sector migration staging')
    header0 = struct.pack(HEADER_STRUCT, HEADER_MAGIC, 1, 0, transform,
                          counter, len(state), len(compressed),
                          zlib.crc32(state), zlib.crc32(compressed), 0)
    header = header0[:-4] + struct.pack('<I', zlib.crc32(header0[:-4]))
    return header + compressed


def unpack_profile(data: bytes) -> tuple[bytes, int]:
    if len(data) < HEADER_BYTES:
        raise SaveValidationError('truncated compact header')
    magic, version, slot, transform, counter, raw_len, packed_len, raw_crc, packed_crc, header_crc = struct.unpack_from(HEADER_STRUCT, data)
    if magic != HEADER_MAGIC or version != 1 or slot != 0 or transform not in (0, 1):
        raise SaveValidationError('unknown compact staging version/slot')
    if zlib.crc32(data[:28]) != header_crc:
        raise SaveValidationError('compact header checksum mismatch')
    if raw_len != LOGICAL_BYTES:
        raise SaveValidationError('incorrect logical state size')
    if packed_len > STAGING_CAPACITY - HEADER_BYTES or len(data) < HEADER_BYTES + packed_len:
        raise SaveValidationError('compact payload length invalid')
    packed = data[HEADER_BYTES:HEADER_BYTES + packed_len]
    if zlib.crc32(packed) != packed_crc:
        raise SaveValidationError('compact payload checksum mismatch')
    raw = gba_lz77_decompress(packed)
    if len(raw) != raw_len:
        raise SaveValidationError('incorrect expanded state length')
    if transform == 1:
        raw = reversible_box_xor(raw)
    if zlib.crc32(raw) != raw_crc:
        raise SaveValidationError('restored state checksum mismatch')
    return raw, counter


def stage_in_memory(original: bytes) -> tuple[bytes, dict]:
    """Replace *older backup bank* in a copy, never touch the valid active bank.

    This is an inspectable proof, not a .sav format supported by current Brisk.
    """
    state, info = get_logical_state(original)
    packed = make_profile(state, info['save_counter'])
    stage_bank = 1 - info['active_bank']
    staged = bytearray(original)
    for i in range(SECTORS_PER_BANK):
        block = bytearray(SECTOR_BYTES)
        piece = packed[i * SECTOR_DATA_BYTES:(i + 1) * SECTOR_DATA_BYTES]
        block[:len(piece)] = piece
        # Use distinct magic so old game never mistakes staging for a legacy save.
        struct.pack_into('<HHII', block, 4084, 0x200 + i,
                         sector_checksum(block[:SECTOR_DATA_BYTES]),
                         STAGE_MAGIC, info['save_counter'])
        start = (stage_bank * SECTORS_PER_BANK + i) * SECTOR_BYTES
        staged[start:start + SECTOR_BYTES] = block
    recovered = read_staged_profile(bytes(staged), stage_bank)
    if recovered != state:
        raise SaveValidationError('staging roundtrip mismatch')
    active_start = info['active_bank'] * SECTORS_PER_BANK * SECTOR_BYTES
    active_end = active_start + SECTORS_PER_BANK * SECTOR_BYTES
    if staged[active_start:active_end] != original[active_start:active_end]:
        raise SaveValidationError('latest legacy bank was overwritten')
    if staged[28 * SECTOR_BYTES:] != original[28 * SECTOR_BYTES:]:
        raise SaveValidationError('legacy extension or emulator trailer was overwritten')
    return bytes(staged), {'stage_bank': stage_bank, 'packed_bytes': len(packed),
                           'source_active_bank': info['active_bank'],
                           'validated': True, 'format_playable': False}


def read_staged_profile(flash: bytes, bank: int) -> bytes:
    if len(flash) < FLASH_BYTES or bank not in (0, 1):
        raise SaveValidationError('bad staged flash or bank')
    payload = bytearray()
    counters = set()
    for i in range(SECTORS_PER_BANK):
        sector = read_sector(flash, bank * SECTORS_PER_BANK + i)
        if (sector['id'] != 0x200 + i or sector['signature'] != STAGE_MAGIC
                or sector_checksum(sector['data']) != sector['checksum']):
            raise SaveValidationError(f'bad stage sector {i}')
        payload.extend(sector['data'])
        counters.add(sector['counter'])
    restored, counter = unpack_profile(payload)
    if counters != {counter}:
        raise SaveValidationError('mixed or incorrect staging counters')
    return restored


if __name__ == '__main__':
    import argparse, json
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('save', type=Path)
    args = parser.parse_args()
    _, stats = stage_in_memory(args.save.read_bytes())
    print(json.dumps(stats, indent=2))
