// Battle-side loader for 20 independently authored Xiphos commander parties.
extern const struct TrainerMon sXiphosFairyNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosFairyEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosFairyHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosFairyKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosFairyExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonExpert[PARTY_SIZE];

static bool32 BriskLoadXiphosCommanderParty(struct Pokemon *party, u16 trainerNum)
{
    u32 id = trainerNum - TRAINER_XIPHOS_COMMANDER_FAIRY;
    u32 mode = GetBriskDifficulty();
    if (id >= 4 || mode > BRISK_MODE_MAX) return FALSE;
    static const struct TrainerMon *const roster[4][5] = {
        {sXiphosFairyNormal, sXiphosFairyEasy, sXiphosFairyHard, sXiphosFairyKaizo, sXiphosFairyExpert},
        {sXiphosFightingNormal, sXiphosFightingEasy, sXiphosFightingHard, sXiphosFightingKaizo, sXiphosFightingExpert},
        {sXiphosSteelNormal, sXiphosSteelEasy, sXiphosSteelHard, sXiphosSteelKaizo, sXiphosSteelExpert},
        {sXiphosDragonNormal, sXiphosDragonEasy, sXiphosDragonHard, sXiphosDragonKaizo, sXiphosDragonExpert},
    };
    struct Trainer trainer = gTrainers[DIFFICULTY_NORMAL][trainerNum];
    trainer.party = roster[id][mode];
    trainer.partySize = PARTY_SIZE;
    trainer.authoredParty = TRUE;
    CreateNPCTrainerPartyFromTrainer(party, &trainer);
    return TRUE;
}
