#!/bin/sh
# Standalone shell launcher regression test, requires only POSIX tools.
set -eu
this_dir=$(cd "$(dirname "$0")" && pwd -P)
root=$(mktemp -d)
trap 'rm -rf "$root"' EXIT HUP INT TERM
rom="$root/Brisk Emerald.gba"
src="$root/Brisk Emerald.sav"
printf 'mock-rom-first' > "$rom"
printf 'existing-brisk-save-and-trailer' > "$src"
sh "$this_dir/brisk-two-save-profiles.sh" "$rom" --prepare-only
slot_root="$root/Brisk Two Save Profiles"
cmp "$src" "$slot_root/Slot 1/Brisk Emerald.sav"
[ ! -e "$slot_root/Slot 2/Brisk Emerald.sav" ]
printf 'slot-one-edited' > "$slot_root/Slot 1/Brisk Emerald.sav"
printf 'slot-two-edited' > "$slot_root/Slot 2/Brisk Emerald.sav"
printf 'mock-rom-updated' > "$rom"
sh "$this_dir/brisk-two-save-profiles.sh" "$rom" --prepare-only
printf 'slot-one-edited' | cmp - "$slot_root/Slot 1/Brisk Emerald.sav"
printf 'slot-two-edited' | cmp - "$slot_root/Slot 2/Brisk Emerald.sav"
printf 'existing-brisk-save-and-trailer' | cmp - "$src"
cmp "$rom" "$slot_root/Slot 1/Brisk Emerald.gba"
cmp "$rom" "$slot_root/Slot 2/Brisk Emerald.gba"
echo "PASS: legacy import, two independent saves, ROM updates, source protection"
