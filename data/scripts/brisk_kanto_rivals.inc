@ Kanto is a story-free postgame region. These battles are optional to start
@ (except the Champion, at the end of the League), and never trigger FRLG quests.
@ Uses the Kanto save-extension defeat flags, not the Hoenn rival's victories.

BriskKantoRival_GfxMapScripts::
    map_script MAP_SCRIPT_ON_LOAD, BriskKantoRival_GfxMapLoad
    .byte 0

BriskKantoRival_GfxMapLoad::
    call Common_EventScript_SetupRivalGfxId
    end

BriskKantoRival_ChampionGfxMapScripts::
    map_script MAP_SCRIPT_ON_LOAD, BriskKantoRival_ChampionGfxMapLoad
    .byte 0

BriskKantoRival_ChampionGfxMapLoad::
    call Common_EventScript_SetupRivalGfxId
    setmetatile 6, 1, METATILE_PokemonLeague_Door_Top_Open, 1
    setmetatile 6, 2, METATILE_PokemonLeague_Door_Mid_Open, 0
    end

BriskKantoRival_EventScript_Route22::
    checkplayergender
    goto_if_eq VAR_RESULT, MALE, BriskKantoRival_EventScript_Route22May
    goto BriskKantoRival_EventScript_Route22Brendan

BriskKantoRival_EventScript_Route22May::
    goto_if_defeated TRAINER_KANTO_MAY_ROUTE22, BriskKantoRival_EventScript_Route22LateMay
    trainerbattle_single TRAINER_KANTO_MAY_ROUTE22, BriskKantoRival_Text_EarlyMay, BriskKantoRival_Text_RivalDefeated
    msgbox BriskKantoRival_Text_AfterEarly, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_Route22Brendan::
    goto_if_defeated TRAINER_KANTO_BRENDAN_ROUTE22, BriskKantoRival_EventScript_Route22LateBrendan
    trainerbattle_single TRAINER_KANTO_BRENDAN_ROUTE22, BriskKantoRival_Text_EarlyBrendan, BriskKantoRival_Text_RivalDefeated
    msgbox BriskKantoRival_Text_AfterEarly, MSGBOX_AUTOCLOSE
    end

@ The second Route 22 match appears on a return visit after meeting Wally in Lavender.
BriskKantoRival_EventScript_Route22LateMay::
    goto_if_defeated TRAINER_KANTO_MAY_ROUTE22_LATE, BriskKantoRival_EventScript_RivalAfter
    goto_if_not_defeated TRAINER_KANTO_WALLY_LAVENDER, BriskKantoRival_EventScript_RivalAfter
    trainerbattle_single TRAINER_KANTO_MAY_ROUTE22_LATE, BriskKantoRival_Text_LateMay, BriskKantoRival_Text_RivalDefeated
    msgbox BriskKantoRival_Text_AfterLate, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_Route22LateBrendan::
    goto_if_defeated TRAINER_KANTO_BRENDAN_ROUTE22_LATE, BriskKantoRival_EventScript_RivalAfter
    goto_if_not_defeated TRAINER_KANTO_WALLY_LAVENDER, BriskKantoRival_EventScript_RivalAfter
    trainerbattle_single TRAINER_KANTO_BRENDAN_ROUTE22_LATE, BriskKantoRival_Text_LateBrendan, BriskKantoRival_Text_RivalDefeated
    msgbox BriskKantoRival_Text_AfterLate, MSGBOX_AUTOCLOSE
    end

BriskKantoRival_EventScript_SSAnne::
    checkplayergender
    goto_if_eq VAR_RESULT, MALE, BriskKantoRival_EventScript_SSAnneMay
    goto BriskKantoRival_EventScript_SSAnneBrendan
BriskKantoRival_EventScript_SSAnneMay::
    goto_if_defeated TRAINER_KANTO_MAY_SS_ANNE, BriskKantoRival_EventScript_RivalAfter
    trainerbattle_single TRAINER_KANTO_MAY_SS_ANNE, BriskKantoRival_Text_SSAnneMay, BriskKantoRival_Text_RivalDefeated
    msgbox BriskKantoRival_Text_AfterSSAnne, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_SSAnneBrendan::
    goto_if_defeated TRAINER_KANTO_BRENDAN_SS_ANNE, BriskKantoRival_EventScript_RivalAfter
    trainerbattle_single TRAINER_KANTO_BRENDAN_SS_ANNE, BriskKantoRival_Text_SSAnneBrendan, BriskKantoRival_Text_RivalDefeated
    msgbox BriskKantoRival_Text_AfterSSAnne, MSGBOX_AUTOCLOSE
    end

BriskKantoRival_EventScript_WallyCerulean::
    goto_if_defeated TRAINER_KANTO_WALLY_CERULEAN, BriskKantoRival_EventScript_WallyAfter
    trainerbattle_single TRAINER_KANTO_WALLY_CERULEAN, BriskKantoRival_Text_WallyCerulean, BriskKantoRival_Text_WallyDefeated
    msgbox BriskKantoRival_Text_WallyCeruleanAfter, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_WallyLavender::
    goto_if_defeated TRAINER_KANTO_WALLY_LAVENDER, BriskKantoRival_EventScript_WallyAfter
    trainerbattle_single TRAINER_KANTO_WALLY_LAVENDER, BriskKantoRival_Text_WallyLavender, BriskKantoRival_Text_WallyDefeated
    msgbox BriskKantoRival_Text_WallyLavenderAfter, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_WallyVictory::
    goto_if_defeated TRAINER_KANTO_WALLY_VICTORY_ROAD, BriskKantoRival_EventScript_WallyAfter
    trainerbattle_single TRAINER_KANTO_WALLY_VICTORY_ROAD, BriskKantoRival_Text_WallyVictory, BriskKantoRival_Text_WallyDefeated
    msgbox BriskKantoRival_Text_WallyVictoryAfter, MSGBOX_AUTOCLOSE
    end

BriskKantoRival_EventScript_Champion::
    checkplayergender
    goto_if_eq VAR_RESULT, MALE, BriskKantoRival_EventScript_ChampionMay
    goto BriskKantoRival_EventScript_ChampionBrendan
BriskKantoRival_EventScript_ChampionMay::
    goto_if_defeated TRAINER_KANTO_MAY_CHAMPION, BriskKantoRival_EventScript_ChampionMayRematch
    trainerbattle_single TRAINER_KANTO_MAY_CHAMPION, BriskKantoRival_Text_ChampionMay, BriskKantoRival_Text_ChampionDefeated
    msgbox BriskKantoRival_Text_AfterChampion, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_ChampionBrendan::
    goto_if_defeated TRAINER_KANTO_BRENDAN_CHAMPION, BriskKantoRival_EventScript_ChampionBrendanRematch
    trainerbattle_single TRAINER_KANTO_BRENDAN_CHAMPION, BriskKantoRival_Text_ChampionBrendan, BriskKantoRival_Text_ChampionDefeated
    msgbox BriskKantoRival_Text_AfterChampion, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_ChampionMayRematch::
    goto_if_defeated TRAINER_KANTO_MAY_CHAMPION_REMATCH, BriskKantoRival_EventScript_RivalAfter
    trainerbattle_single TRAINER_KANTO_MAY_CHAMPION_REMATCH, BriskKantoRival_Text_RematchMay, BriskKantoRival_Text_ChampionDefeated
    msgbox BriskKantoRival_Text_AfterChampion, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_ChampionBrendanRematch::
    goto_if_defeated TRAINER_KANTO_BRENDAN_CHAMPION_REMATCH, BriskKantoRival_EventScript_RivalAfter
    trainerbattle_single TRAINER_KANTO_BRENDAN_CHAMPION_REMATCH, BriskKantoRival_Text_RematchBrendan, BriskKantoRival_Text_ChampionDefeated
    msgbox BriskKantoRival_Text_AfterChampion, MSGBOX_AUTOCLOSE
    end

BriskKantoRival_EventScript_RivalAfter::
    msgbox BriskKantoRival_Text_RivalAfter, MSGBOX_AUTOCLOSE
    end
BriskKantoRival_EventScript_WallyAfter::
    msgbox BriskKantoRival_Text_WallyAfter, MSGBOX_AUTOCLOSE
    end

BriskKantoRival_Text_EarlyMay:
    .string "MAY: You found KANTO, too!\\n"
    .string "Ready for another battle?$"
BriskKantoRival_Text_EarlyBrendan:
    .string "BRENDAN: Let's see how your\\n"
    .string "team handles KANTO!$"
BriskKantoRival_Text_SSAnneMay:
    .string "MAY: New region, new teams!\\n"
    .string "Try to keep up!$"
BriskKantoRival_Text_SSAnneBrendan:
    .string "BRENDAN: I have been training\\n"
    .string "for this rematch!$"
BriskKantoRival_Text_LateMay:
    .string "MAY: I've kept training!\\n"
    .string "How far have you come?$"
BriskKantoRival_Text_LateBrendan:
    .string "BRENDAN: Back for another\\n"
    .string "round? I am ready!$"
BriskKantoRival_Text_ChampionMay:
    .string "MAY: One last challenge.\\n"
    .string "Let's battle, Champion!$"
BriskKantoRival_Text_ChampionBrendan:
    .string "BRENDAN: This is the top.\\n"
    .string "Show me everything!$"
BriskKantoRival_Text_RematchMay:
    .string "MAY: I've improved my team.\\n"
    .string "Here's our rematch!$"
BriskKantoRival_Text_RematchBrendan:
    .string "BRENDAN: Time for a tougher\\n"
    .string "rematch!$"
BriskKantoRival_Text_RivalDefeated:
    .string "You keep getting stronger!$"
BriskKantoRival_Text_ChampionDefeated:
    .string "You truly are the Champion!$"
BriskKantoRival_Text_AfterEarly:
    .string "KANTO is full of challenges.\\n"
    .string "I'll see you on the road!$"
BriskKantoRival_Text_AfterSSAnne:
    .string "There is so much more to\\n"
    .string "explore here!$"
BriskKantoRival_Text_AfterLate:
    .string "You've earned your place at\\n"
    .string "the LEAGUE.$"
BriskKantoRival_Text_AfterChampion:
    .string "Great battle! The path to\\n"
    .string "the HALL OF FAME is open.$"
BriskKantoRival_Text_RivalAfter:
    .string "Keep exploring KANTO.\\n"
    .string "Our journey isn't over.$"
BriskKantoRival_Text_WallyCerulean:
    .string "WALLY: The air here feels\\n"
    .string "fresh! Want to battle?$"
BriskKantoRival_Text_WallyLavender:
    .string "WALLY: I trained every day.\\n"
    .string "Please battle with me!$"
BriskKantoRival_Text_WallyVictory:
    .string "WALLY: This is my best team.\\n"
    .string "I won't hold back!$"
BriskKantoRival_Text_WallyDefeated:
    .string "I still have so much to learn!$"
BriskKantoRival_Text_WallyCeruleanAfter:
    .string "Thanks! I'll keep exploring\\n"
    .string "and training around KANTO.$"
BriskKantoRival_Text_WallyLavenderAfter:
    .string "I think I'm getting better\\n"
    .string "each time we battle!$"
BriskKantoRival_Text_WallyVictoryAfter:
    .string "That was a great battle!\\n"
    .string "Good luck at the LEAGUE.$"
BriskKantoRival_Text_WallyAfter:
    .string "Thank you! I'll keep on\\n"
    .string "training.$"
