# Compact two-profile save feasibility

Status: research prototype only, 2026-10-29. No ROM save code changed.
Source baseline: master 0282d634c45a7ae72720dcaf6f9b00d3ed00bac6.

## Requirements

- Two independent playthroughs within a normal 128 KiB flash image.
- Preserve 20 boxes per profile, party, bag, progression, options, Kanto flags,
  fusion storage, and all other currently persisted data.
- One-time migration of supported old Brisk saves into logical Slot 1.
- Never silently drop data, share PC extension sectors, or truncate a profile.
- Full backup copies are unavailable under the proposed capacity constraint.

## Existing layout

`src/save.c` writes two alternating 14-sector snapshots and four shared sectors
for the expanded PC. These are not independent player profiles.
`include/save.h` defines 32 sectors of 4096 bytes. Each normal sector has 3968
data bytes, 116 SaveBlock3 bytes, and 12 footer bytes.

The four PC extension sectors are already unavailable to Hall of Fame,
Trainer Hill special data, and recorded battles in this baseline. Retaining
current features does not imply restoring those already-disabled records.

The inspected BoxPokemon is 80 bytes. Its 48-byte secure region is XORed with
personality and OT ID in `src/pokemon.c`. Reversing this XOR in a temporary
serialization buffer exposes repeated values without changing gameplay data.
Applying the same transform again restores every original byte. The prototype
does not alter substructure order, fields, checksums, padding, or empty records.

600 boxed records alone occupy 48000 bytes. PokemonStorage also contains
metadata and four fusion records. Existing assertions bound SaveBlock1 to
15872 bytes, SaveBlock2 to 3968, and SaveBlock3 to 1624. These are limits, not
measured current struct sizes; the inherited `test/save.c` values are stale
for the 20-box/Kanto build and must not be used as current measurements.

## Candidate format

Reserve 16 sectors per independent profile. With 12 bytes per-sector overhead
and a provisional 64-byte profile header, each profile has 65280 bytes available
for serialized/compressed data. Final metadata or journaling can reduce this.

Store a versioned flat stream of SaveBlock1, SaveBlock2, SaveBlock3, and the
complete PokemonStorage. Use explicit section identifiers/lengths instead of
depending only on compiler offsets. Prefer separate storage codec handling:
reverse PC XOR before compression and restore it after decompression. Assess
party/fusion transforms only after compiler-verified offsets are available.

The profile header must identify format version, logical profile, codec,
section schema, compressed/uncompressed sizes, save generation and whole-stream
CRC. Per-sector generations/CRCs must reject mixed or partial writes. Decode
must be bounded and validate all lengths and destination capacities before
mutating live game state. Commit metadata must be programmed last.

Do not auto-select the profile with the greatest generation: counters belong
to individual profiles. Saving/new game/deletion/link saves/partial saves must
all use the selected profile. Deletion and overwriting must target only that
profile. The title menu can display name, play time, badges and difficulty.

## Experiment

`python3 compact_save_audit.py SAVE.sav` reads but never modifies the file. It
checks signature/ID/counter coherence of each main bank, builds an 18-sector
snapshot in logical order, and measures plain versus XOR-preprocessed BIOS
type-0x10 LZ77. It verifies exact round trips. RTC trailers are accepted but
are excluded from profile snapshots. Main/extension checksums are NOT verified;
this is not a save repair or migration utility.

The full snapshot retains all old sector padding and footer bytes, so it is
deliberately more conservative than a packed live-structure serializer. It
does not prove a final runtime memory budget. Current 20-box preprocessing is
skipped unless all four extension signatures/IDs are recognized. Counter
mismatches are reported because the extension is shared across legacy banks.

`python3 compact_save_audit.py --self-test` checks empty/literal/match/boundary
and random streams, malformed input, XOR reversibility, and synthetic data.

Measured prototype results (synthetic, NOT real-player saves):

| Fixture | Bytes before | LZ77 bytes | XOR then LZ77 bytes |
| --- | ---: | ---: | ---: |
| 600 distinct PC keys with partly zero payload; other snapshot data zero | 73728 | 46183 | 43153 |
| Arbitrary high-entropy snapshot | 73728 | 82921 | Not a capacity guarantee |

The first fixture deliberately varies each record's data but is not a legal
game save. Its non-PC state is zero and cannot stand in for a late-game save.
The second demonstrates why lossless generic compression cannot promise that
every possible byte snapshot fits. It is not proof that reachable legal full
save states exceed capacity either. Both conclusions require separate evidence.

## Release gates

1. Measure ARM compiler sizes, offsets and padding for the actual release
   configuration, not upstream defaults or obsolete tests.
2. Obtain real early/late-game 20-box saves and historical 14-box saves. Validate
   checksums with exact historical schemas before migration experiments.
3. Construct reachable stress data: 600 varied species/forms, distinct OT IDs,
   nicknames, moves, capsules, ribbons/contest stats, fusion records, full bag,
   active records, progression and per-profile Kanto flags.
4. Prove a bounded capacity for supported states through structural packing.
   Measured typical compression alone is not an acceptable promise. A failed
   save due to becoming too large late in a playthrough is not a finished design.
5. Port/measure the compressor and bounded decoder on GBA, including save time,
   watchdog/interrupt behavior, scratch memory and all partial/link-save paths.
6. Design interruption-safe migration separately. A legacy profile occupies one
   14-sector bank plus the four shared extension sectors, leaving only 14
   sectors safe for staging while preserving its only current complete copy.
   Two final 16-sector banks do not by themselves solve transient migration
   space. Prove a staged/journaled write schedule with interruption at every
   erase/write/commit boundary, or require an externally backed converter.
   Never simply initialize Slot 2 before proving the old data survives.
7. Test profile isolation, unsupported versions, malformed/truncated payloads,
   interrupted saves, and the target emulators/flashcarts.

Conclusion: preprocessing plus compact serialization is promising. The tools
establish reversible byte handling and a way to measure real inputs. They do
not yet establish guaranteed fit, migration safety, or playable two-slot support.
