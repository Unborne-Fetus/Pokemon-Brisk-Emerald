# Ball Capsules and send-out text

Party → Pokémon → ITEM now offers two separate features.

## Ball Capsule

Three seal slots each accept NONE, Heart, Star, Bubbles, Line, Smoke,
Electricity, Fire, Confetti, Flowers, Music Note, A–Z, !, or ?.
Up/Down changes the slot; Left/Right changes the seal; A saves; B discards.
Seals produce particles when the Pokémon leaves its ball. Smoke and fire
rise, electricity jitters, confetti and flowers fall, music floats, and
letters and the remaining shapes burst outward.

## Send-out text

Each Pokémon independently selects NONE, one of the existing seven phrases,
or Custom 1/2/3. The three custom phrases are shared across the save, so
editing one changes the wording for every Pokémon using that phrase slot.
Each allows 15 characters. Select a custom slot and press START to open
the game's keyboard; completing the keyboard saves the phrase and assigns
it to the selected Pokémon. A saves a preset choice; B discards selection
changes. Selecting NONE disables the extra line without removing seals.

## Save compatibility

Pokémon, boxed Pokémon, and SaveBlock1 sizes and field offsets are unchanged.
Existing 2-bit Heart/Star/Bubbles slots decode automatically. Expanded seals
use three base-39 digits, stored with a 64 offset in 16 formerly unused/
legacy bits. Existing phrase choices 0–7 are retained; a formerly unused
bit extends choices to 8–10. Custom phrases occupy the old 52-byte unused
Dex flag filler, with a version marker and three bounded 16-byte strings.
No active Pokédex flags are reused. The old MON_DATA_BALL_CAPSULE interface
remains available for legacy data; new editors use MON_DATA_BALL_CAPSULE_SEALS.
