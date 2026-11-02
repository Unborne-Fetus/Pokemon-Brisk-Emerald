# Save ownership: standalone ROM versus Brisk Dex

The **standalone Brisk Emerald GBA ROM now shows the standard Continue / New
Game menu** and writes one normal game save. Emerald's two rotating flash
banks remain an internal corruption-recovery mechanism, **not** two
selectable player profiles. The existing twenty PC boxes are not removed.

New standalone saves use Emerald's conventional sector writer with Brisk's
20-box storage extension. The earlier in-game `SLOT 1 / SLOT 2` selector
is removed.

## Older two-profile ROM saves

The old BKS2 compressed Slot 1 remains readable/writable automatically for
compatibility; no option to choose a second profile is shown in the ROM.
This is a legacy compatibility path, **not** an advertised two-save mode.
An orphaned compact Slot 2 is protected against destructive normal writes.
Do not delete or overwrite the old `.sav` until Brisk Dex has migrated both
profiles successfully.

## Brisk Dex

Brisk Dex's desktop integrated game now stores two independent external
PENSAVE snapshots in the application user-data directory. The selected
snapshot is mounted as the native game's one ordinary save when it starts.
Switching requires the game to be stopped and unsaved changes must be saved
from within the game's normal menu first.

The migration detects the old two compressed BKS2 profiles, validates them,
converts each to a conventional native flash image with the complete 20-box
extension, and retains an untouched `pre-migration-compact.sav` recovery
file. Slot 2 is never silently discarded or converted into Slot 1.

**Do not assume copying a raw compact BKS2 `.sav` directly to the newer
standalone ROM preserves both profiles.** Use the Brisk Dex importer first.
