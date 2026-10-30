#!/bin/sh
# Brisk Emerald two-profile launcher for macOS/Linux.
# Stores each FULL .sav in a separate ROM folder. Does NOT change flash format.
set -eu

rom="${1:-}"
if [ -z "$rom" ]; then
  printf 'Full path to Brisk Emerald .gba: '
  IFS= read -r rom
fi
if [ ! -f "$rom" ]; then
  echo "ERROR: Could not find .gba ROM: $rom" >&2
  exit 1
fi
src_dir=$(cd "$(dirname "$rom")" && pwd -P)
file=$(basename "$rom")
case "$file" in
  *.gba|*.GBA) ;;
  *) echo "ERROR: Choose a .gba ROM" >&2; exit 1 ;;
esac
stem=${file%.*}
root="$src_dir/Brisk Two Save Profiles"
slot1="$root/Slot 1"
slot2="$root/Slot 2"
mkdir -p "$slot1" "$slot2"
cp "$src_dir/$file" "$slot1/$file"
cp "$src_dir/$file" "$slot2/$file"

# Import the source save to Slot 1 exactly once, never moving it.
if [ ! -e "$slot1/$stem.sav" ] && [ ! -e "$slot1/$stem.srm" ]; then
  if [ -f "$src_dir/$stem.sav" ]; then
    cp "$src_dir/$stem.sav" "$slot1/$stem.sav"
    echo 'Imported old save into Slot 1 (source unchanged).'
  elif [ -f "$src_dir/$stem.srm" ]; then
    cp "$src_dir/$stem.srm" "$slot1/$stem.srm"
    echo 'Imported old save into Slot 1 (source unchanged).'
  fi
fi

if [ "${2:-}" = "--prepare-only" ]; then
  echo "Profiles prepared in: $root"
  exit 0
fi

echo 'BRISK EMERALD - TWO SAVE PROFILES'
echo '1. Slot 1 (existing game)'
echo '2. Slot 2 (independent new game)'
echo 'Emulator must save beside the selected ROM, NOT in a shared directory.'
printf 'Choose 1 or 2: '
IFS= read -r selected
case "$selected" in
  1) target="$slot1/$file" ;;
  2) target="$slot2/$file" ;;
  *) echo 'Cancelled.'; exit 0 ;;
esac

if [ -n "${2:-}" ]; then
  exec "$2" "$target"
elif [ "$(uname)" = Darwin ]; then
  open "$target"
else
  xdg-open "$target"
fi
