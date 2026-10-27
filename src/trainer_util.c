#include "global.h"
#include "brisk_modes.h"
#include "brisk_trainer_moves.h"
#include "main.h"
#include "data.h"
#include "move.h"
#include "random.h"
#include "string_util.h"
#include "trainer_util.h"
#include "text.h"

#include "constants/battle_ai.h"
#include "constants/pokeball.h"
#include "constants/opponents.h"
#include "data/brisk_ordinary_trainers.h"

static bool32 UsesFairLoadout(const struct Trainer *trainer)
{
#ifndef EMERALD
    return FALSE; // The manifest identifies Hoenn teams, not FRLG trainer IDs.
#else
    u32 address = (u32)trainer;
    u32 start = (u32)gTrainers;
    if (address < start || address >= start + sizeof(gTrainers))
        return FALSE; // Partners, generated Frontier teams and custom fixtures.
    u32 offset = address - start;
    if (offset % sizeof(struct Trainer) != 0)
        return FALSE;
    return sBriskFairTrainer[(offset / sizeof(struct Trainer)) % TRAINERS_COUNT];
#endif
}

rng_value_t GeneratePartySeed(const struct Trainer *trainer)
{
    u32 seed = Crc32B((const u8 *)trainer, sizeof(struct Trainer)) ^ READ_OTID_FROM_SAVE;
    return LocalRandomSeed(seed);
}

static void CustomTrainerPartyAssignMoves(struct Pokemon *mon, const struct TrainerMon *partyEntry)
{
    bool32 noMoveSet = TRUE;
    u32 j;

    for (j = 0; j < MAX_MON_MOVES; ++j)
    {
        if (partyEntry->moves[j] != MOVE_NONE)
            noMoveSet = FALSE;
    }
    if (noMoveSet)
    {
        GiveMonInitialMoveset(mon);
        // TODO: Figure out a default strategy when moves are not set, to generate a good moveset
        return;
    }

    for (j = 0; j < MAX_MON_MOVES; ++j)
    {
        u32 pp = GetMovePP(partyEntry->moves[j]);
        SetMonData(mon, MON_DATA_MOVE1 + j, &partyEntry->moves[j]);
        SetMonData(mon, MON_DATA_PP1 + j, &pp);
    }
}

u32 GeneratePersonalityForGender(u32 gender, u32 species)
{
    const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[species];
    if (gender == MON_MALE)
    {
        assertf(speciesInfo->genderRatio < MON_FEMALE, "species %d cannot be male", species);
        return ((255 - speciesInfo->genderRatio) / 2) + speciesInfo->genderRatio;
    }
    if (gender == MON_FEMALE)
    {
        assertf(speciesInfo->genderRatio != MON_MALE && speciesInfo->genderRatio != MON_GENDERLESS, "species %d cannot be female", species);
        return speciesInfo->genderRatio / 2;
    }
    if (gender == MON_GENDERLESS)
        assertf(speciesInfo->genderRatio == MON_GENDERLESS, "species %d cannot be genderless", species);
    else
        errorf("GeneratePersonalityForGender called with invalid gender value %d", gender);
    return 0;
}

const u8 sModuloLUT[25] = {0, 21, 17, 13, 9, 5, 1, 22, 18, 14, 10, 6, 2, 23, 19, 15, 11, 7, 3, 24, 20, 16, 12, 8, 4};

static void ModifyPersonalityForNature(u32 *personality, s32 newNature)
{
    s32 nature = GetNatureFromPersonality(*personality);
    s32 diff = abs(newNature - nature);
    s32 sign = (newNature > nature) ? 1 : -1;
    if (diff > NUM_NATURES / 2)
    {
        diff = NUM_NATURES - diff;
        sign *= -1;
    }
    *personality += (sModuloLUT[diff] * 0x100 * sign);
}

static bool32 SetCorrectAbilityNum(struct Pokemon *mon, enum Species species, enum Ability ability)
{
    const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[species];
    u32 abilityNum;
    u32 maxAbilityNum = ARRAY_COUNT(speciesInfo->abilities);
    // Old trainer exports can name abilities removed by a species rebalance.
    // Reject them before indexing ability data, then use the normal legal
    // ability selection below in GenerateMonFromTrainerMon.
    if (ability == ABILITY_NONE || ability >= ABILITIES_COUNT)
        return FALSE;
    for (abilityNum = 0; abilityNum < maxAbilityNum; ++abilityNum)
    {
        if (speciesInfo->abilities[abilityNum] == ability)
            break;
    }
    if (abilityNum == maxAbilityNum)
        return FALSE;
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
    return TRUE;
}

void MakeTrainerGenerator(struct TrainerGenerator *trainerGen, const struct Trainer *trainer)
{
    trainerGen->fairLoadout = UsesFairLoadout(trainer);
    trainerGen->gender = trainer->gender;
    if (trainer->aiFlags & AI_FLAG_SMART_TERA)
        trainerGen->smartTera = TRUE;
    trainerGen->isFrontier = FALSE;
    StringCopyN(trainerGen->name, trainer->trainerName, TRAINER_NAME_LENGTH + 1);
    trainerGen->trainerClass = trainer->trainerClass;
    trainerGen->otID = OTID_STRUCT_RANDOM_NO_SHINY;
    trainerGen->localRngState = GeneratePartySeed(trainer);
}

void MakePartnerGenerator(struct TrainerGenerator *trainerGen, const struct Trainer *partner)
{
    u32 otID;
    trainerGen->fairLoadout = FALSE;
    trainerGen->gender = partner->gender;
    if (partner->aiFlags & AI_FLAG_SMART_TERA)
        trainerGen->smartTera = TRUE;
    trainerGen->isFrontier = FALSE;
    StringCopyN(trainerGen->name, partner->trainerName, TRAINER_NAME_LENGTH + 1);
    trainerGen->trainerClass = partner->trainerClass;
    otID = Crc32B((const u8 *)partner, sizeof(struct Trainer));
    trainerGen->otID = OTID_STRUCT_PRESET(otID);
    trainerGen->localRngState = LocalRandomSeed(otID);
}

// Apply difficulty to the generated opponent, leaving authored teams untouched.
static void ApplyBriskTrainerTraining(struct Pokemon *mon)
{
    u32 mode = GetBriskDifficulty();
    if (mode == BRISK_MODE_NORMAL)
        return;

    u32 ivs = mode == BRISK_MODE_EASY ? 0 : 0x3FFFFFFF; // Six 5-bit IVs.
    SetMonData(mon, MON_DATA_IVS, &ivs);

    u8 evs[NUM_STATS] = {0};
    if (mode == BRISK_MODE_EXTREME || mode == BRISK_MODE_HARD)
    {
        enum Species species = GetMonData(mon, MON_DATA_SPECIES);
        u16 scores[NUM_STATS];
        for (u32 stat = 0; stat < NUM_STATS; stat++)
            scores[stat] = GetSpeciesBaseStat(species, stat);
        enum Ability ability = GetMonAbility(mon);
        if (ability == ABILITY_HUGE_POWER || ability == ABILITY_PURE_POWER) scores[STAT_ATK] *= 2;
        if (ability == ABILITY_HUSTLE) scores[STAT_ATK] = scores[STAT_ATK] * 3 / 2;
        if (ability == ABILITY_FUR_COAT) scores[STAT_DEF] *= 2;
        if (ability == ABILITY_ICE_SCALES) scores[STAT_SPDEF] *= 2;
        bool32 physical = FALSE, special = FALSE, slow = FALSE;
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
        {
            enum Move move = GetMonData(mon, MON_DATA_MOVE1 + i);
            if (move == MOVE_TRICK_ROOM || move == MOVE_GYRO_BALL)
                slow = TRUE;
            if (move == MOVE_NONE || GetMovePower(move) <= 1)
                continue;
            if (GetMoveCategory(move) == DAMAGE_CATEGORY_PHYSICAL && move != MOVE_BODY_PRESS && move != MOVE_FOUL_PLAY)
                physical = TRUE;
            if (GetMoveCategory(move) == DAMAGE_CATEGORY_SPECIAL)
                special = TRUE;
        }
        if (!physical) scores[STAT_ATK] = 0;
        if (!special) scores[STAT_SPATK] = 0;
        if (slow) scores[STAT_SPEED] = 0;
        if (GetSpeciesBaseHP(species) == 1) scores[STAT_HP] = 0;
        // Stable tie order; no random rolls and no EVs above 252 or 510 total.
        u32 level = GetMonData(mon, MON_DATA_LEVEL);
        u32 budget = mode == BRISK_MODE_EXTREME ? 510 : level < 20 ? 128 : level < 35 ? 256 : level < 50 ? 384 : 510;
        if (mode == BRISK_MODE_HARD)
        {
            u32 authoredTotal = 0;
            for (u32 stat = 0; stat < NUM_STATS; stat++)
                authoredTotal += GetMonData(mon, MON_DATA_HP_EV + stat);
            budget = min(510, max(budget, authoredTotal));
        }
        const u8 amounts[] = {min(252, budget / 2), min(252, budget / 2), budget > 504 ? 6 : 0};
        for (u32 i = 0; i < ARRAY_COUNT(amounts); i++)
        {
            u32 best = 0;
            for (u32 stat = 1; stat < NUM_STATS; stat++)
                if (scores[stat] > scores[best])
                    best = stat;
            if (scores[best] == 0)
                break;
            evs[best] = amounts[i];
            scores[best] = 0;
        }
    }
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        SetMonData(mon, MON_DATA_HP_EV + stat, &evs[stat]);
}

void GenerateMonFromTrainerMon(struct Pokemon *mon, const struct TrainerMon *trainerMon, struct TrainerGenerator *trainer)
{
    struct TrainerMon adjusted = *trainerMon;
    if (!trainer->isFrontier && trainer->otID.method == OT_ID_RANDOM_NO_SHINY)
    {
        u32 category = BriskIsImportantTrainer(trainer->trainerClass) ? BRISK_RAND_IMPORTANT : BRISK_RAND_TRAINERS;
        adjusted.lvl = ScaleBriskTrainerLevel(adjusted.lvl, adjusted.species ^ Crc32B((const u8 *)&trainer->localRngState, sizeof(trainer->localRngState)));
        adjusted.heldItem = BriskRandomItem(adjusted.heldItem);
        enum Species species = BriskRandomSpecies(adjusted.species, category);
        if (species != adjusted.species || BriskRandomizerEnabled(BRISK_RAND_ALL))
        {
            adjusted.species = species;
            adjusted.gender = TRAINER_MON_RANDOM_GENDER;
            adjusted.ability = ABILITY_NONE;
            adjusted.nickname = NULL;
            for (u32 i = 0; i < MAX_MON_MOVES; i++)
                adjusted.moves[i] = MOVE_NONE;
            adjusted.gigantamaxFactor = FALSE;
        }
    }
    trainerMon = &adjusted;
    u32 data;
    u32 personality = (LocalRandom32(&trainer->localRngState) & 0xFFFFDF00) + 0x1000;
    u32 genderValue = 0;
    if (trainerMon->gender == TRAINER_MON_RANDOM_GENDER)
        genderValue = LocalRandom32(&trainer->localRngState) & 0x000000FF;
    else if (trainerMon->gender == TRAINER_MON_MALE)
        genderValue = GeneratePersonalityForGender(MON_MALE, trainerMon->species);
    else if (trainerMon->gender == TRAINER_MON_FEMALE)
        genderValue = GeneratePersonalityForGender(MON_FEMALE, trainerMon->species);
    else
        errorf("Unkwown trainer mon gender value %d", trainerMon->gender);
    personality |= genderValue;
    ModifyPersonalityForNature(&personality, trainerMon->nature);
    CreateMon(mon, trainerMon->species, trainerMon->lvl, personality, trainer->otID);
    if (trainerMon->nickname != NULL)
        SetMonData(mon, MON_DATA_NICKNAME, trainerMon->nickname);
    if (trainerMon->ev) //ev in struct TrainerMon are stored in Showdown order not vanilla Emerald order
    {
        SetMonData(mon, MON_DATA_HP_EV, &trainerMon->ev[0]);
        SetMonData(mon, MON_DATA_ATK_EV, &trainerMon->ev[1]);
        SetMonData(mon, MON_DATA_DEF_EV, &trainerMon->ev[2]);
        SetMonData(mon, MON_DATA_SPATK_EV, &trainerMon->ev[3]);
        SetMonData(mon, MON_DATA_SPDEF_EV, &trainerMon->ev[4]);
        SetMonData(mon, MON_DATA_SPEED_EV, &trainerMon->ev[5]);
    }

    SetMonData(mon, MON_DATA_IVS, &trainerMon->iv);
    CustomTrainerPartyAssignMoves(mon, trainerMon);
    SetMonData(mon, MON_DATA_HELD_ITEM, &trainerMon->heldItem);

    bool32 abilitySet = FALSE;
    if (trainerMon->ability)
    {
        abilitySet = SetCorrectAbilityNum(mon, trainerMon->species, trainerMon->ability);
    }

    if (!abilitySet)
    {
        if (B_TRAINER_MON_RANDOM_ABILITY == 2)
        {
            do {
                data = Random() % NUM_ABILITY_SLOTS; // includes hidden abilities
            } while (GetAbilityBySpecies(trainerMon->species, data) == ABILITY_NONE);
            SetMonData(mon, MON_DATA_ABILITY_NUM, &data);
        }
        else if (B_TRAINER_MON_RANDOM_ABILITY == 0)
        {
            data = 0;
            SetMonData(mon, MON_DATA_ABILITY_NUM, &data);
        }
        // else B_TRAINER_MON_RANDOM_ABILITY == 1
        // this is the default from CreateMon (random non-hidden ability based on personality)
    }

    if (trainerMon->ball < POKEBALL_COUNT)
    {
        data = trainerMon->ball;
        SetMonData(mon, MON_DATA_POKEBALL, &data);
    }
    else if (B_TRAINER_CLASS_POKE_BALLS >= GEN_7 && trainer->trainerClass && trainerMon->ball == POKEBALL_COUNT)
    {
        data = gTrainerClasses[trainer->trainerClass].ball ?: BALL_POKE;
        SetMonData(mon, MON_DATA_POKEBALL, &data);
    }
    else if (trainerMon->ball > POKEBALL_COUNT)
    {
        errorf("Invalid ball for %S in %S's party", GetMonData(mon, MON_DATA_NICKNAME), trainer->name);
    }

    SetMonData(mon, MON_DATA_FRIENDSHIP, &trainerMon->friendship);

    data = trainerMon->isShiny;
    SetMonData(mon, MON_DATA_IS_SHINY, &data);
    if (trainerMon->shouldUseDynamax)
    {
        data = trainerMon->dynamaxLevel;
    }
    else
    {
        data = BLOCK_AI_DYNAMAX;
    }
    SetMonData(mon, MON_DATA_DYNAMAX_LEVEL, &data);
    if (trainerMon->gigantamaxFactor)
    {
        data = trainerMon->gigantamaxFactor;
        SetMonData(mon, MON_DATA_GIGANTAMAX_FACTOR, &data);
    }
    if (trainerMon->teraType)
    {
        data = trainerMon->teraType;
        SetMonData(mon, MON_DATA_TERA_TYPE, &data);
    }
    else if (!trainer->smartTera)
    {
        data = TYPE_MYSTERY;
        SetMonData(mon, MON_DATA_TERA_TYPE, &data);
    }

    if (!trainer->isFrontier && trainer->otID.method == OT_ID_RANDOM_NO_SHINY)
    {
        if (trainer->fairLoadout)
            ApplyBriskFairTrainerLoadout(mon);
        else
            ApplyBriskTrainerLoadout(mon);
        ApplyBriskTrainerTraining(mon);
    }

    CalculateMonStats(mon);
    SetMonData(mon, MON_DATA_OT_NAME, trainer->name);
    data = trainer->gender;
    SetMonData(mon, MON_DATA_OT_GENDER, &data);
}

#if TESTING
void Test_ApplyBriskTrainerTraining(struct Pokemon *mon)
{
    ApplyBriskTrainerTraining(mon);
}
#endif
