#include "wingman_boss_provider.h"
#include "../../core/bosses.h"
#include <format>

std::vector<BossGroup> WingmanBossProvider::GetBossGroups() const {
	return {
			{"Raids",       "normalRaidsTable",   BossCategory::RAID_NORMAL,   {{ValeGuardian, BossType::NORMAL}, {Gorseval, BossType::NORMAL}, {Sabetha, BossType::NORMAL}, {Slothasor, BossType::NORMAL}, {BanditTrio, BossType::NORMAL}, {Matthias, BossType::NORMAL}, {Escort, BossType::NORMAL}, {KeepConstruct, BossType::NORMAL}, {TwistedCastle, BossType::NORMAL}, {Xera, BossType::NORMAL}, {Cairn, BossType::NORMAL}, {MursaatOverseer, BossType::NORMAL}, {Samarog, BossType::NORMAL}, {Deimos, BossType::NORMAL}, {SoullessHorror, BossType::NORMAL}, {RiverOfSouls, BossType::NORMAL}, {BrokenKing, BossType::NORMAL}, {EaterOfSouls, BossType::NORMAL}, {Eyes, BossType::NORMAL}, {Dhuum, BossType::NORMAL}, {ConjuredAmalgamate, BossType::NORMAL}, {TwinLargos, BossType::NORMAL}, {Qadim, BossType::NORMAL}, {Adina, BossType::NORMAL}, {Sabir, BossType::NORMAL}, {QadimThePeerless, BossType::NORMAL}, {Greer, BossType::NORMAL}, {Decima, BossType::NORMAL}, {Ura, BossType::NORMAL}}},
			{"Raid CMs",    "cmRaidsTable",       BossCategory::RAID_CM,       {{KeepConstructCM, BossType::CM}, {CairnCM, BossType::CM}, {MursaatOverseerCM, BossType::CM}, {SamarogCM, BossType::CM}, {DeimosCM, BossType::CM}, {SoullessHorrorCM, BossType::CM}, {DhuumCM, BossType::CM}, {ConjuredAmalgamateCM, BossType::CM}, {TwinLargosCM, BossType::CM}, {QadimCM, BossType::CM}, {AdinaCM, BossType::CM}, {SabirCM, BossType::CM}, {QadimThePeerlessCM, BossType::CM}, {GreerCM, BossType::CM}, {DecimaCM, BossType::CM}, {UraCM, BossType::CM}}                                                                                                                                                                                                                                                                                                                                                                                                                                                 },
			{"Raid LMs",    "lmRaidsTable",       BossCategory::RAID_LM,       {{UraCM, BossType::LCM}}																																																																																																																																																																																																																								   },
			{"Fractal CMs", "cmFractalsTable",    BossCategory::FRACTAL_CM,    {{MAMACM, BossType::CM}, {SiaxTheCorruptedCM, BossType::CM}, {EnsolyssOfTheEndlessTormentCM, BossType::CM}, {SkorvaldTheShatteredCM, BossType::CM}, {ArtsariivCM, BossType::CM}, {ArkkCM, BossType::CM}, {ElementalAiKeeperOfThePeakCM, BossType::CM}, {DarkAiKeeperOfThePeakCM, BossType::CM}, {ElementalAndDarkAiKeeperOfThePeakCM, BossType::CM}, {KanaxaiScytheOfHouseAurkusCM, BossType::CM}, {EparchCM, BossType::CM}}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               },
			{"Strikes",     "normalStrikesTable", BossCategory::STRIKE_NORMAL, {{IcebroodConstruct, BossType::NORMAL}, {TheVoiceAndTheClaw, BossType::NORMAL}, {FraenirOfJormag, BossType::NORMAL}, {Boneskinner, BossType::NORMAL}, {WhisperOfJormag, BossType::NORMAL}, {Freezie, BossType::NORMAL}, {CaptainMaiTrin, BossType::NORMAL}, {Ankka, BossType::NORMAL}, {MinisterLi, BossType::NORMAL}, {VoidAmalgamate, BossType::NORMAL}, {OldLionsCourt, BossType::NORMAL}, {Dagda, BossType::NORMAL}, {Cerus, BossType::NORMAL}}                                                                                                                                                                                                                                                                                                                                                                                                                                                                        },
			{"Strike CMs",  "cmStrikesTable",     BossCategory::STRIKE_CM,     {{CaptainMaiTrinCM, BossType::CM}, {AnkkaCM, BossType::CM}, {MinisterLiCM, BossType::CM}, {VoidAmalgamateCM, BossType::CM}, {OldLionsCourtCM, BossType::CM}, {DagdaCM, BossType::CM}, {CerusCM, BossType::CM}}																																																																																																																																																																											 },
			{"Strike LMs",  "lmStrikesTable",     BossCategory::STRIKE_LM,     {{CerusCM, BossType::LCM}}																																																																																																																																																																																																																								 }
	};
}

std::string WingmanBossProvider::GetProofIdentifier(Boss boss, BossType type) const {
	return (type == BossType::LCM)
				   ? std::format("--{}", abs(int(boss)))
				   : std::format("{}", int(boss));
}

std::string WingmanBossProvider::GetProofIdentifier(const std::string& currency) const {
	return currency;
}

std::vector<ProofOption> WingmanBossProvider::GetAvailableProofs() const {
	static const std::vector<Boss> raidNormal = {ValeGuardian, Gorseval, Sabetha, Slothasor, BanditTrio, Matthias, Escort, KeepConstruct, TwistedCastle, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, EaterOfSouls, Eyes, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura};
	static const std::vector<Boss> raidCM = {KeepConstructCM, CairnCM, MursaatOverseerCM, SamarogCM, DeimosCM, SoullessHorrorCM, DhuumCM, ConjuredAmalgamateCM, TwinLargosCM, QadimCM, AdinaCM, SabirCM, QadimThePeerlessCM, GreerCM, DecimaCM, UraCM};
	static const std::vector<Boss> fractalCM = {MAMACM, SiaxTheCorruptedCM, EnsolyssOfTheEndlessTormentCM, SkorvaldTheShatteredCM, ArtsariivCM, ArkkCM, ElementalAiKeeperOfThePeakCM, DarkAiKeeperOfThePeakCM, ElementalAndDarkAiKeeperOfThePeakCM, KanaxaiScytheOfHouseAurkusCM, EparchCM};
	static const std::vector<Boss> strikeNormal = {IcebroodConstruct, TheVoiceAndTheClaw, FraenirOfJormag, Boneskinner, WhisperOfJormag, Freezie, CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus};
	static const std::vector<Boss> strikeCM = {CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM};

	std::vector<ProofOption> proofs;

	for (const auto& boss : raidNormal) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss), "Raid", "Normal", "Normal"});
	}
	for (const auto& boss : raidCM) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss, BossType::CM), "Raid", "CM", "CM"});
	}
	proofs.push_back({std::to_string(-int(UraCM)), GetBossName(UraCM, BossType::LCM), "Raid", "LM", "LCM"});

	for (const auto& boss : fractalCM) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss, BossType::CM), "Fractal", "CM", "CM"});
	}

	for (const auto& boss : strikeNormal) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss), "Strike", "Normal", "Normal"});
	}
	for (const auto& boss : strikeCM) {
		proofs.push_back({std::to_string(int(boss)), GetBossName(boss, BossType::CM), "Strike", "CM", "CM"});
	}
	proofs.push_back({std::to_string(-int(CerusCM)), GetBossName(CerusCM, BossType::LCM), "Strike", "LM", "LCM"});

	return proofs;
}

BossGroup WingmanBossProvider::CreateCustomBossGroup(const CustomTab& tab) const {
	std::vector<BossEntry> bosses;
	BossCategory category = BossCategory::SUMMARY;

	for (const auto& proof : tab.proofs) {
		int bossId = std::stoi(proof.proofId);
		Boss boss = static_cast<Boss>(abs(bossId)); // Handle negative IDs for LCM
		BossType type = BossType::NORMAL;
		if (proof.bossType == "CM") type = BossType::CM;
		else if (proof.bossType == "LCM" || bossId < 0)
			type = BossType::LCM;
		bosses.push_back({boss, type});
	}

	return BossGroup {tab.displayName, tab.id + "Table", category, bosses, {}};
}