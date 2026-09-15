
## Table of Contents
  - [Upgraded Battle Engine](#upgraded-battle-engine)
  - [Full Trainer customization](#full-trainer-customization)
  - [Pokémon data](#pokémon-data)
  - [Interface improvements](#interface-improvements)
  - [Overworld improvements](#overworld-improvements)
  - [Dev Tools](#developer-tools)

## Upgraded Battle Engine
- ***Battle gimmicks:*** Mega Evolution, Primal Reversion, Ultra Burst, Z-Moves, Dynamax, Gigantamax and Terastallization.
- ***Newer game battle types:*** Double Wild Battles, custom Multi Battles, Inverse Battles, 1v2/2v1 battles, Sky Battles.
- ***Updated battle mechanics:*** Critical capture, Frostbite support, Poké Ball quick menu, Move description menu, no badge boosts, Gen 4 Fog, obedience, Affection, Party swap upon catch, move effectiveness in battle, FRLG/Gen4+ whiteout money calculation, Gen 4-style shadows.
- ***Updated move data***: Fairy/Stellar types, Physical/Special split, flags.
- ***Updated calculations:*** Damage, experience, mid-turn speed, end-battle stats and EVs, Level 100 EVs.
- ***Every item, ability and move effect up to Gen IX:*** Includes contest data up to SwSh ([source](https://pokemonurpg.com/info/contests/rse-move-list/)).
- ***Initial battle conditions:*** Stat stages, battle terrain, Wild AI flags.
- ***Faster battles:*** Simultaneous HP reduction, shortcut to "Run" option, faster battle intro, faster HP drain, faster AI calculations.
- ***Easier customization:*** Cleaner codebase to implement custom moves and effects.
- ***Improved AI:*** Faster and considers new effects added by Expansion.
- ***Popular features:*** Level/EV Caps, Sleep Clause, Type Indicators.

## Full Trainer customization
- ***Compatible with Pokémon Showdown's team syntax:*** Create your trainer teams in the [teambuilder](https://play.pokemonshowdown.com/teambuilder) and paste the results!
- ***Custom Pokémon data:*** Nicknames, EVs, IVs, Moves, Abilities, Poké Balls, Friendship, Nature, Gender, Shininess, Dynamax level, Gigantamax Factor and Tera Type.
  - ***"Ace Pokémon":*** Will save a specific Pokémon for last.
  - ***Trainer Pools:*** A trainer may get a pool of randomized Pokémon instead of set teams.
- ***Custom sliding trainer messages:*** First Turn, landing a super-effective hit, before Mega Evolution, etc.
- ***New AI Flag options:*** Customize the intelligence of your trainers.
- ***Trainer class Poké Balls:*** Divers use Dive Balls, Breeders use Nest Balls, etc.
## Pokémon data
- ***Improved Pokémon Data structure:*** Optimized space to allow fitting more information, such as Tera type, 12-character names, Hyper-trained stats, evolution conditions, saved HP/status effect.
- ***Updated breeding mechanics:*** Poké Ball/Egg Move/Ability/Nature inheritance, Level 1 eggs automatic incense babies.
- ***Updated species data:*** Stats, Types, Abilities, Hidden Abilities, Egg Groups, EV Yields, movesets, Battle Facility bans, guaranteed perfect IV counts, ORAS Dex numbers.
- ***Simpler species data manipulation:***: Only requires to edit ~5 files instead of vanilla pokeemerald's 20+ to add a new Pokémon.
- ***Updated sprites:*** DS-style sprites with support for Emerald's 2-frame animations and gender difference.
- ***Species toggles:*** You can disable specific groups of Pokémon to save space, including families, cross-gen evolutions, Mega Evolutions, Regional forms, etc.
- ***Revamped Evolution System***: Multiple Evolution conditions can be stacked in order to create complex methods without additional coding. Every condition except Affection and console gyroscope is supported.
- ***Form Change System.*** Most form changes can be added without additional coding. This includes support for: Holding/using an item, HP thresholds being met, weather change in and/or out of battle, Fusions, and more.

## Interface improvements

Box Link added

Second page added in option menu

Overworld and Battle speed up options

Text speed increased

HGSS Pokedex


## Overworld improvements

Overworld encounters.

Map changes

## Developer tools

SPECIES_NONE is now ignored instead of added as an encounter called ????? when placed in the encounter table.

Expanded encounter system. Now allows for 12 pokemon in the water, and a large 25 on land.

Items like Metal Coat and Leaders Crest can be used on a pokemon, rather than just given.
