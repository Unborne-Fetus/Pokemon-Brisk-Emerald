# Brisk Emerald — independent two-slot save research (work in progress)

This is **not** an enabled or playable dual-save implementation. The current save
writer must not be changed until the data layout, power-loss behavior, and overflow
handling are proven. This branch contains a non-destructive feasibility tool
and tests; `master` is unchanged.

## Existing layout and why a menu alone is unsafe

- 32 physical 4 KiB sectors = 128 KiB flash.
- Physical sectors 0–13 and 14–27 are **alternating automatic backups** of
  one player, not independent profiles.
- Each normal sector carries 3,968 bytes of main data, 116 bytes of SaveBlock3,
  and a 12-byte footer.
- Sectors 28–31 carry four shared extension chunks with sector IDs 0x100–0x103
  for the final six PC boxes. Each full save overwrites those four chunks.
- Neither original backup bank owns a complete 20-box profile independent of
  the shared extension. Selecting an older bank after the extension has been
  overwritten can yield mixed-generation Pokémon data.

**Do not reinterpret backup bank 2 as another player's save.**

## Requirements

1. Two truly separate profiles, **20 boxes/600 Pokémon per profile**, with
   independent party, Bag, story flags, Pokédex, and save options.
2. Preserve and validate the original old-format save; one-way conversion to
   Slot 1 only, with Slot 2 empty. No backwards conversion is needed.
3. Stay within 128 KiB if this can be made lossless and crash-safe.
4. Reject malformed/unsupported saves before any migration write.
5. Do not silently truncate Pokémon, reduce boxes, or overwrite profiles.

## Verified test file

The private supplied save was analyzed locally and was **not** committed.

- Input size: 131,088 bytes (128 KiB flash + 16 trailing emulator bytes).
- Bank 0 has all 14 checksum-valid sections, save counter 222.
- Bank 1 has all 14 checksum-valid sections, save counter 223, and is newer.
- Sectors 28–31 have the matching extension counter 223 and valid checksums.
- The 20 boxes each contain 30 occupied records (600/600).
- Logical uncompressed state including padded save blocks: **73,048 bytes**.
- Greedy GBA BIOS-compatible LZ77 output: **11,580 bytes**, or **11,612**
  after allowing a proposed 32-byte profile header.
- The optional reversible secure-box XOR transform was tested but did not
  improve this particular synthetic/repetitive save.

This is only one unusually compressible specimen. The unit tests deliberately
include an incompressible full-sized logical state; it **does not fit** the
proposed 16-sector profile. The design cannot promise lossless two-profile
support until valid worst-case data is bounded.

## Tentative sector budgets

- Protected first migration: at most 14 * 3,968 - 32 = **55,520 bytes**
  while the latest complete legacy save still occupies sectors 14–31.
- Proposed final two profiles: 16 * 3,968 - 32 = **63,456 bytes each**
  if each profile occupies 16 sectors.
- Those figures assume existing sector payload sizes; they are **not** a final
  on-flash format. They do not reserve crash-recovery journal space.
- A straightforward 16+16 split leaves no spare backup bank. It is **not
  power-loss safe** simply because it has checksums.

## Work remaining before enabling in-game saves

1. Prove an on-device deterministic compressor/decoder and a lossless
   structural representation of all valid, maximally diverse 20-box profiles,
   including every SaveBlock1/2/3 field. Test many real saves, not just a
   600-Bulbasaur fixture.
2. Solve the overflow case *before* writing. A failed compression must never
   erase the active profile. If a 128 KiB guaranteed-safe representation is
   impossible, report the constraint rather than reducing box count.
3. Design a commit/recovery protocol for interrupted saves that retains both
   profiles. Use fault-injected flash tests and independent per-profile CRCs.
4. Implement a source-format detector, copy/verify/commit one-time migration,
   then two-profile new-game/load selection and a per-profile save writer.
5. Test SaveBlock3, special-sector operations, LinkFullSave, Hall of Fame,
   Dynamax rental guard, corruption recovery, resets during writes, and a
   rebuilt GBA ROM in mGBA.

## Read-only audit command

From the repository root:

```bash
python tools/brisk_multisave_probe.py path/to/pokeemerald.sav
python -m unittest tools/test_brisk_multisave_probe.py
BRISK_TEST_SAVE=path/to/pokeemerald.sav python -m unittest tools/test_brisk_multisave_probe.py
```

The probe reconstructs current SaveBlock2 + SaveBlock1 + all 20 PC boxes +
SaveBlock3 from validated sections, tries GBA BIOS type-0x10 LZ77 compression
with/without a reversible secure-Pokémon XOR preprocessing step, decompresses
the result, and reports proposed capacity checks. It never writes a save, and
does not claim that its prototype serialization can already be loaded by the
game. Python 3.9+ recommended (Python 3.11+ for portable tests using randbytes).
