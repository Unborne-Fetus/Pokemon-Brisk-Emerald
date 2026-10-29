# Orange Islands to Kanto

From Orange City, travel east through Routes 145 and 146. Use Surf on Route
146's east coast and continue east across **Orange Strait**. A short water
transition loads Kanto's graphics. On the Kanto side, travel southwest to
**Cinnabar Island**, or east toward the existing Route 20 and Seafoam Islands.

To return, Surf east from Cinnabar into Orange Strait, then north to the
strait's northern edge. The crossing returns you to the Orange Islands side;
Surf west to Route 146.

This adds **exploration access only**. Kanto's existing terrain, scripts, trainers, encounters,
items, gyms, story flags, and progression have not been reworked. The Emerald
map builder previously excluded Kanto entirely. It now includes those layouts
and connections, while building Kanto with inactive story triggers, trainer
battles, gifts, hidden pickups, and services. Interacting with its residents
explains that the campaign is not ready. This prevents template events with
placeholder flags and overlapping Hoenn variables from modifying a Hoenn save.
Original Kanto JSON, scripts and terrain are retained for a future campaign port.
There is no claim of a playable second story yet. The existing Cinnabar–Route 20 connection now
passes through the new sea junction. Both endpoints retain their original
offset and connect back through that junction. No Kanto maps or layout IDs
were reordered. No badges, story variables, or story flags are granted or reset.

Both halves use their neighboring maps' primary/secondary tilesets and layout
formats. The regional crossing uses an ordinary field warp in open water,
which reloads both tilesets and restores Surf from the destination's water
behavior. Destinations are clear of the reverse crossing triggers.

Route 146's new approach opens only existing ocean tiles in the rectangle
x=20..62, y=24..43. Existing land, scenery, objects, pickups, and door warps
are preserved. Existing saves on Route 146 also refresh those water tiles
when their cached map view is restored.

## Region Map handoff

The west half of Orange Strait retains its own name on the Hoenn map. The
Orange Islands do not yet have a dedicated drawn Town Map, so their player
marker uses the eastern edge of Hoenn instead of indexing unset section
coordinates. When the player crosses to the east half, the map now selects
**Kanto**, anchored to the adjacent Route 20 map square; the existing
FRLG Kanto Town Map artwork, cities and route labels are used automatically.
The Kanto-side sea junction is specifically exempted from the converter's
Kanto-story safety suppression so its return Surf trigger keeps working.

In the Emerald build, Kanto city Fly destinations remain unavailable until
that region's travel/heal systems are ported and verified. Hoenn Fly destinations
are unchanged. This change does not activate Kanto gyms, trainers, shops,
story events, flags, or gifts. Original Kanto terrain and source maps are untouched.

## Battle-only Kanto, without a story

Kanto in **Brisk Emerald** is intended as a second region devoted to Gyms,
ordinary trainers, and its Elite Four, **not** the FireRed/LeafGreen campaign.
Emerald's map compiler excludes rival and Professor Oak plot actors, Team
Rocket NPCs, Silph Co. plot characters, and the rival Champion finale. The
original forced story scenes, roadblocks, Rocket encounters, and quest
triggers remain inactive. Giovanni is retained **only as Viridian City's
Gym Leader**, not as a Team Rocket boss. This conversion occurs only for
Emerald; the original FRLG source maps remain intact.

All eight Gym Leader NPCs, the four Elite Four NPCs, and ordinary Kanto
trainer NPCs are preserved and made visible independently of colliding FRLG
hide flags. **Their trainer battles are not enabled yet.** FRLG's opponent
IDs and defeat flags overlap with Hoenn, so they require a Kanto-specific
roster, independent battle records, and safe scripted challenges before they
can be fought. These characters currently display a short placeholder
message. Neither badges nor story flags are granted or reset.
