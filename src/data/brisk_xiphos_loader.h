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

extern const struct TrainerMon sXiphosAgentANormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentAEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentAHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentAKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentAExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentENormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentEEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentEHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentEKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentEExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistANormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistAEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistAHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistAKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistAExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorANormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorAEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorAHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorAKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorAExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieExpert[PARTY_SIZE];

// All 16 Xiphos IDs have explicit five-mode rosters (mode IDs 0-4).
static bool32 BriskLoadXiphosCommanderParty(struct Pokemon *party, u16 trainerNum)
{
    u32 id;
    u32 mode = GetBriskDifficulty();
    if (trainerNum < TRAINER_XIPHOS_COMMANDER_FAIRY || trainerNum > TRAINER_XIPHOS_JESSIE || mode > BRISK_MODE_MAX)
        return FALSE;
    id = trainerNum - TRAINER_XIPHOS_COMMANDER_FAIRY;
    static const struct TrainerMon *const roster[16][5] = {
        {sXiphosFairyNormal, sXiphosFairyEasy, sXiphosFairyHard, sXiphosFairyKaizo, sXiphosFairyExpert},
        {sXiphosFightingNormal, sXiphosFightingEasy, sXiphosFightingHard, sXiphosFightingKaizo, sXiphosFightingExpert},
        {sXiphosSteelNormal, sXiphosSteelEasy, sXiphosSteelHard, sXiphosSteelKaizo, sXiphosSteelExpert},
        {sXiphosDragonNormal, sXiphosDragonEasy, sXiphosDragonHard, sXiphosDragonKaizo, sXiphosDragonExpert},
        {sXiphosAgentANormal, sXiphosAgentAEasy, sXiphosAgentAHard, sXiphosAgentAKaizo, sXiphosAgentAExpert},
        {sXiphosAgentBNormal, sXiphosAgentBEasy, sXiphosAgentBHard, sXiphosAgentBKaizo, sXiphosAgentBExpert},
        {sXiphosAgentCNormal, sXiphosAgentCEasy, sXiphosAgentCHard, sXiphosAgentCKaizo, sXiphosAgentCExpert},
        {sXiphosAgentDNormal, sXiphosAgentDEasy, sXiphosAgentDHard, sXiphosAgentDKaizo, sXiphosAgentDExpert},
        {sXiphosAgentENormal, sXiphosAgentEEasy, sXiphosAgentEHard, sXiphosAgentEKaizo, sXiphosAgentEExpert},
        {sXiphosAgentFNormal, sXiphosAgentFEasy, sXiphosAgentFHard, sXiphosAgentFKaizo, sXiphosAgentFExpert},
        {sXiphosAgentGNormal, sXiphosAgentGEasy, sXiphosAgentGHard, sXiphosAgentGKaizo, sXiphosAgentGExpert},
        {sXiphosScientistANormal, sXiphosScientistAEasy, sXiphosScientistAHard, sXiphosScientistAKaizo, sXiphosScientistAExpert},
        {sXiphosScientistBNormal, sXiphosScientistBEasy, sXiphosScientistBHard, sXiphosScientistBKaizo, sXiphosScientistBExpert},
        {sXiphosSeniorANormal, sXiphosSeniorAEasy, sXiphosSeniorAHard, sXiphosSeniorAKaizo, sXiphosSeniorAExpert},
        {sXiphosSeniorBNormal, sXiphosSeniorBEasy, sXiphosSeniorBHard, sXiphosSeniorBKaizo, sXiphosSeniorBExpert},
        {sXiphosJessieNormal, sXiphosJessieEasy, sXiphosJessieHard, sXiphosJessieKaizo, sXiphosJessieExpert},
    };
    struct Trainer trainer = gTrainers[DIFFICULTY_NORMAL][trainerNum];
    trainer.party = roster[id][mode];
    trainer.partySize = PARTY_SIZE;
    trainer.authoredParty = TRUE;
    CreateNPCTrainerPartyFromTrainer(party, &trainer);
    return TRUE;
}
