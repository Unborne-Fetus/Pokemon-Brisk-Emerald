# Brisk Emerald: two independent profiles (Windows launcher)

**Status:** This is a working *external save-isolation workflow*, not the
requested in-ROM two-slot menu. It avoids all 128 KiB compression/backup
limitations by using two ordinary, full-size `.sav` files in different
directories. Neither profile sacrifices any of its 20 PC boxes.

## How to use (Windows, no Python or installation needed)

1. Put your updated **Brisk Emerald .gba ROM** and current `.sav` beside
   each other. Their base filenames should match (for example,
   `Brisk Emerald.gba` and `Brisk Emerald.sav`).
2. Download `tools/Brisk-Two-Save-Profiles.bat` from the repository.
3. Drag the `.gba` file onto the BAT file, or run the BAT and paste the full
   ROM path when prompted.
4. Choose **Slot 1** to continue with the migrated old save, or **Slot 2**
   to start a new and independent adventure.
5. In your emulator's settings, **store game saves beside the ROM file**.
   Disable any single global/custom save-file path that causes two ROMs
   to share one `.sav`.
6. Save normally using the in-game save menu. Close the emulator completely
   before switching profiles.

For emulators without a Windows `.gba` file association:

```bat
"Brisk-Two-Save-Profiles.bat" "C:\Games\Brisk Emerald.gba" "C:\Emulators\mGBA.exe"
```

The launcher creates:

```text
Brisk Two Save Profiles/
  Slot 1/
    Brisk Emerald.gba
    Brisk Emerald.sav    <-- one-time COPY from the original save
  Slot 2/
    Brisk Emerald.gba
    Brisk Emerald.sav    <-- created by the emulator when you save
```

It updates the ROM copies to match your selected original ROM on each
launch, but **never overwrites or deletes an existing slot save**.

### Safety and compatibility

- Original saves are left alone. A legacy `.sav` or `.srm` beside the source
  ROM is imported into Slot 1 only when that slot is empty.
- All 20 PC boxes are stored in each **separate physical save file** with
  the existing Brisk engine's sector layout, unmodified.
- Keep backups of your `.sav` files before changing emulator settings.
- If the emulator has a globally configured fixed save path, the two
  profiles may not be isolated. Check the emulator configuration and the
  save file that actually changes after saving.
- An emulator may automatically name its save differently; if your game does
  not load your original save, verify whether it expects `.sav` or `.srm`.
- Do not launch both profiles at once. The two save files are independent,
  but simultaneous use of the same ROM save directory is unsupported.

### Not the end of the in-ROM feature

The separate in-game menu/save codec remains under research in
`docs/brisk/two-slot-save-research.md`. Do not merge the compressed
two-profile flash format into the game unless its capacity and power-loss
requirements are resolved. This launcher is a conservative option that
already allows two full independent adventures without risking a rewrite of
the 128 KiB flash sector code.
