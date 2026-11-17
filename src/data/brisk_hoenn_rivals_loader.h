// The original Hoenn May/Brendan battles keep their scripts and trainer IDs.
// This party loader supersedes their older generic mode rosters at battle
// time, while leaving Route 103's single starter tutorial untouched.
#include "brisk_hoenn_rivals.h"

static const u8 sBriskHoennRivalSupports[2][HOENN_RIVAL_STAGE_COUNT][5] =
{
    [HOENN_RIVAL_MAY] =
    {
        [HOENN_RIVAL_RUSTBORO] = {HOENN_RIVAL_MON_WINGULL, HOENN_RIVAL_MON_ELECTRIKE, HOENN_RIVAL_MON_SHROOMISH, HOENN_RIVAL_MON_MARILL, HOENN_RIVAL_MON_TRAPINCH},
        [HOENN_RIVAL_ROUTE110] = {HOENN_RIVAL_MON_PELIPPER, HOENN_RIVAL_MON_BRELOOM, HOENN_RIVAL_MON_MANECTRIC, HOENN_RIVAL_MON_VIBRAVA, HOENN_RIVAL_MON_METANG},
        [HOENN_RIVAL_ROUTE119] = {HOENN_RIVAL_MON_PELIPPER, HOENN_RIVAL_MON_BRELOOM, HOENN_RIVAL_MON_MANECTRIC, HOENN_RIVAL_MON_MILOTIC, HOENN_RIVAL_MON_METAGROSS},
        [HOENN_RIVAL_LILYCOVE] = {HOENN_RIVAL_MON_PELIPPER, HOENN_RIVAL_MON_BRELOOM, HOENN_RIVAL_MON_MANECTRIC, HOENN_RIVAL_MON_MILOTIC, HOENN_RIVAL_MON_METAGROSS},
    },
    [HOENN_RIVAL_BRENDAN] =
    {
        [HOENN_RIVAL_RUSTBORO] = {HOENN_RIVAL_MON_SHROOMISH, HOENN_RIVAL_MON_ELECTRIKE, HOENN_RIVAL_MON_WINGULL, HOENN_RIVAL_MON_MARILL, HOENN_RIVAL_MON_TRAPINCH},
        [HOENN_RIVAL_ROUTE110] = {HOENN_RIVAL_MON_BRELOOM, HOENN_RIVAL_MON_MANECTRIC, HOENN_RIVAL_MON_VIBRAVA, HOENN_RIVAL_MON_PELIPPER, HOENN_RIVAL_MON_METANG},
        [HOENN_RIVAL_ROUTE119] = {HOENN_RIVAL_MON_BRELOOM, HOENN_RIVAL_MON_MANECTRIC, HOENN_RIVAL_MON_FLYGON, HOENN_RIVAL_MON_MILOTIC, HOENN_RIVAL_MON_METAGROSS},
        [HOENN_RIVAL_LILYCOVE] = {HOENN_RIVAL_MON_FLYGON, HOENN_RIVAL_MON_BRELOOM, HOENN_RIVAL_MON_MANECTRIC, HOENN_RIVAL_MON_MILOTIC, HOENN_RIVAL_MON_METAGROSS},
    },
};

static const u8 sBriskHoennRivalAceLevels[HOENN_RIVAL_STAGE_COUNT][5] =
{
    // Normal, Easy, Hard, Kaizo, Expert. Existing mode IDs/save values.
    [HOENN_RIVAL_RUSTBORO] = {24,12,25,25,25},
    [HOENN_RIVAL_ROUTE110] = {44,22,45,45,45},
    [HOENN_RIVAL_ROUTE119] = {65,33,65,65,65},
    [HOENN_RIVAL_LILYCOVE] = {74,37,75,75,75},
};
static const u8 sBriskHoennRivalCounts[HOENN_RIVAL_STAGE_COUNT][5] =
{
    [HOENN_RIVAL_RUSTBORO] = {3,2,4,5,4},
    [HOENN_RIVAL_ROUTE110] = {4,3,5,6,5},
    [HOENN_RIVAL_ROUTE119] = {5,4,6,6,6},
    [HOENN_RIVAL_LILYCOVE] = {6,4,6,6,6},
};
static const u8 sBriskHoennRivalEmptyEvs[NUM_STATS] = {0};
static const u8 sBriskHoennRivalNormalEvs[NUM_STATS] = {32,64,32,64,32,64};

static u8 BriskHoennRivalAppropriateSpecies(u8 index, u8 level)
{
    // Route 110 and Easy-mode encounters must respect evolution levels.
    switch (index)
    {
    case HOENN_RIVAL_MON_PELIPPER: if (level < 25) return HOENN_RIVAL_MON_WINGULL; break;
    case HOENN_RIVAL_MON_MANECTRIC: if (level < 26) return HOENN_RIVAL_MON_ELECTRIKE; break;
    case HOENN_RIVAL_MON_BRELOOM: if (level < 23) return HOENN_RIVAL_MON_SHROOMISH; break;
    case HOENN_RIVAL_MON_FLYGON:
        if (level < 35) return HOENN_RIVAL_MON_TRAPINCH;
        if (level < 45) return HOENN_RIVAL_MON_VIBRAVA;
        break;
    case HOENN_RIVAL_MON_VIBRAVA: if (level < 35) return HOENN_RIVAL_MON_TRAPINCH; break;
    case HOENN_RIVAL_MON_METAGROSS: if (level < 45) return HOENN_RIVAL_MON_METANG; break;
    }
    return index;
}

static bool32 BriskLoadHoennRivalParty(struct Pokemon *party, u16 trainerNum)
{
    u8 who, stage, ace, i, count, mode, highest, choice, level;
    struct TrainerMon mons[PARTY_SIZE];
    struct Trainer trainer;

    switch (trainerNum)
    {
    case TRAINER_MAY_RUSTBORO_TREECKO:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_RUSTBORO; ace = HOENN_RIVAL_MON_BLAZIKEN; break;
    case TRAINER_MAY_RUSTBORO_TORCHIC:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_RUSTBORO; ace = HOENN_RIVAL_MON_SWAMPERT; break;
    case TRAINER_MAY_RUSTBORO_MUDKIP:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_RUSTBORO; ace = HOENN_RIVAL_MON_SCEPTILE; break;
    case TRAINER_MAY_ROUTE_110_TREECKO:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_ROUTE110; ace = HOENN_RIVAL_MON_BLAZIKEN; break;
    case TRAINER_MAY_ROUTE_110_TORCHIC:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_ROUTE110; ace = HOENN_RIVAL_MON_SWAMPERT; break;
    case TRAINER_MAY_ROUTE_110_MUDKIP:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_ROUTE110; ace = HOENN_RIVAL_MON_SCEPTILE; break;
    case TRAINER_MAY_ROUTE_119_TREECKO:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_ROUTE119; ace = HOENN_RIVAL_MON_BLAZIKEN; break;
    case TRAINER_MAY_ROUTE_119_TORCHIC:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_ROUTE119; ace = HOENN_RIVAL_MON_SWAMPERT; break;
    case TRAINER_MAY_ROUTE_119_MUDKIP:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_ROUTE119; ace = HOENN_RIVAL_MON_SCEPTILE; break;
    case TRAINER_MAY_LILYCOVE_TREECKO:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_LILYCOVE; ace = HOENN_RIVAL_MON_BLAZIKEN; break;
    case TRAINER_MAY_LILYCOVE_TORCHIC:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_LILYCOVE; ace = HOENN_RIVAL_MON_SWAMPERT; break;
    case TRAINER_MAY_LILYCOVE_MUDKIP:
        who = HOENN_RIVAL_MAY; stage = HOENN_RIVAL_LILYCOVE; ace = HOENN_RIVAL_MON_SCEPTILE; break;
    case TRAINER_BRENDAN_RUSTBORO_TREECKO:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_RUSTBORO; ace = HOENN_RIVAL_MON_BLAZIKEN; break;
    case TRAINER_BRENDAN_RUSTBORO_TORCHIC:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_RUSTBORO; ace = HOENN_RIVAL_MON_SWAMPERT; break;
    case TRAINER_BRENDAN_RUSTBORO_MUDKIP:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_RUSTBORO; ace = HOENN_RIVAL_MON_SCEPTILE; break;
    case TRAINER_BRENDAN_ROUTE_110_TREECKO:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_ROUTE110; ace = HOENN_RIVAL_MON_BLAZIKEN; break;
    case TRAINER_BRENDAN_ROUTE_110_TORCHIC:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_ROUTE110; ace = HOENN_RIVAL_MON_SWAMPERT; break;
    case TRAINER_BRENDAN_ROUTE_110_MUDKIP:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_ROUTE110; ace = HOENN_RIVAL_MON_SCEPTILE; break;
    case TRAINER_BRENDAN_ROUTE_119_TREECKO:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_ROUTE119; ace = HOENN_RIVAL_MON_BLAZIKEN; break;
    case TRAINER_BRENDAN_ROUTE_119_TORCHIC:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_ROUTE119; ace = HOENN_RIVAL_MON_SWAMPERT; break;
    case TRAINER_BRENDAN_ROUTE_119_MUDKIP:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_ROUTE119; ace = HOENN_RIVAL_MON_SCEPTILE; break;
    case TRAINER_BRENDAN_LILYCOVE_TREECKO:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_LILYCOVE; ace = HOENN_RIVAL_MON_BLAZIKEN; break;
    case TRAINER_BRENDAN_LILYCOVE_TORCHIC:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_LILYCOVE; ace = HOENN_RIVAL_MON_SWAMPERT; break;
    case TRAINER_BRENDAN_LILYCOVE_MUDKIP:
        who = HOENN_RIVAL_BRENDAN; stage = HOENN_RIVAL_LILYCOVE; ace = HOENN_RIVAL_MON_SCEPTILE; break;
    default: return FALSE; // Route 103, placeholders and all other Hoenn trainers unchanged.
    }

    mode = GetBriskDifficulty();
    if (mode > BRISK_MODE_MAX)
        mode = BRISK_MODE_NORMAL;
    highest = sBriskHoennRivalAceLevels[stage][mode];
    count = sBriskHoennRivalCounts[stage][mode];

    for (i = 0; i < count; i++)
    {
        // Keep the starter as the ace regardless of team size on Easy/Normal.
        level = highest - (count - 1 - i);
        if (i == count - 1)
        {
            choice = ace;
            if (level < 36)
            {
                if (ace == HOENN_RIVAL_MON_SCEPTILE) choice = HOENN_RIVAL_MON_GROVYLE;
                if (ace == HOENN_RIVAL_MON_BLAZIKEN) choice = HOENN_RIVAL_MON_COMBUSKEN;
                if (ace == HOENN_RIVAL_MON_SWAMPERT) choice = HOENN_RIVAL_MON_MARSHTOMP;
            }
        }
        else
        {
            choice = sBriskHoennRivalSupports[who][stage][i];
            // Show May's Salamence developing in the toughest late Hoenn battles.
            if (who == HOENN_RIVAL_MAY
             && (stage == HOENN_RIVAL_ROUTE119 && mode == BRISK_MODE_KAIZO
                 || stage == HOENN_RIVAL_LILYCOVE
                    && (mode == BRISK_MODE_HARD || mode == BRISK_MODE_KAIZO || mode == BRISK_MODE_EXPERT))
             && i == 0)
                choice = HOENN_RIVAL_MON_SALAMENCE;
            // Easy Brendan's Lilycove Vibrava has evolved naturally at Lv36.
            if (who == HOENN_RIVAL_BRENDAN && stage == HOENN_RIVAL_LILYCOVE
             && mode == BRISK_MODE_EASY && i < 3)
            {
                static const u8 easySupports[] = {
                    HOENN_RIVAL_MON_BRELOOM, HOENN_RIVAL_MON_MANECTRIC, HOENN_RIVAL_MON_FLYGON
                };
                choice = easySupports[i];
            }
            choice = BriskHoennRivalAppropriateSpecies(choice, level);
        }

        mons[i] = sBriskHoennRivalMons[choice];
        mons[i].lvl = level;
        if (mode == BRISK_MODE_EASY)
        {
            mons[i].iv = 0;
            mons[i].ev = sBriskHoennRivalEmptyEvs;
            mons[i].heldItem = ITEM_NONE;
        }
        else if (mode == BRISK_MODE_NORMAL)
        {
            mons[i].iv = level < 50 ? 24 * 0x2108421 : 0x3FFFFFFF;
            if (level < 50)
                mons[i].ev = sBriskHoennRivalNormalEvs;
            if (stage == HOENN_RIVAL_RUSTBORO)
                mons[i].heldItem = ITEM_ORAN_BERRY;
            else if (stage == HOENN_RIVAL_ROUTE110)
                mons[i].heldItem = ITEM_SITRUS_BERRY;
        }
        else if (stage == HOENN_RIVAL_RUSTBORO)
            mons[i].heldItem = ITEM_ORAN_BERRY;
    }
    trainer = *GetTrainerStructFromId(trainerNum);
    trainer.party = mons;
    trainer.partySize = count;
    trainer.authoredParty = TRUE;
    if (mode != BRISK_MODE_EASY)
        trainer.aiFlags |= AI_FLAG_BASIC_TRAINER | AI_FLAG_SMART_SWITCHING
                         | AI_FLAG_HP_AWARE | AI_FLAG_TRY_TO_2HKO;
    CreateNPCTrainerPartyFromTrainer(party, &trainer);
    return TRUE;
}
