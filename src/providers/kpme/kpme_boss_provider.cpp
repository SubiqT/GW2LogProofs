#include "kpme_boss_provider.h"
#include "../../core/bosses.h"

std::vector<BossGroup> KpmeBossProvider::GetBossGroups() const {
    return {
        {"Summary", "kpmeSummaryTable", BossCategory::SUMMARY, {}, {
            "Legendary Insight",
            "Unstable Fractal Essence",
            "Boneskinner Ritual Vial"
        }},
        {"Raid Tokens", "kpmeRaidsTable", BossCategory::RAID_NORMAL, {
            ValeGuardian, Gorseval, Sabetha,
            Slothasor, Matthias,
            Escort, KeepConstruct, Xera,
            Cairn, MursaatOverseer, Samarog, Deimos,
            SoullessHorror, RiverOfSouls, BrokenKing, Dhuum,
            ConjuredAmalgamate, TwinLargos, Qadim,
            Adina, Sabir, QadimThePeerless,
            Greer, Decima, Ura
        }},
        {"Raid CM Coffers", "kpmeRaidCMsTable", BossCategory::RAID_CM, {
            GreerCM, DecimaCM, UraCM
        }},
        {"Strike Coffers", "kpmeStrikesTable", BossCategory::STRIKE_NORMAL, {
            CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate,
            OldLionsCourt,
            Dagda, Cerus
        }},
        {"Strike CM Coffers", "kpmeStrikeCMsTable", BossCategory::STRIKE_CM, {
            CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM,
            OldLionsCourtCM,
            DagdaCM, CerusCM
        }}
    };
}

std::string KpmeBossProvider::GetProofIdentifier(Boss boss, BossCategory category) const {
    switch (category) {
        case BossCategory::RAID_NORMAL:
            return GetKpMeBossToken(boss);
        case BossCategory::RAID_CM:
        case BossCategory::STRIKE_NORMAL:
        case BossCategory::STRIKE_CM:
            return GetKpMeBossCoffer(boss);
        default:
            return std::to_string(int(boss));
    }
}

std::string KpmeBossProvider::GetProofIdentifier(const std::string& currency) const {
    return currency;
}

std::string GetKpMeBossToken(Boss boss) {
    switch (boss) {
        case ValeGuardian: return "vale_guardian_token";
        case Gorseval: return "gorseval_token";
        case Sabetha: return "sabetha_token";
        case Slothasor: return "slothasor_token";
        case Matthias: return "matthias_token";
        case Escort: return "escort_token";
        case KeepConstruct: return "keep_construct_token";
        case Xera: return "xera_token";
        case Cairn: return "cairn_token";
        case MursaatOverseer: return "mursaat_overseer_token";
        case Samarog: return "samarog_token";
        case Deimos: return "deimos_token";
        case SoullessHorror: return "soulless_horror_token";
        case RiverOfSouls: return "river_of_souls_token";
        case BrokenKing: return "broken_king_token";
        case Dhuum: return "dhuum_token";
        case ConjuredAmalgamate: return "conjured_amalgamate_token";
        case TwinLargos: return "twin_largos_token";
        case Qadim: return "qadim_token";
        case Adina: return "adina_token";
        case Sabir: return "sabir_token";
        case QadimThePeerless: return "qadim_the_peerless_token";
        case Greer: return "greer_token";
        case Decima: return "decima_token";
        case Ura: return "ura_token";
        default: return "";
    }
}

std::string GetKpMeBossCoffer(Boss boss) {
    switch (boss) {
        case GreerCM: return "greer_coffer";
        case DecimaCM: return "decima_coffer";
        case UraCM: return "ura_coffer";
        case CaptainMaiTrin: return "captain_mai_trin_coffer";
        case Ankka: return "ankka_coffer";
        case MinisterLi: return "minister_li_coffer";
        case VoidAmalgamate: return "void_amalgamate_coffer";
        case OldLionsCourt: return "old_lions_court_coffer";
        case Dagda: return "dagda_coffer";
        case Cerus: return "cerus_coffer";
        case CaptainMaiTrinCM: return "captain_mai_trin_cm_coffer";
        case AnkkaCM: return "ankka_cm_coffer";
        case MinisterLiCM: return "minister_li_cm_coffer";
        case VoidAmalgamateCM: return "void_amalgamate_cm_coffer";
        case OldLionsCourtCM: return "old_lions_court_cm_coffer";
        case DagdaCM: return "dagda_cm_coffer";
        case CerusCM: return "cerus_cm_coffer";
        default: return "";
    }
}