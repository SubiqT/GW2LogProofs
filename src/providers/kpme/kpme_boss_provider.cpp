#include "kpme_boss_provider.h"
#include "../../core/bosses.h"

std::vector<BossGroup> KpmeBossProvider::GetBossGroups() const {
	return {
			{"Summary", "kpmeSummaryTable", BossCategory::SUMMARY, {}, {"Legendary Insight", "Unstable Fractal Essence", "Boneskinner Ritual Vial"}},
			{"Raid Tokens", "kpmeRaidsTable", BossCategory::RAID_NORMAL, {{ValeGuardian, BossType::NORMAL}, {Gorseval, BossType::NORMAL}, {Sabetha, BossType::NORMAL}, {Slothasor, BossType::NORMAL}, {Matthias, BossType::NORMAL}, {Escort, BossType::NORMAL}, {KeepConstruct, BossType::NORMAL}, {Xera, BossType::NORMAL}, {Cairn, BossType::NORMAL}, {MursaatOverseer, BossType::NORMAL}, {Samarog, BossType::NORMAL}, {Deimos, BossType::NORMAL}, {SoullessHorror, BossType::NORMAL}, {RiverOfSouls, BossType::NORMAL}, {BrokenKing, BossType::NORMAL}, {Dhuum, BossType::NORMAL}, {ConjuredAmalgamate, BossType::NORMAL}, {TwinLargos, BossType::NORMAL}, {Qadim, BossType::NORMAL}, {Adina, BossType::NORMAL}, {Sabir, BossType::NORMAL}, {QadimThePeerless, BossType::NORMAL}, {Greer, BossType::NORMAL}, {Decima, BossType::NORMAL}, {Ura, BossType::NORMAL}}},
			{"Raid CM Coffers", "kpmeRaidCMsTable", BossCategory::RAID_CM, {{GreerCM, BossType::CM}, {DecimaCM, BossType::CM}, {UraCM, BossType::CM}}},
			{"Strike Coffers", "kpmeStrikesTable", BossCategory::STRIKE_NORMAL, {{CaptainMaiTrin, BossType::NORMAL}, {Ankka, BossType::NORMAL}, {MinisterLi, BossType::NORMAL}, {VoidAmalgamate, BossType::NORMAL}, {OldLionsCourt, BossType::NORMAL}, {Dagda, BossType::NORMAL}, {Cerus, BossType::NORMAL}}},
			{"Strike CM Coffers", "kpmeStrikeCMsTable", BossCategory::STRIKE_CM, {{CaptainMaiTrinCM, BossType::CM}, {AnkkaCM, BossType::CM}, {MinisterLiCM, BossType::CM}, {VoidAmalgamateCM, BossType::CM}, {OldLionsCourtCM, BossType::CM}, {DagdaCM, BossType::CM}, {CerusCM, BossType::CM}}}
	};
}

std::string KpmeBossProvider::GetProofIdentifier(Boss boss, BossType type) const {
	return GetKpMeBossIdentifier(boss);
}

std::string KpmeBossProvider::GetProofIdentifier(const std::string& currency) const {
	return currency;
}

std::string GetKpMeBossIdentifier(Boss boss) {
	switch (boss) {
		// Raid tokens
		case ValeGuardian: return "Vale Guardian Fragment";
		case Gorseval: return "Gorseval Tentacle Piece";
		case Sabetha: return "Sabetha Flamethrower Fragment Piece";
		case Slothasor: return "Slothasor Mushroom";
		case Matthias: return "White Mantle Abomination Crystal";
		case Escort: return "Turret Fragment";
		case KeepConstruct: return "Keep Construct Rubble";
		case Xera: return "Ribbon Scrap";
		case Cairn: return "Cairn Fragment";
		case MursaatOverseer: return "Recreation Room Floor Fragment";
		case Samarog: return "Fragment of Saul's Burden";
		case Deimos: return "Impaled Prisoner Token";
		case SoullessHorror: return "Desmina's Token";
		case RiverOfSouls: return "River of Souls Token";
		case BrokenKing: return "Statue Token";
		case Dhuum: return "Dhuum's Token";
		case ConjuredAmalgamate: return "Conjured Amalgamate Token";
		case TwinLargos: return "Twin Largos Token";
		case Qadim: return "Qadim's Token";
		case Adina: return "Cardinal Adina's Token";
		case Sabir: return "Cardinal Sabir's Token";
		case QadimThePeerless: return "Ether Djinn's Token";
		case Greer: return "Greer's Token";
		case Decima: return "Decima's Token";
		case Ura: return "Ura's Token";
		// Coffers
		case GreerCM: return "Greer's Coffer";
		case DecimaCM: return "Decima's Coffer";
		case UraCM: return "Ura's Coffer";
		case CaptainMaiTrin: return "Mai Trin's Coffer";
		case Ankka: return "Ankka's Coffer";
		case MinisterLi: return "Minister Li's Coffer";
		case VoidAmalgamate: return "Void's Coffer";
		case OldLionsCourt: return "Assault Knights' Coffer";
		case Dagda: return "Dagda's Coffer";
		case Cerus: return "Cerus's Coffer";
		case CaptainMaiTrinCM: return "Mai Trin's Magnificent Coffer";
		case AnkkaCM: return "Ankka's Magnificent Coffer";
		case MinisterLiCM: return "Minister Li's Magnificent Coffer";
		case VoidAmalgamateCM: return "Void's Magnificent Coffer";
		case OldLionsCourtCM: return "Assault Knights' Magnificent Coffer";
		case DagdaCM: return "Dagda's Magnificent Coffer";
		case CerusCM: return "Cerus's Magnificent Coffer";
		default: return "";
	}
}

std::vector<ProofOption> KpmeBossProvider::GetAvailableProofs() const {
	std::vector<ProofOption> proofs;

	// Summary currencies
	proofs.push_back({"Legendary Insight", "Legendary Insight", "Summary", "Currency", "Normal"});
	proofs.push_back({"Unstable Fractal Essence", "Unstable Fractal Essence", "Summary", "Currency", "Normal"});
	proofs.push_back({"Boneskinner Ritual Vial", "Boneskinner Ritual Vial", "Summary", "Currency", "Normal"});

	// Raid tokens
	for (const auto& boss : {ValeGuardian, Gorseval, Sabetha, Slothasor, Matthias, Escort, KeepConstruct, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura}) {
		proofs.push_back({GetKpMeBossIdentifier(boss), GetBossName(boss, BossType::NORMAL, BossProofType::TOKEN), "Raid", "Token", "Normal"});
	}

	// Raid CM coffers
	for (const auto& boss : {GreerCM, DecimaCM, UraCM}) {
		proofs.push_back({GetKpMeBossIdentifier(boss), GetBossName(boss, BossType::CM, BossProofType::COFFER), "Raid", "CM Coffer", "CM"});
	}

	// Strike coffers
	for (const auto& boss : {CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus}) {
		proofs.push_back({GetKpMeBossIdentifier(boss), GetBossName(boss, BossType::NORMAL, BossProofType::COFFER), "Strike", "Coffer", "Normal"});
	}

	// Strike CM coffers
	for (const auto& boss : {CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM}) {
		proofs.push_back({GetKpMeBossIdentifier(boss), GetBossName(boss, BossType::CM, BossProofType::COFFER), "Strike", "CM Coffer", "CM"});
	}

	return proofs;
}

BossGroup KpmeBossProvider::CreateCustomBossGroup(const CustomTab& tab) const {
	// Validate input
	if (tab.id.empty() || tab.displayName.empty()) {
		return BossGroup {"", "", BossCategory::SUMMARY, {}, {}};
	}

	std::vector<BossEntry> bosses;
	std::vector<std::string> currencies;
	BossCategory category = BossCategory::SUMMARY; // Default to summary for mixed content

	// Separate currencies and bosses
	for (const auto& proof : tab.proofs) {
		if (proof.proofId.empty()) continue; // Skip empty proof IDs

		// Check if it's a currency
		if (proof.proofId == "Legendary Insight" || proof.proofId == "Unstable Fractal Essence" || proof.proofId == "Boneskinner Ritual Vial") {
			currencies.push_back(proof.proofId);
		} else {
			// Find boss by proof identifier
			bool found = false;
			BossType type = BossType::NORMAL;
			if (proof.bossType == "CM") type = BossType::CM;
			else if (proof.bossType == "LCM")
				type = BossType::LCM;

			for (const auto& boss : {ValeGuardian, Gorseval, Sabetha, Slothasor, Matthias, Escort, KeepConstruct, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura, GreerCM, DecimaCM, UraCM, CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus, CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM}) {
				if (GetKpMeBossIdentifier(boss) == proof.proofId) {
					bosses.push_back({boss, type});
					break;
				}
			}
		}
	}

	// Return empty group if no valid content found
	if (bosses.empty() && currencies.empty()) {
		return BossGroup {"", "", BossCategory::SUMMARY, {}, {}};
	}

	return BossGroup {tab.displayName, tab.id + "Table", category, bosses, currencies};
}