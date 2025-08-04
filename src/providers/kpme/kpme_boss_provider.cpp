#include "kpme_boss_provider.h"
#include "../../core/bosses.h"
#include <unordered_map>
#include <unordered_set>


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
	// For KPME, normal raids use tokens, everything else uses coffers
	static const std::unordered_set<Boss> raidBosses = {
			ValeGuardian, Gorseval, Sabetha, Slothasor, Matthias, Escort, KeepConstruct, Xera,
			Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing,
			Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless,
			Greer, Decima, Ura
	};

	if (type == BossType::NORMAL && raidBosses.count(boss)) {
		return GetKpMeBossToken(boss);
	}
	return GetKpMeBossCoffer(boss);
}

std::string KpmeBossProvider::GetProofIdentifier(const std::string& currency) const {
	return currency;
}

std::string GetKpMeBossToken(Boss boss) {
	static const std::unordered_map<Boss, std::string> tokenMap = {
			{ValeGuardian,       "Vale Guardian Fragment"             },
			{Gorseval,           "Gorseval Tentacle Piece"            },
			{Sabetha,            "Sabetha Flamethrower Fragment Piece"},
			{Slothasor,          "Slothasor Mushroom"                 },
			{Matthias,           "White Mantle Abomination Crystal"   },
			{Escort,             "Turret Fragment"                    },
			{KeepConstruct,      "Keep Construct Rubble"              },
			{Xera,               "Ribbon Scrap"                       },
			{Cairn,              "Cairn Fragment"                     },
			{MursaatOverseer,    "Recreation Room Floor Fragment"     },
			{Samarog,            "Impaled Prisoner Token"             },
			{Deimos,             "Fragment of Saul's Burden"          },
			{SoullessHorror,     "Desmina's Token"                    },
			{RiverOfSouls,       "River of Souls Token"               },
			{BrokenKing,         "Statue Token"                       },
			{Dhuum,              "Dhuum's Token"                      },
			{ConjuredAmalgamate, "Conjured Amalgamate Token"          },
			{TwinLargos,         "Twin Largos Token"                  },
			{Qadim,              "Qadim's Token"                      },
			{Adina,              "Cardinal Adina's Token"             },
			{Sabir,              "Cardinal Sabir's Token"             },
			{QadimThePeerless,   "Ether Djinn's Token"                },
			{Greer,              "Greer's Token"                      },
			{Decima,             "Decima's Token"                     },
			{Ura,                "Ura's Token"                        }
	};
	auto it = tokenMap.find(boss);
	return it != tokenMap.end() ? it->second : "";
}

std::string GetKpMeBossCoffer(Boss boss) {
	static const std::unordered_map<Boss, std::string> cofferMap = {
			{ValeGuardian,       "Vale Guardian Coffer"               },
			{Gorseval,           "Gorseval's Coffer"                  },
			{Sabetha,            "Sabetha's Coffer"                   },
			{Slothasor,          "Slothasor's Coffer"                 },
			{Matthias,           "Matthias's Coffer"                  },
			{Escort,             "McLeod's Coffer"                    },
			{KeepConstruct,      "Keep Construct's Coffer"            },
			{Xera,               "Xera's Coffer"                      },
			{Cairn,              "Cairn's Coffer"                     },
			{MursaatOverseer,    "Mursaat Overseer's Coffer"          },
			{Samarog,            "Samarog's Coffer"                   },
			{Deimos,             "Deimos's Coffer"                    },
			{SoullessHorror,     "Desmina's Coffer"                   },
			{RiverOfSouls,       "River of Souls Coffer"              },
			{BrokenKing,         "Statue of Grenth Coffer"            },
			{Dhuum,              "Dhuum's Coffer"                     },
			{ConjuredAmalgamate, "Conjured Amalgamate's Coffer"       },
			{TwinLargos,         "Twin Largos' Coffer"                },
			{Qadim,              "Qadim's Coffer"                     },
			{Adina,              "Cardinal Adina's Coffer"            },
			{Sabir,              "Cardinal Sabir's Coffer"            },
			{QadimThePeerless,   "Qadim the Peerless's Coffer"        },
			{Greer,              "Greer's Coffer"                     },
			{Decima,             "Decima's Coffer"                    },
			{Ura,                "Ura's Coffer"                       },
			{GreerCM,            "Greer's Magnificent Coffer"         },
			{DecimaCM,           "Decima's Magnificent Coffer"        },
			{UraCM,              "Ura's Magnificent Coffer"           },
			{CaptainMaiTrin,     "Mai Trin's Coffer"                  },
			{Ankka,              "Ankka's Coffer"                     },
			{MinisterLi,         "Minister Li's Coffer"               },
			{VoidAmalgamate,     "Void's Coffer"                      },
			{OldLionsCourt,      "Assault Knights' Coffer"            },
			{Dagda,              "Dagda's Coffer"                     },
			{Cerus,              "Cerus's Coffer"                     },
			{CaptainMaiTrinCM,   "Mai Trin's Magnificent Coffer"      },
			{AnkkaCM,            "Ankka's Magnificent Coffer"         },
			{MinisterLiCM,       "Minister Li's Magnificent Coffer"   },
			{VoidAmalgamateCM,   "Void's Magnificent Coffer"          },
			{OldLionsCourtCM,    "Assault Knights' Magnificent Coffer"},
			{DagdaCM,            "Dagda's Magnificent Coffer"         },
			{CerusCM,            "Cerus's Magnificent Coffer"         }
	};
	auto it = cofferMap.find(boss);
	return it != cofferMap.end() ? it->second : "";
}

std::vector<ProofOption> KpmeBossProvider::GetAvailableProofs() const {
	std::vector<ProofOption> proofs;

	// Summary currencies
	proofs.push_back({"Legendary Insight", "Legendary Insight", "Summary", "Currency", "Normal"});
	proofs.push_back({"Unstable Fractal Essence", "Unstable Fractal Essence", "Summary", "Currency", "Normal"});
	proofs.push_back({"Boneskinner Ritual Vial", "Boneskinner Ritual Vial", "Summary", "Currency", "Normal"});

	// Raid tokens
	for (const auto& boss : {ValeGuardian, Gorseval, Sabetha, Slothasor, Matthias, Escort, KeepConstruct, Xera, Cairn, MursaatOverseer, Samarog, Deimos, SoullessHorror, RiverOfSouls, BrokenKing, Dhuum, ConjuredAmalgamate, TwinLargos, Qadim, Adina, Sabir, QadimThePeerless, Greer, Decima, Ura}) {
		proofs.push_back({GetKpMeBossToken(boss), GetBossName(boss, BossType::NORMAL, BossProofType::TOKEN), "Raid", "Token", "Normal"});
	}

	// Raid CM coffers
	for (const auto& boss : {GreerCM, DecimaCM, UraCM}) {
		proofs.push_back({GetKpMeBossCoffer(boss), GetBossName(boss, BossType::CM, BossProofType::COFFER), "Raid", "CM Coffer", "CM"});
	}

	// Strike coffers
	for (const auto& boss : {CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate, OldLionsCourt, Dagda, Cerus}) {
		proofs.push_back({GetKpMeBossCoffer(boss), GetBossName(boss, BossType::NORMAL, BossProofType::COFFER), "Strike", "Coffer", "Normal"});
	}

	// Strike CM coffers
	for (const auto& boss : {CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM, OldLionsCourtCM, DagdaCM, CerusCM}) {
		proofs.push_back({GetKpMeBossCoffer(boss), GetBossName(boss, BossType::CM, BossProofType::COFFER), "Strike", "CM Coffer", "CM"});
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
				if (GetKpMeBossToken(boss) == proof.proofId || GetKpMeBossCoffer(boss) == proof.proofId) {
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