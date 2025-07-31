#include "wingman_boss_provider.h"
#include "../../core/bosses.h"
#include <format>

std::vector<BossGroup> WingmanBossProvider::GetBossGroups() const {
	return {
			{"Raids",       "normalRaidsTable",   BossCategory::RAID_NORMAL,   {ValeGuardian, Gorseval, Sabetha, Slothasor, BanditTrio, Matthias, Escort, KeepConstruct, TwistedCastle, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, EaterOfSouls, Eyes, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura}},
			{"Raid CMs",    "cmRaidsTable",       BossCategory::RAID_CM,       {KeepConstructCM, CairnCM, MursaatOverseerCM, SamarogCM, DeimosCM, SoullessHorrorCM, DhuumCM, ConjuredAmalgamateCM, TwinLargosCM, QadimCM, AdinaCM, SabirCM, QadimThePeerlessCM, GreerCM, DecimaCM, UraCM}                                                                                                             },
			{"Raid LMs",    "lmRaidsTable",       BossCategory::RAID_LM,       {UraCM}																																																																												},
			{"Fractal CMs", "cmFractalsTable",    BossCategory::FRACTAL_CM,    {MAMACM, SiaxTheCorruptedCM, EnsolyssOfTheEndlessTormentCM, SkorvaldTheShatteredCM, ArtsariivCM, ArkkCM, ElementalAiKeeperOfThePeakCM, DarkAiKeeperOfThePeakCM, ElementalAndDarkAiKeeperOfThePeakCM, KanaxaiScytheOfHouseAurkusCM, EparchCM}                                                                           },
			{"Strikes",     "normalStrikesTable", BossCategory::STRIKE_NORMAL, {IcebroodConstruct, TheVoiceAndTheClaw, FraenirOfJormag, Boneskinner, WhisperOfJormag, Freezie, CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus}                                                                                                                                        },
			{"Strike CMs",  "cmStrikesTable",     BossCategory::STRIKE_CM,     {CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM}																																																						 },
			{"Strike LMs",  "lmStrikesTable",     BossCategory::STRIKE_LM,     {CerusCM}																																																																											  }
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

std::vector<ProofOption> WingmanBossProvider::GetAvailableProofs() const {
	std::vector<ProofOption> proofs;
	
	// Raid Normal
	for (const auto& boss : {ValeGuardian, Gorseval, Sabetha, Slothasor, BanditTrio, Matthias, Escort, KeepConstruct, TwistedCastle, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, EaterOfSouls, Eyes, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura}) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss), "Raid", "Normal"});
	}
	
	// Raid CM
	for (const auto& boss : {KeepConstructCM, CairnCM, MursaatOverseerCM, SamarogCM, DeimosCM, SoullessHorrorCM, DhuumCM, ConjuredAmalgamateCM, TwinLargosCM, QadimCM, AdinaCM, SabirCM, QadimThePeerlessCM, GreerCM, DecimaCM, UraCM}) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss), "Raid", "CM"});
	}
	
	// Raid LM
	proofs.push_back({std::to_string(-int(UraCM)), GetBossName(UraCM), "Raid", "LM"});
	
	// Fractal CM
	for (const auto& boss : {MAMACM, SiaxTheCorruptedCM, EnsolyssOfTheEndlessTormentCM, SkorvaldTheShatteredCM, ArtsariivCM, ArkkCM, ElementalAiKeeperOfThePeakCM, DarkAiKeeperOfThePeakCM, ElementalAndDarkAiKeeperOfThePeakCM, KanaxaiScytheOfHouseAurkusCM, EparchCM}) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss), "Fractal", "CM"});
	}
	
	// Strike Normal
	for (const auto& boss : {IcebroodConstruct, TheVoiceAndTheClaw, FraenirOfJormag, Boneskinner, WhisperOfJormag, Freezie, CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus}) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss), "Strike", "Normal"});
	}
	
	// Strike CM
	for (const auto& boss : {CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM}) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss), "Strike", "CM"});
	}
	
	// Strike LM
	proofs.push_back({std::to_string(-int(CerusCM)), GetBossName(CerusCM), "Strike", "LM"});
	
	return proofs;
}

BossGroup WingmanBossProvider::CreateCustomBossGroup(const CustomTab& tab) const {
	std::vector<Boss> bosses;
	BossCategory category = BossCategory::RAID_NORMAL;
	
	for (const auto& proofId : tab.proofIds) {
		int bossId = std::stoi(proofId);
		Boss boss = static_cast<Boss>(bossId);
		bosses.push_back(boss);
	}
	
	// Use first boss to determine category
	if (!bosses.empty()) {
		int firstId = int(bosses[0]);
		if (firstId < 0) {
			Boss positiveBoss = static_cast<Boss>(-firstId);
			if (positiveBoss == UraCM) category = BossCategory::RAID_LM;
			else if (positiveBoss == CerusCM) category = BossCategory::STRIKE_LM;
			else if (firstId >= -22000) category = BossCategory::STRIKE_CM;
			else if (firstId >= -17000) category = BossCategory::FRACTAL_CM;
			else category = BossCategory::RAID_CM;
		} else {
			if (firstId >= 22000) category = BossCategory::STRIKE_NORMAL;
			else if (firstId >= 17000) category = BossCategory::FRACTAL_CM;
			else category = BossCategory::RAID_NORMAL;
		}
	}
	
	return BossGroup{tab.displayName, tab.id + "Table", category, bosses, {}};
}