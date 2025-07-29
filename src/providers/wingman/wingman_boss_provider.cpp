#include "wingman_boss_provider.h"
#include "../../core/bosses.h"
#include <format>

std::vector<BossGroup> WingmanBossProvider::GetBossGroups() const {
    return {
        {"Raids", "normalRaidsTable", BossCategory::RAID_NORMAL, {
            ValeGuardian, Gorseval, Sabetha,
            Slothasor, BanditTrio, Matthias,
            Escort, KeepConstruct, TwistedCastle, Xera,
            Cairn, MursaatOverseer, Samarog, Deimos,
            SoullessHorror, RiverOfSouls, BrokenKing, EaterOfSouls, Eyes, Dhuum,
            ConjuredAmalgamate, TwinLargos, Qadim,
            Adina, Sabir, QadimThePeerless,
            Greer, Decima, Ura
        }},
        {"Raid CMs", "cmRaidsTable", BossCategory::RAID_CM, {
            KeepConstructCM,
            CairnCM, MursaatOverseerCM, SamarogCM, DeimosCM,
            SoullessHorrorCM, DhuumCM,
            ConjuredAmalgamateCM, TwinLargosCM, QadimCM,
            AdinaCM, SabirCM, QadimThePeerlessCM,
            GreerCM, DecimaCM, UraCM
        }},
        {"Raid LMs", "lmRaidsTable", BossCategory::RAID_LM, {UraCM}},
        {"Fractal CMs", "cmFractalsTable", BossCategory::FRACTAL_CM, {
            MAMACM, SiaxTheCorruptedCM, EnsolyssOfTheEndlessTormentCM,
            SkorvaldTheShatteredCM, ArtsariivCM, ArkkCM,
            ElementalAiKeeperOfThePeakCM, DarkAiKeeperOfThePeakCM, ElementalAndDarkAiKeeperOfThePeakCM,
            KanaxaiScytheOfHouseAurkusCM,
            EparchCM
        }},
        {"Strikes", "normalStrikesTable", BossCategory::STRIKE_NORMAL, {
            IcebroodConstruct, TheVoiceAndTheClaw, FraenirOfJormag, Boneskinner, WhisperOfJormag,
            Freezie,
            CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate,
            OldLionsCourt,
            Dagda, Cerus
        }},
        {"Strike CMs", "cmStrikesTable", BossCategory::STRIKE_CM, {
            CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM,
            OldLionsCourtCM,
            DagdaCM, CerusCM
        }},
        {"Strike LMs", "lmStrikesTable", BossCategory::STRIKE_LM, {CerusCM}}
    };
}

std::string WingmanBossProvider::GetProofIdentifier(Boss boss, BossCategory category) const {
    return (category == BossCategory::RAID_LM || category == BossCategory::STRIKE_LM) 
        ? std::format("-{}", int(boss)) 
        : std::format("{}", int(boss));
}

std::string WingmanBossProvider::GetProofIdentifier(const std::string& currency) const {
    return currency;
}