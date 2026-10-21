# Environmental lighting

Brisk's field lighting uses native GBA palette processing. No emulator shader,
extra sprite layer, save format migration, or user action is required.

## Environments

| Profile | Where | Treatment |
| --- | --- | --- |
| Plains | Other outdoor towns/routes | Warm dawn/daylight, amber sunset, blue moonlight |
| Coast | Major seaside towns and ocean routes | Cooler sea air and brighter blue nights |
| Orange Islands | Orange City and Routes 143–149 | Warm dawn, golden tropical daylight, stronger golden sunsets, clear moonlight |
| Forest | Petalburg Woods, Fortree, Routes 119–120 | Green canopy shade with cooler, deeper nights |
| Mountain | Fallarbor, Routes 113–115 | Cool daylight and lavender evening/night colors |
| Volcanic | Chimney, Jagged Pass, Fiery Path, Scorched Slab | Restrained ember warmth |
| Cave | Other underground maps | Constant muted blue ambient light |
| Ice | Shoal Cave, Icefall Cave | Constant pale blue ice light |
| Underwater | All underwater maps | Constant blue-green depth tint |
| Ruins | Sealed Chamber, Desert Ruins, Sky Pillar | Constant aged stone warmth |
| Haunted | Mt. Pyre, Abandoned Ship | Constant restrained violet atmosphere |
| Industrial | New Mauville | Constant cool fluorescent tint |
| Interior | Ordinary buildings and secret bases | Warm indoor light, independent of outdoor time |

Map type takes precedence for underwater areas and ordinary buildings. A cave
on an island receives cave lighting rather than tropical sunlight. Dungeon
sections retain their specialized profiles. Shoal's profile also covers its
non-ice rooms because they share a section; individual-room overrides can be
added later if desired.

All profiles now apply an always-on ambient grade, including midday. Outdoor
clock transitions add variety without disabling the effect during daylight.

## Integration

`src/brisk_lighting.c` selects the profile from the current map type and region
section and returns four palette endpoints. Existing RTC interpolation handles
dawn, morning, evening, and night transitions. The original alternate tileset
palette swapping remains outdoors only. The lighting pass processes map and
object palettes, excludes UI palettes and blend-immune sprite tags, and honors
expansion's alpha-marked emissive colors. Weather is applied after the ambient
lighting; thunder's existing strong-flash exemption remains intact.

The same endpoints are used for map fade-in, weather palette updates, newly
loaded object palettes, and the minute-based time refresh. The unfaded source
palettes remain available to avoid cumulative tinting. No per-frame full-screen
pixel processing is added. UI dark mode remains independent, and battle/menu
palette rendering is unchanged.

Disable `OW_ENVIRONMENTAL_LIGHTING` in `include/config/overworld.h` to restore the
previous outdoor-only day/night behavior. `OW_ENABLE_DNS` remains the master
field lighting switch. Palette values live in the profile table; tint channels
are Q0.8 multipliers, while blend colors use 5-bit RGB and coefficients 0–16.

## Verification

Run `python tools/brisk/check_environment_lighting.py` for host C checks of the
actual selector, valid parameter ranges across map types/sections, indoor
isolation, and sunset/night hue direction. Build the ROM with `make`.

In-game visual QA still needs dawn/day/sunset/night visits to Orange City,
Petalburg Woods, Shoal Cave, Fiery Path, New Mauville, a house, and underwater;
check weather, entering/exiting buildings, menus, and return from battle.
These are palette-based ambient effects, not spatial shadows or new dynamic
point lights. Existing tileset emissive colors are supported without inventing
new lamp objects or marking unrelated tiles as luminous.
