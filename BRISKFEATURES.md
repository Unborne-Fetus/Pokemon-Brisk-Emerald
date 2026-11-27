# Pokémon Brisk Emerald — Features

Brisk Emerald expands the original Hoenn adventure with Pokémon from Generations I–IX, custom Pokémon and Mega Evolutions, new regions, competitive battles, five difficulty modes, and optional gameplay systems. Built on pokeemerald-expansion 1.17.0.

This is the player-facing guide to the current **master branch**, not a guarantee that an older downloadable ROM contains every feature. Some features require story progression, specific options or difficulty modes. Species implemented in the engine are not necessarily all obtainable in an ordinary playthrough. For inherited engine features and developer tools, see [FEATURES.md](FEATURES.md).

## Contents

- [Pokémon and collecting](#pokémon-and-collecting)
- [Five difficulty modes](#five-difficulty-modes)
- [Hoenn campaign and trainers](#hoenn-campaign-and-trainers)
- [Orange Islands](#orange-islands)
- [Kanto postgame](#kanto-postgame)
- [Team Xiphos Aegis](#team-xiphos-aegis)
- [Dynamax Adventures](#dynamax-adventures)
- [Scott's Trainer Summit](#scotts-trainer-summit)
- [Champions Archive and special bosses](#champions-archive-and-special-bosses)
- [Gameplay options](#gameplay-options)
- [Exploration, catching and training](#exploration-catching-and-training)
- [Randomizer](#randomizer)
- [Interface and presentation](#interface-and-presentation)
- [Customization and contests](#customization-and-contests)
- [Brisk Dex and saves](#brisk-dex-and-saves)
- [Detailed references and credits](#detailed-references-and-credits)

## Pokémon and collecting

- National Pokédex support for the 1,025 official Pokémon through Generation IX, plus additional custom entries. Regional forms, cross-generation evolutions, legendaries, mythicals, Ultra Beasts and Paradox Pokémon are supported.
- Mega Evolution, Primal Reversion, Ultra Burst, Z-Moves, Dynamax, Gigantamax and Terastallization, subject to the usual items, unlocks and battle restrictions.
- Modern moves, abilities, items, types (including Fairy and Stellar), physical/special split, updated breeding, form changes and battle mechanics inherited from Expansion.
- Dozens of community-designed custom Mega Evolutions, including alternative Mega forms with independent stones. The documented distribution includes 67 custom stones. Native 80×80 battle sprites, regular and shiny palettes, and compatible menu previews. Examples include Mega Flygon, Mega Tinkaton, Mega Arboliva, Mega Clawitzer, Mega Hydreigon, Mega Lapras, Mega Noivern and Mega Dragonite Y.
- Custom Pokémon families, additional evolutions of familiar Pokémon, and Halloween forms. Examples: Hissiorite → Cobarett → Pythonova, Baoby → Baobaraffe, Drashimi → Tsushimi → Tobishimi, Slugaqua → Flabaqua → Glautilus, Bulbent → Maguorn → Treewise, Raticate → Bruticate and Noctowl → Noctarch.
- Permanent custom Paradox evolutions: Blastoise → Steam Shell and Garganacl → Iron Furnace via Sun Stone. Both are implemented with Protosynthesis; ordinary Mega and Gigantamax forms remain supported.
- Rhydon has a split evolution: level 60 for Rhyfortress or a Protector for Rhyperior.
- Custom species and forms have their own stats, types, abilities and learnsets where applicable. Some imported families are defined but not yet placed in ordinary encounters or gifts; see individual import notes.
- Item-based evolution alternatives reduce reliance on trading (for example usable Metal Coat and Leader's Crest). Check the in-game evolution information for specific species.
- New legendary, mythical and Ultra Beast acquisitions from static encounters, gifts, special rewards, evolutions and event locations, including Mew on Faraway Island and Deoxys on Birth Island. Story, travel, puzzle and event-item requirements still apply.
- New world pickups include 40 Mega Stones (28 official and 12 custom) and 42 nature mints. Lilycove Department Store 3F sells mints; its 4F Mega Stone clerk offers official and custom selections. Orange City Mart also sells custom stones.

### Signature abilities and balance

Brisk adds custom abilities to give individual Pokémon and selected Megas new strategies. The signature catalogue is being expanded; not every final-stage species has a custom signature yet.

| Pokémon | Example signature | Effect summary |
| --- | --- | --- |
| Spidops | Web Excretant | Entry boosts Speed by two stages and lowers other active battlers' Evasion by one. |
| Kricketune | Grand Maestro | Damaging sound moves use the highest effective non-HP offensive calculation stat. |
| Luvdisc | Heartbreaker | An opponent that knocks it out directly loses half its maximum HP. |
| Delibird | Flash Freeze | Ice attacks have a 40% freeze chance; attacks against frozen targets deal double damage. |
| Sunflora | Solar Fleet | Effective Speed receives a temporary three-stage boost in sun. |
| Carnivine | Jaw Snap | Contact retaliation, stronger against Bug attackers. |
| Watchog | Duran | Three Evasion stages on entry. |
| Wugtrio | Clearwater | All moves gain one priority bracket; incoming attack damage doubles. |
| Furret | Build Me Up | End-turn random stat gain and opposing stat loss. |
| Ariados | Overdose | Successfully inflicting bad poison also causes immediate damage. |
| Magcargo | Burning Crust | Ground immunity and reduced Water damage. |
| Lumineon | Ogooué Basin | Double power for Water attacks. |
| Maractus | Nopales | Reduced Fire damage and Water redirection/absorption. |
| Swalot | Torsion | Half physical damage. |
| Heatmor | Flame Tongue | Fire attacks hit both foes in doubles and attacks deal extra damage to Bug types. |

Other additions include Ledian's Speed Boxer and catalogue batches for Pokémon from Venusaur through Dragonite, including Poison Wart, Flame Tail, Power Cannons, Blind Dive, Queens Gambit, Lunar Power, Magic Tail and more. Many of those signatures are assigned to specific Mega forms, not their regular species. Existing trainers and original ability slots are preserved where specified by each implementation.

**Fuller references:** [signature mechanics](docs/brisk_signature_moves_and_abilities.md), [ten abilities](docs/brisk_ten_signature_abilities.md), [Flash Freeze and Solar Fleet](docs/brisk_flash_freeze_solar_fleet.md), [catalogue batch 1](docs/brisk_signature_abilities_batch1.md), [batch 2](docs/brisk_signature_abilities_batch2.md), and [current ability definitions](src/data/abilities.h).

## Five difficulty modes

Select Easy, Normal, Expert, Hard or Kaizo in Options page 4. Difficulty changes team composition, levels, training, AI, rules, prices and shared EXP rather than simply inflating every opponent's level.

| Mode | Challenge | Style | Battle consumables* | Shared EXP** | Shop prices | EXP cap | Hoenn League |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Easy | More approachable, simpler AI and smaller teams | Shift | Yes | 200% | 1× | None | Lv. 70 |
| Normal | Stronger than original Emerald | Set | Yes | 100% | 2× | Optional | Lv. 100 |
| Expert | Fuller competitive teams, moderated training and fairer AI | Set | Yes | 100% | 3× | Optional | Lv. 100 |
| Hard | Very difficult, optimized teams and strong AI | Set | No | 50% | 4× | Optional | Lv. 100 |
| Kaizo | Highest challenge, predictive AI, full training | Set | No | None | 10× | Mandatory | Lv. 100 |

\* Held items remain usable; Poké Balls remain available in eligible wild fights unless the Bag is disabled. Special facilities have their own rules.  
\** Shared EXP rates do not reduce normal participating battler EXP.

- Easy generally reduces generated opponent levels, uses level-appropriate evolutions and simpler sets, and has no opponent IV/EV training. Your starter remains level 5 and Birch's opening Zigzagoon level 3 in every mode.
- Normal introduces stronger teams and staged training. Expert uses a distinct authored roster variation with moderated IV/EV training and no omniscient AI or move prediction.
- Hard and Kaizo use more complete parties, stronger held items, coverage, support moves, strategic AI and improved training. Kaizo uses perfect IVs, full EV investment and stricter EXP restrictions.
- Hard/Kaizo trainer levels are limited by progression; exact scaling differs between ordinary trainers, story fights and wild encounters. Their gym progression targets are 25, 35, 45, 50, 55, 65, 75 and 90, then 100 for League/postgame.
- Dedicated story, League, Archive, Easter egg and other protected battles may use authored exceptions.
- Original Gen III held-item EXP Share or modern party-wide EXP Share can be selected. The modern Key Item is earned through Devon and toggled in the Bag. Kaizo disables shared EXP.

[Difficulty targets](docs/brisk_mode_targets.md) · [Expert mode](docs/brisk_expert_mode.md) · [Current rules](src/brisk_modes.c) · [Level-cap implementation](docs/tutorials/brisk/difficulty-and-level-caps.md)

## Hoenn campaign and trainers

- Original Hoenn story with revised ordinary trainers, Gym Leaders, rivals, Wally, Team Magma/Aqua, admins, bosses and League teams.
- Difficulty-specific authored parties, improved team composition, evolution-aware early opponents, appropriate held items, explicit moves and abilities, stronger training and smarter AI.
- May/Brendan's teams evolve throughout Hoenn, growing from starter-dependent early lineups into distinct competitive teams that carry their identities into Kanto.
- Gym leader teams and rematches are strengthened. Sidney, Phoebe and Glacia use native Doubles, while Drake and Wallace use Singles; the battle-format option can override ordinary scripted formats.
- All five Hoenn League opponents have six curated Pokémon, level 70 on Easy and level 100 on other modes.
- Optional unlimited rematches against eligible defeated trainers; one-time story rewards and progression flags are not repeated.
- Approximately 5% of ordinary scripted trainer identities have a consistent incidental shiny team member. This selection is not rerolled for every battle.
- Eligible lost or drawn normal trainer/wild battles offer a retry with a fully healed restored party. Retry restores attempt-only changes, items and original opponents rather than sending the player back to a Center. Special modes and battles with already transferred catches retain their own result rules.

[Hoenn rivals](docs/brisk/hoenn-rivals.md) · [League teams](docs/brisk_league_teams.md) · [Story fights](docs/brisk_story_and_balls.md) · [Trainer progression](docs/brisk_trainer_progression.md) · [Battle retry](docs/brisk_battle_retry.md)

## Orange Islands

A tropical expansion connected to Hoenn through Route 141, forest Routes 147–148 and the teleporter to Route 149.

- Routes 143–146 and 149–150, forest approach Routes 147–148, beaches, islands, boardwalks, bridges, sea routes, grass encounters, fishing piers and a scenic lookout.
- Orange City: Pokémon Center, Mega Stone shop, homes, paved streets, town square and harbor.
- Rest benches fully restore party HP, PP and status, including fainted Pokémon. Bench and Center healing sequences are faster.
- Optional ISLAND SHINY gives eligible outdoor island wild encounters independent 1/50 base shiny odds. Forest approach and indoor caves retain their respective rules.
- Tapu Fini on Route 143, Tapu Bulu on Route 145 and Tapu Koko on Route 146.
- Dynamax Adventure research cave on Route 146, plus Kaizo-exclusive challenges in Orange City's plaza.
- Surf east from Route 146 into Orange Strait to reach Kanto.

[Orange Islands maps and access](docs/maps/orange-islands.md) · [Kanto connection](docs/brisk/kanto-connection.md)

## Kanto postgame

An expanded high-level postgame, not a replay of FireRed/LeafGreen's original story.

- Seamless travel through Orange Strait from Route 146 toward Cinnabar Island and Route 20. Return by the same sea connection.
- Kanto cities, routes, caves, NPCs, trainer battles, regional tilesets, palettes and town map artwork.
- Set ten years after Team Rocket's dissolution, with new resident dialogue explaining the region's recovery and increased trainer and wild Pokémon strength.
- Six-member high-level postgame teams. Original mode rosters range from levels 90–92 on Easy to 100 on Kaizo; Expert adds distinct roster selection.
- Gym and League challenges, plus optional talk-to-battle encounters with the visiting May/Brendan and Wally. The original Blue/Oak/Rocket story scenes remain inactive.
- May/Brendan appear on Route 22, S.S. Anne, Silph Co. and in the Champion's Room after Lance. Wally appears in Cerulean, Lavender and Victory Road. The rival's starter becomes their Mega ace; Wally uses Mega Gallade at Victory Road.
- Eleven Kanto cities become Fly destinations upon visiting. Press L in Town Map, Fly or PokéNav map to switch between Hoenn and Kanto; normal travel requirements still apply.

[Kanto travel and map](docs/brisk/kanto-connection.md) · [Trainer teams](docs/brisk/kanto-trainer-teams.md) · [Rival encounters](docs/brisk/kanto-rivals.md)

## Team Xiphos Aegis

A new Kanto villain organization specializing in Fairy, Fighting, Steel and Dragon Pokémon.

- Agents, scientists, senior members, four specialist commanders and Director Jessie: 16 encounter entries across Kanto with five-mode team selection.
- Containment Protocol is an enemy-only battle modifier: the first direct HP hit against an eligible Pokémon on each switch-in is halved; the surviving defender gains +2 Attack, Special Attack and Speed. Protect, Substitute-only hits, hazards and status damage do not consume the effect. It is not a collectible Pokémon ability.
- Commanders are located in Celadon Game Corner, Saffron Dojo, Silph Co. 11F and Victory Road 2F.
- Defeat all four commanders to challenge Jessie in Rocket Hideout B4F. Their double-battle team opens with Aegislash and Archaludon, includes Iron Valiant and places Mega Magearna in the final roster slot. Aegislash and Magearna retain their native abilities rather than Containment Protocol.
- Victory unlocks the Kanto Classic Red/Leaf outfit and a one-time level-70 Magearna reward. A full party and PC defer the gift.
- Rocket-derived encounter sprites are temporary stand-ins; the old Rocket quest remains inactive.

[Encounter locations](docs/brisk/xiphos-kanto-placements.md) · [Commanders and protocol](docs/xiphos_commanders_and_protocol.md) · [Outfit reward](docs/brisk/kanto-classic-jessie-outfit.md)

## Dynamax Adventures

Speak to the scientist in the Route 146 research cave to begin a single-player rental expedition with an AI-controlled Hoenn rival.

- Choose a rental from curated Mega/Gigantamax-capable sets; your partner chooses a different rental. Rentals use competitive moves, abilities, items, natures and EVs.
- Three intermediate bosses followed by a final legendary/mythical encounter. Route selections reveal the primary type rather than the complete identity.
- Dedicated 2v1 combat, with intermediate bosses at 5× ordinary maximum HP and the final boss at 7×. Rentals and intermediate encounters are level 70; final bosses level 100.
- Bosses automatically use their applicable Mega/Gigantamax/Dynamax form; boss Dynamax/Gigantamax lasts until the battle ends. Both allies have independent eligible gimmick allowances.
- Four shared lives, lost when both allies faint; fleeing ends the run. The player's ordinary party remains separate.
- Difficulty-based rental HP and between-round recovery:

| Mode | Rental HP | HP restored after battles |
| --- | --- | --- |
| Easy | 4× | Full, plus status cure |
| Normal | 2× | Full |
| Expert | 1× | 75% of maximum |
| Hard | 1× | 50% of maximum |
| Kaizo | 0.5× | 25% of maximum |

Kaizo bosses deal double direct move damage. PP is restored between rounds.

- No ordinary battle EXP or Bag access; guaranteed optional recruitment after a victory without spending a Poké Ball.
- Swap rentals with catches, store previously caught bench members, and speak to Birch between rounds to switch to an available caught Pokémon. Benched Pokémon retain their HP, status and PP.
- At the end, choose one caught Pokémon to keep. Full storage defers the reward; quitting discards temporary catches. Runs and unclaimed catches do not survive a reset.
- Separate 1/100 shiny odds for Adventure Pokémon, preserved through recruitment and rewards, regardless of ordinary shiny settings.
- Dedicated large boss health bar, compact icon-based rental selection, hidden standard boss healthboxes/type indicators, and faster HP animations.
- Human multiplayer networking, individual-faint life counting and resumption from a save are not implemented as part of the current mode.

[Adventure mechanics and controls](docs/dynamax_adventures.md) · [Expert rules](docs/brisk_expert_mode.md)

## Scott's Trainer Summit

A configurable boss-rush facility on Route 135 featuring previously defeated eligible trainers in randomized order.

- Scott's challenge lobby provides healing and PC access for preparation.
- Choose from all five difficulties independently of the regular journey.
- Configure original/level 50+/level 100 opponents, no/half/full HP recovery, status curing, PP restoration, Bag access, Shift/Set style and gimmick recharge.
- Streak rewards are cumulative within each milestone:

| Wins | Reward bundle |
| --- | --- |
| 1–9 | Random Mega Stone at each win |
| 10 | Mega Stone + random Pokémon |
| 25 | Previous categories + shiny Pokémon |
| 50 | Previous categories + legendary Pokémon |
| 75 | Previous categories + shiny legendary |
| 100, 125, etc. | Previous categories + increasing number of shiny legendaries |

You may stop between rounds. The mode has its own temporary settings rather than permanently rewriting the regular difficulty preferences.

[Current Summit implementation and reward schedule](src/brisk_boss_rush.c)

## Champions Archive and special bosses

- Route 135 opens after entering the Hall of Fame, with a Champions Archive containing major cross-generation champion challenges and special encounters. Archive trainers use strong strategic AI while preserving their curated teams.
- Separate Meteor Falls Steven champion encounter: six level-100 Steel Pokémon with deliberate competitive sets.
- Kaizo-only repeatable Orange City fights: Hooded Figure and Tankerson, each with six level-100 Pokémon. First victories grant a level-70 Necrozma and Guzzlord respectively; rewards can be claimed later if storage is full.
- Special trainer, dedication and Easter egg encounters retain their authored identity and exceptions.

[Steven's team](docs/brisk_steven_champion.md) · [Kaizo boss rosters](docs/brisk_kaizo_bosses.md) · [Legendary acquisition guide](docs/brisk/legendary-locations.md)

## Gameplay options

Use L/R to browse Options pages. Most gameplay preferences persist in the save.

| Option | What it does |
| --- | --- |
| Battle speed | 1×, 2×, 3× or 4× |
| Overworld speed | 1×, 2×, 4× or 8× |
| Auto Run | Run without holding B; enabled by default |
| Move Prompts | Skip the preliminary move-replacement question while retaining move choice/cancellation |
| Level Caps | Toggle progression caps where difficulty permits |
| Rematches | Repeat eligible defeated ordinary trainers |
| Shiny Odds | Choose base odds from 1/8192 through 1/50; default 1/512 |
| Shiny Hunt | Accelerate eligible overworld spawn turnover |
| EXP Share | Gen III held item or modern party-wide Key Item |
| Appearance | Light or Dark |
| Help | In-game explanations of Brisk's systems and controls |
| Change Name | Rename the player without changing Trainer ID or ownership |
| Difficulty | Easy, Normal, Expert, Hard or Kaizo |
| Random Wild / Trainers / Items / Important / Everything | Independent randomizer categories |
| Format | Regular, Singles or Doubles |
| Followers | Toggle your following Pokémon, not scripted companions |
| Island Shiny | Toggle island wild shiny boost |
| Display Types | Toggle known type indicators in ordinary battles |
| Battle Bag | Toggle Bag access including catching; difficulty and special mode restrictions still apply |
| Talk Only | Dedicated trainer interaction preference |
| Mega Mode | Matching held stone maintains Mega form outside battle |
| Debug Menu | Optional engine debugging tools |
| OW Encounters | Visible wild Pokémon on/off; ordinary random encounters remain |
| Gimmick Gen | Recharge an owned Tera Orb after completed battles |
| Mute Sound / Music / Cries | Separate controls for sound effects, music/fanfares and Pokémon cries |
| Auto Battle | Off, Send or Passive |

Ordinary text speed, battle animations, button mapping, stereo/mono and frame preferences remain available.

**Mega Mode:** matching held stones allow eligible Pokémon to remain Mega in the party, PC, overworld and battles, even outside ordinary Mega activation. Multiple eligible team members can be Mega at once; the rule also applies to opponents. Removing a stone or turning the option off restores the relevant base form. Standard Mega Evolution remains available with Mega Mode off.

**Gimmick Gen:** recharges the owned Tera Orb after each completed battle without granting a new Orb, bypassing required items or allowing repeat activation within one battle. Other gimmicks keep normal battle-local limits.

**Battle format:** Regular preserves scripted formats; Singles uses one active Pokémon per side; Doubles uses two when available and adds a second wild Pokémon where eligible. Dedicated facilities and special formats keep their restrictions.

[Gameplay options](docs/brisk/gameplay-options.md) · [Gimmick Gen](docs/brisk_gimmick_gen.md) · [Audio controls](docs/brisk_audio_mutes.md)

## Exploration, catching and training

- Reworked encounter distributions throughout Hoenn, expanded encounter tables with 25 land and 12 water slots; empty SPECIES_NONE slots do not produce invalid encounters.
- Visible roaming wild Pokémon, optional ordinary grass/water encounters, following Pokémon and regional overworld sprites.
- Adjustable base shiny odds: 1/8192, 1/4096, 1/2048, 1/1024, 1/512, 1/256, 1/128, 1/100 and 1/50. Existing Pokémon keep their shiny status. Existing Shiny Charm/DexNav bonuses can modify the base roll.
- Shiny Hunt speeds up encounter turnover while preserving protected shinies and static Pokémon.
- Repels prevent eligible wild encounters regardless of the lead Pokémon's level.
- Overworld Poké Ball catching respects progression/level restrictions. Battle catch-rate information helps choose balls. All 28 ball types use the appropriate battle handling subject to normal catch restrictions.
- Wonder Trade exchanges a Pokémon for a randomly selected eligible trade partner.
- Smart wild battle AI and mode-specific training changes, rather than only difficulty changes to trainers.
- Follower auto-battles: OFF disables them, SEND toggles with R+B and PASSIVE trains while idle. Both followers and visible encounters must be enabled. Follower defeats eligible generated wild encounters without entering battle, earning reduced EXP and no EVs. Shiny, static, story and other protected encounters are excluded; wild Pokémon eight or more levels above the follower cannot be auto-defeated. Respects HP, EXP caps, menus, movement and other field restrictions.
- Improved move relearner in the summary screen: START from moves, L/R to browse level, egg, TM and tutor lists. TM moves need the corresponding TM in the Bag.
- Nature mints, modern breeding, EV/IV information, updated evolution methods and EXP Candy level-up handling.
- Two-in-one bike: stop and press L to switch Mach/Acro modes. Mach mode offers speed and Acro supports tricks and rails.
- Lead Pokémon abilities can create outdoor weather: rain, sunlight, sandstorms, snow and wind effects. Native protected map weather and field scripts take precedence; ability weather cannot create a new Go-Goggles requirement.
- Modern field features inherited from Expansion, including indoor running, Defog, modern Repel prompts, chain fishing, berry mechanics, Box Link and map pop-ups.

[Auto-battle controls](docs/tutorials/brisk/overworld-auto-battles.md) · [Wild/trainer balance](docs/brisk/wild_trainer_balance_2026_11_02.md) · [Shiny settings](docs/tutorials/brisk/shiny-odds.md)

## Randomizer

Options page 4 offers separate switches; combine them as desired.

- **Random Wild:** wild and static species, fishing and overworld spawns.
- **Trainers:** ordinary trainer Pokémon, with appropriate moves and abilities.
- **Items:** ordinary pickups, hidden items, gifts, shop stock and generated held items, preserving important items and HMs.
- **Important:** Gym Leaders, rivals, League and other major fights.
- **Everything:** all categories above plus starters, gifts, eggs, species types, abilities, stats, learnsets and evolution destinations.

Randomization is deterministic for the save's Trainer ID. Existing Pokémon retain their identities and acquired moves; relevant live species traits follow the enabled setting. Evolution requirements remain native to the species, while valid evolution destinations are randomized. Key story flags, map entrances and warps are not randomized. Disabled species, eggs and temporary battle transformations are excluded from ordinary randomized species selection.

[Randomizer behavior](docs/brisk/gameplay-options.md) · [Implementation](src/brisk_modes.c)

## Interface and presentation

- HGSS-style expanded Pokédex with detailed species information and browsing.
- DexNav available upon receiving the Pokédex; hidden encounter detection unlocks with the PokéNav. Expanded land/water lists show 12/5 species per page, L/R paging, SELECT registration and A searching. R in the field repeats a search. Empty slots are filtered and completion checks the full encounter table.
- Compact expanded start menu, Box Link and twenty PC boxes.
- Faster text, full-screen transitions, battle introductions, healing, HP animations, menu navigation, D-pad repeat and save messages.
- Separate battle and overworld speed settings allow faster play without relying on emulator fast-forward.
- Dark appearance uses slate surfaces, raised panels, cyan selections and readable text across menus, Bag, PC, shop, battle interface, trainer card, naming screen and HGSS Pokédex. Pokémon artwork, type colors, status bars and item colors remain intact. Light mode restores the original UI assets.
- Environmental palette lighting differentiates plains, coasts, islands, forests, mountains, volcanoes, caves, ice, underwater, ruins, haunted maps, industrial areas and indoor spaces. Outdoor palettes vary with time of day; interiors and specialized dungeons have suitable ambient lighting.
- Regular battle type indicators and catch-rate display, nature colors, EV/IV checks and improved summary screen.
- Expanded regional Town Map/Fly browsing between Hoenn and Kanto.

[Environmental lighting](docs/brisk/environment_lighting.md) · [Gameplay UI and options](docs/brisk/gameplay-options.md) · [Expansion reference](FEATURES.md)

## Customization and contests

- Ball Capsules: Party → Pokémon → Item → Ball Capsule. Three reusable seal slots, including hearts, stars, bubbles, lines, smoke, electricity, fire, confetti, flowers, music notes, letters A–Z and punctuation. Set individual seals without consuming a held-item slot; capsules persist through boxing and saving.
- Custom send-out text: select a preset or one of three save-wide editable phrases (up to 15 characters each) independently per Pokémon. A Pokémon can use a phrase and seals together.
- Name changes via Options retain Trainer ID and Pokémon ownership.
- Kanto Classic Red/Leaf outfit unlocked by defeating Director Jessie.
- Rhythm contests: tap A on the announced beat (2, 3 or 4) during solo appeals. Perfect adds four hearts, Good adds two, and a miss adds no penalty. Timing tightens at higher ranks and NPC accuracy scales with rank. Existing categories, moves, combos, ribbons and link contests are retained.
- Separate music, effects and cries mute settings; rhythm cues can be followed visually without sound.

[Ball Capsules and phrases](docs/ball_capsules.md) · [Contest rhythm](docs/contest_rhythm.md) · [Kanto outfit](docs/brisk/kanto-classic-jessie-outfit.md)

## Brisk Dex and saves

The standalone ROM uses a standard Continue/New Game menu and one ordinary save, with internal corruption-recovery banks and twenty PC boxes. The old selectable two-profile ROM menu is removed. The legacy compressed first slot remains compatible, but old dual-profile saves require careful migration to avoid losing the second slot.

[Brisk Dex](https://github.com/Unborne-Fetus/Brisk-Dex) is the separate companion/integrated-game project. Its documented desktop game supports two independent external save snapshots and migration of legacy two-profile saves, preserving a recovery copy. Platform features, releases and runtime status are maintained separately in that repository; this guide does not claim that every app build is currently working or available.

[Save ownership and migration](docs/brisk/save-system-modes.md)

## Detailed references and credits

| Topic | Reference |
| --- | --- |
| Base engine capabilities and developer tools | [FEATURES.md](FEATURES.md) |
| Legendary, mythical and Ultra Beast locations | [Acquisition guide](docs/brisk/legendary-locations.md) |
| Mega Stones and nature mints | [Item location guide](docs/brisk_item_distribution.md) |
| Custom Megas | [Sprite and form guide](docs/custom_mega_sprites.md), [KFWeagz forms](docs/kfweagz_forms.md) |
| Custom species, evolution lines, Halloween forms | [princess-phoenix](docs/phoenix_fakemon.md), [CDRX73](docs/cdrx73_fakemon.md), [Sacred Phoenix](docs/sacred_phoenix_fakemon.md) |
| Trainer teams | [Authored modes](docs/brisk_authored_mode_teams.md), [ordinary trainers](docs/brisk_ordinary_trainers.md), [trainer plans](docs/brisk_trainer_mode_plan.md), [Kanto teams](docs/brisk/kanto-trainer-teams.md) |
| Community feature porting | [Tutorial hub](docs/tutorials/brisk/README.md) |
| All documentation | [Documentation index](docs/SUMMARY.md) |
| Artists, base engine and contributors | [Credits](CREDITS.md), [README](README.md) |

The earlier missing-Pokémon checklist has been superseded by the maintained legendary acquisition guide and the individual custom species placement notes. Those describe acquisition sources, not proof that every species or every story route has been successfully playtested in a released build.

Brisk Emerald is built on the work of the pokeemerald-expansion community and many independent artists. Individual asset sources and applicable licenses are retained in the linked credits and import documentation.

