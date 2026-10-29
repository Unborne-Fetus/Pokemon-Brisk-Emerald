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
