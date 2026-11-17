// Rivals are rendered using existing Kanto trainer IDs and saved defeat bits.
// These are optional, single battles, independent of the FRLG story and Hoenn saves.
extern const struct TrainerMon sKantoRival_RIVAL_ROUTE22[PARTY_SIZE], sKantoRival_RIVAL_SS_ANNE[PARTY_SIZE];
extern const struct TrainerMon sKantoRival_RIVAL_ROUTE22_LATE[PARTY_SIZE], sKantoRival_RIVAL_SILPH[PARTY_SIZE], sKantoRival_RIVAL_CHAMPION[PARTY_SIZE];
extern const struct TrainerMon sKantoRival_RIVAL_CHAMPION_REMATCH[PARTY_SIZE];
extern const struct TrainerMon sKantoRival_WALLY_CERULEAN[PARTY_SIZE], sKantoRival_WALLY_LAVENDER[PARTY_SIZE], sKantoRival_WALLY_VICTORY[PARTY_SIZE];
extern const struct TrainerMon sKantoRival_BLAZIKEN, sKantoRival_SWAMPERT;

static bool32 BriskLoadKantoRivalParty(struct Pokemon *party, u16 trainerNum)
{
    const struct TrainerMon *source = NULL;
    struct TrainerMon mons[PARTY_SIZE];
    struct Trainer trainer;
    u32 mode = GetBriskDifficulty();
    u8 level;
    u8 bonus = 0;
    u32 i;
    bool32 finalRound = FALSE, isWally = FALSE;
    switch (trainerNum)
    {
    case TRAINER_KANTO_MAY_ROUTE22:
    case TRAINER_KANTO_BRENDAN_ROUTE22: source = sKantoRival_RIVAL_ROUTE22; break;
    case TRAINER_KANTO_MAY_SS_ANNE:
    case TRAINER_KANTO_BRENDAN_SS_ANNE: source = sKantoRival_RIVAL_SS_ANNE; bonus = 2; break;
    case TRAINER_KANTO_MAY_SILPH:
    case TRAINER_KANTO_BRENDAN_SILPH: source = sKantoRival_RIVAL_SILPH; bonus = 4; break;
    case TRAINER_KANTO_MAY_ROUTE22_LATE:
    case TRAINER_KANTO_BRENDAN_ROUTE22_LATE: source = sKantoRival_RIVAL_ROUTE22_LATE; bonus = 5; break;
    case TRAINER_KANTO_MAY_CHAMPION:
    case TRAINER_KANTO_BRENDAN_CHAMPION: source = sKantoRival_RIVAL_CHAMPION; finalRound = TRUE; bonus = 6; break;
    case TRAINER_KANTO_MAY_CHAMPION_REMATCH:
    case TRAINER_KANTO_BRENDAN_CHAMPION_REMATCH: source = sKantoRival_RIVAL_CHAMPION_REMATCH; finalRound = TRUE; bonus = 6; break;
    case TRAINER_KANTO_WALLY_CERULEAN: source = sKantoRival_WALLY_CERULEAN; isWally = TRUE; bonus = 1; break;
    case TRAINER_KANTO_WALLY_LAVENDER: source = sKantoRival_WALLY_LAVENDER; isWally = TRUE; bonus = 3; break;
    case TRAINER_KANTO_WALLY_VICTORY_ROAD: source = sKantoRival_WALLY_VICTORY; isWally = TRUE; finalRound = TRUE; bonus = 5; break;
    default: return FALSE;
    }
    // Mode IDs are Normal=0 Easy=1 Hard=2 Kaizo=3 Expert=4.
    static const u8 levels[] = {94, 90, 97, 100, 99};
    if (mode >= ARRAY_COUNT(levels))
        mode = BRISK_MODE_NORMAL;
    level = min(100, levels[mode] + bonus);
    memcpy(mons, source, sizeof(mons));
    for (i = 0; i < PARTY_SIZE; i++)
    {
        mons[i].lvl = level;
        if (mode == BRISK_MODE_EASY)
            mons[i].iv = 831283992;
    }
    if (!isWally)
    {
        // Opponent always carries the fully evolved Hoenn starter that beats the player's starter.
        // 0 Treecko -> Blaziken; 1 Torchic -> Swampert; 2 Mudkip -> Sceptile.
        if (VarGet(VAR_STARTER_MON) == 0)
            mons[5] = sKantoRival_BLAZIKEN;
        else if (VarGet(VAR_STARTER_MON) == 1)
            mons[5] = sKantoRival_SWAMPERT;
        // sKantoRival_*_RIVAL arrays carry Sceptile by default.
        mons[5].lvl = level;
        if (mode == BRISK_MODE_EASY)
            mons[5].iv = 831283992;
        if (!finalRound)
            mons[5].heldItem = ITEM_LIFE_ORB;
    }
    trainer = *GetTrainerStructFromId(trainerNum);
    trainer.party = mons;
    trainer.partySize = PARTY_SIZE;
    trainer.trainerClass = finalRound && !isWally ? TRAINER_CLASS_CHAMPION : TRAINER_CLASS_RIVAL;
    trainer.trainerPic = GetTrainerPicFromId(trainerNum);
    trainer.gender = trainer.trainerPic == TRAINER_PIC_MAY ? TRAINER_GENDER_FEMALE : TRAINER_GENDER_MALE;
    StringCopy(trainer.trainerName, GetTrainerNameFromId(trainerNum));
    if (mode != BRISK_MODE_EASY)
        trainer.aiFlags |= AI_FLAG_BASIC_TRAINER | AI_FLAG_SMART_SWITCHING | AI_FLAG_HP_AWARE | AI_FLAG_TRY_TO_2HKO;
    trainer.authoredParty = TRUE;
    CreateNPCTrainerPartyFromTrainer(party, &trainer);
    return TRUE;
}
