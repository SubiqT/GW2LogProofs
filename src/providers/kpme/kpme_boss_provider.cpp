#include "kpme_boss_provider.h"
#include "../../core/bosses.h"

std::vector<BossGroup> KpmeBossProvider::GetBossGroups() const {
	return {
			{"Summary", "kpmeSummaryTable", BossCategory::SUMMARY, {}, {"Legendary Insight", "Unstable Fractal Essence", "Boneskinner Ritual Vial"}},
			{"Raid Tokens", "kpmeRaidsTable", BossCategory::RAID_NORMAL, {ValeGuardian, Gorseval, Sabetha, Slothasor, Matthias, Escort, KeepConstruct, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura}},
			{"Raid CM Coffers", "kpmeRaidCMsTable", BossCategory::RAID_CM, {GreerCM, DecimaCM, UraCM}},
			{"Strike Coffers", "kpmeStrikesTable", BossCategory::STRIKE_NORMAL, {CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus}},
			{"Strike CM Coffers", "kpmeStrikeCMsTable", BossCategory::STRIKE_CM, {CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM}}
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

std::vector<ProofOption> KpmeBossProvider::GetAvailableProofs() const {
	std::vector<ProofOption> proofs;
	
	// Summary currencies
	proofs.push_back({"Legendary Insight", "Legendary Insight", "Summary", "Currency"});
	proofs.push_back({"Unstable Fractal Essence", "Unstable Fractal Essence", "Summary", "Currency"});
	proofs.push_back({"Boneskinner Ritual Vial", "Boneskinner Ritual Vial", "Summary", "Currency"});
	
	// Raid tokens
	for (const auto& boss : {ValeGuardian, Gorseval, Sabetha, Slothasor, Matthias, Escort, KeepConstruct, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura}) {
		proofs.push_back({GetKpMeBossToken(boss), GetBossName(boss), "Raid", "Token"});
	}
	
	// Raid CM coffers
	for (const auto& boss : {GreerCM, DecimaCM, UraCM}) {
		proofs.push_back({GetKpMeBossCoffer(boss), GetBossName(boss), "Raid", "CM Coffer"});
	}
	
	// Strike coffers
	for (const auto& boss : {CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus}) {
		proofs.push_back({GetKpMeBossCoffer(boss), GetBossName(boss), "Strike", "Coffer"});
	}
	
	// Strike CM coffers
	for (const auto& boss : {CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM}) {
		proofs.push_back({GetKpMeBossCoffer(boss), GetBossName(boss), "Strike", "CM Coffer"});
	}
	
	return proofs;
}

BossGroup KpmeBossProvider::CreateCustomBossGroup(const CustomTab& tab) const {
	// Validate input
	if (tab.id.empty() || tab.displayName.empty()) {
		return BossGroup{"", "", BossCategory::SUMMARY, {}, {}};
	}
	
	std::vector<Boss> bosses;
	std::vector<std::string> currencies;
	BossCategory category = BossCategory::SUMMARY; // Default to summary for mixed content
	
	// Separate currencies and bosses
	for (const auto& proofId : tab.proofIds) {
		if (proofId.empty()) continue; // Skip empty proof IDs
		
		// Check if it's a currency
		if (proofId == "Legendary Insight" || proofId == "Unstable Fractal Essence" || proofId == "Boneskinner Ritual Vial") {
			currencies.push_back(proofId);
		} else {
			// Find boss by proof identifier
			bool found = false;
			for (const auto& boss : {ValeGuardian, Gorseval, Sabetha, Slothasor, Matthias, Escort, KeepConstruct, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura}) {
				if (GetKpMeBossToken(boss) == proofId) {
					bosses.push_back(boss);
					found = true;
					break;
				}
			}
			if (!found) {
				for (const auto& boss : {GreerCM, DecimaCM, UraCM, CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus, CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM}) {
					if (GetKpMeBossCoffer(boss) == proofId) {
						bosses.push_back(boss);
						break;
					}
				}
			}
		}
	}
	
	// Return empty group if no valid content found
	if (bosses.empty() && currencies.empty()) {
		return BossGroup{"", "", BossCategory::SUMMARY, {}, {}};
	}
	
	// Always use SUMMARY category for custom tabs to handle mixed content
	category = BossCategory::SUMMARY;
	
	return BossGroup{tab.displayName, tab.id + "Table", category, bosses, currencies};
}