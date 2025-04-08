#include "shared.h"
#include "bosses.h"
#include "resource.h"

std::vector<Boss> sortedRaidBosses {
	ValeGuardian, Gorseval, Sabetha,
	Slothasor, BanditTrio, Matthias,
	Escort, KeepConstruct, TwistedCastle, Xera,
	Cairn, MursaatOverseer, Samarog, Deimos,
	SoullessHorror, RiverOfSouls, BrokenKing, EaterOfSouls, Eyes, Dhuum,
	ConjuredAmalgamate, TwinLargos, Qadim,
	Adina, Sabir, QadimThePeerless,
	Greer, Decima, Ura
};
std::vector<Boss> sortedRaidCmBosses{
	KeepConstructCM,
	CairnCM, MursaatOverseerCM, SamarogCM, DeimosCM,
	SoullessHorrorCM, DhuumCM,
	ConjuredAmalgamateCM, TwinLargosCM, QadimCM,
	AdinaCM, SabirCM, QadimThePeerlessCM,
	GreerCM, DecimaCM, UraCM
};
std::vector<Boss> sortedRaidLmBosses{
	UraCM
};
std::vector<Boss> sortedFractalCMBosses {
	MAMACM, SiaxTheCorruptedCM, EnsolyssOfTheEndlessTormentCM,
	SkorvaldTheShatteredCM, ArtsariivCM, ArkkCM,
	ElementalAiKeeperOfThePeakCM, DarkAiKeeperOfThePeakCM, ElementalAndDarkAiKeeperOfThePeakCM,
	KanaxaiScytheOfHouseAurkusCM,
	// CerusAndDeimosCM, // Not tracked anymore by arc or wingman
	EparchCM
};
std::vector<Boss> sortedStrikeBosses {
	IcebroodConstruct, TheVoiceAndTheClaw, FraenirOfJormag, Boneskinner, WhisperOfJormag,
	Freezie,
	CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate,
	OldLionsCourt,
	Dagda, Cerus
};
std::vector<Boss> sortedStrikeCMBosses {
	CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM,
	OldLionsCourtCM,
	DagdaCM, CerusCM
};
std::vector<Boss> sortedStrikeLMBosses {
	CerusCM
};

std::vector<std::string> sortedKpmeSummary {
	"Legendary Insight",
	//"Legendary Divination", // Added to LI ( 1LD == 1LI )
	"Unstable Fractal Essence",
	//"Unstable Cosmic Essence", // Added to UFE ( 1UCE == 5UFE )
	"Boneskinner Ritual Vial"
};

std::vector<Boss> sortedKpmeRaidBosses {
	ValeGuardian, Gorseval, Sabetha,
	Slothasor, Matthias,
	Escort, KeepConstruct, Xera,
	Cairn, MursaatOverseer, Samarog, Deimos,
	SoullessHorror, RiverOfSouls, BrokenKing, Dhuum,
	ConjuredAmalgamate, TwinLargos, Qadim,
	Adina, Sabir, QadimThePeerless,
	Greer, Decima, Ura
};

std::vector<Boss> sortedKpmeRaidCMBosses{
	GreerCM, DecimaCM, UraCM
};

std::vector<Boss> sortedKpmeStrikeBosses {
	CaptainMaiTrin, Ankka, MinisterLi, VoidAmalgamate,
	OldLionsCourt,
	Dagda, Cerus
};
std::vector<Boss> sortedKpmeStrikeCMBosses {
	CaptainMaiTrinCM, AnkkaCM, MinisterLiCM, VoidAmalgamateCM,
	OldLionsCourtCM,
	DagdaCM, CerusCM
};

const char* GetBossName(Boss boss) {
	boss = Boss(abs(boss));
	switch (boss) {
	/* Raids */
	case ValeGuardian:
		return "Vale Guardian";
	case Gorseval:
		return "Gorseval";
	case Sabetha:
		return "Sabetha";
	case Slothasor:
		return "Slothasor";
	case BanditTrio:
		return "Bandit Trio";
	case Matthias:
		return "Matthias";
	case Escort:
		return "Escort";
	case KeepConstruct:
		return "Keep Construct";
	case TwistedCastle:
		return "Twisted Castle";
	case Xera:
		return "Xera";
	case Cairn:
		return "Cairn";
	case MursaatOverseer:
		return "Mursaat Overseer";
	case Samarog:
		return "Samarog";
	case Deimos:
		return "Deimos";
	case SoullessHorror:
		return "Soulles Horror";
	case RiverOfSouls:
		return "River of Souls";
	case BrokenKing:
		return "Broken King";
	case EaterOfSouls:
		return "Eater of Souls";
	case Eyes:
		return "Eyes";
	case Dhuum:
		return "Dhuum";
	case ConjuredAmalgamate:
		return "Conjured Amalgamate";
	case TwinLargos:
		return "Twin Largos";
	case Qadim:
		return "Qadim";
	case Adina:
		return "Adina";
	case Sabir:
		return "Sabir";
	case QadimThePeerless:
		return "Qadim the Peerless";
	case Greer:
		return "Greer";
	case Decima:
		return "Decima";
	case Ura:
		return "Ura";
	/* Fractals */
	case MAMA:
		return "MAMA";
	case SiaxTheCorrupted:
		return "Siax";
	case EnsolyssOfTheEndlessTorment:
		return "Ensolyss";
	case SkorvaldTheShattered:
		return "Skorvald";
	case Artsariiv:
		return "Artsariiv";
	case Arkk:
		return "Arkk";
	case ElementalAiKeeperOfThePeak:
		return "Elemental Ai";
	case DarkAiKeeperOfThePeak:
		return "Dark Ai";
	case ElementalAndDarkAiKeeperOfThePeak:
		return "Full Ai";
	case KanaxaiScytheOfHouseAurkus:
		return "Kanaxai";
	case CerusAndDeimos:
		return "Cerus & Deimos";
	case Eparch:
		return "Eparch";
	/* Strikes */
	case IcebroodConstruct:
		return "Icebrood Construct";
	case TheVoiceAndTheClaw:
		return "Kodans / The Voice and The Claw";
	case FraenirOfJormag:
		return "Fraenir of Jormag";
	case Boneskinner:
		return "Boneskinner";
	case WhisperOfJormag:
		return "Whisper of Jormag";
	case Freezie:
		return "Freezie";
	case CaptainMaiTrin:
		return "Captain Mai Trin";
	case Ankka:
		return "Ankka";
	case MinisterLi:
		return "Minister Li";
	case VoidAmalgamate:
		return "Harvest Temple / Void Amalgamate";
	case OldLionsCourt:
		return "Old Lion's Court";
	case Dagda:
		return "Dagda";
	case Cerus:
		return "Cerus";
	default:
		return "Unknown";
	}
}

Texture* GetBossTexture(Boss boss) {
	boss = Boss(abs(boss));
	switch (boss) {
	/* Raids */
	case ValeGuardian:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_VALE_GUARDIAN", IDB_VALE_GUARDIAN, hSelf);
	case Gorseval:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_GORSEVAL", IDB_GORSEVAL, hSelf);
	case Sabetha:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_SABETHA", IDB_SABETHA, hSelf);
	case Slothasor:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_SLOTHASOR", IDB_SLOTHASOR, hSelf);
	case BanditTrio:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_NARELLA", IDB_NARELLA, hSelf);
	case Matthias:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_MATTHIAS", IDB_MATTHIAS, hSelf);
	case Escort:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ESCORT", IDB_ESCORT, hSelf);
	case KeepConstruct:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_KEEP_CONSTRUCT", IDB_KEEP_CONSTRUCT, hSelf);
	case TwistedCastle:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_TWISTED_CASTLE", IDB_TWISTED_CASTLE, hSelf);
	case Xera:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_XERA", IDB_XERA, hSelf);
	case Cairn:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_CAIRN", IDB_CAIRN, hSelf);
	case MursaatOverseer:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_MURSAAT_OVERSEER", IDB_MURSAAT_OVERSEER, hSelf);
	case Samarog:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_SAMAROG", IDB_SAMAROG, hSelf);
	case Deimos:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_DEIMOS", IDB_DEIMOS, hSelf);
	case SoullessHorror:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_SOULLESS_HORROR", IDB_SOULLESS_HORROR, hSelf);
	case RiverOfSouls:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_RIVER_OF_SOULS", IDB_RIVER_OF_SOULS, hSelf);
	case BrokenKing:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_BROKEN_KING", IDB_BROKEN_KING, hSelf);
	case EaterOfSouls:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_EATER_OF_SOULS", IDB_EATER_OF_SOULS, hSelf);
	case Eyes:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_EYES_OF_FATE", IDB_EYES_OF_FATE, hSelf);
	case Dhuum:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_DHUUM", IDB_DHUUM, hSelf);
	case ConjuredAmalgamate:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_CONJURED_AMALGAMATE", IDB_CONJURED_AMALGAMATE, hSelf);
	case TwinLargos:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_KENUTT", IDB_KENUTT, hSelf);
	case Qadim:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_QADIM", IDB_QADIM, hSelf);
	case Adina:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ADINA", IDB_ADINA, hSelf);
	case Sabir:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_SABIR", IDB_SABIR, hSelf);
	case QadimThePeerless:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_QADIM_THE_PEERLESS", IDB_QADIM_THE_PEERLESS, hSelf);
	case Greer:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_GREER", IDB_GREER, hSelf);
	case Decima:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_DECIMA", IDB_DECIMA, hSelf);
	case Ura:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_URA", IDB_URA, hSelf);
	/* Fractals */
	case MAMA:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_MAMA", IDB_MAMA, hSelf);
	case SiaxTheCorrupted:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_SIAX", IDB_SIAX, hSelf);
	case EnsolyssOfTheEndlessTorment:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ENSOLYSS", IDB_ENSOLYSS, hSelf);
	case SkorvaldTheShattered:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_SKORVALD", IDB_SKORVALD, hSelf);
	case Artsariiv:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ARTSARIIV", IDB_ARTSARIIV, hSelf);
	case Arkk:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ARKK", IDB_ARKK, hSelf);
	case ElementalAiKeeperOfThePeak:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ELEMENTAL_AI_LIGHT", IDB_ELEMENTAL_AI_LIGHT, hSelf);
	case DarkAiKeeperOfThePeak:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ELEMENTAL_AI_DARK", IDB_ELEMENTAL_AI_DARK, hSelf);
	case ElementalAndDarkAiKeeperOfThePeak:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ELEMENTAL_AI_LIGHT_AND_DARK", IDB_ELEMENTAL_AI_LIGHT_AND_DARK, hSelf);
	case KanaxaiScytheOfHouseAurkus:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_KANAXAI", IDB_KANAXAI, hSelf);
	case CerusAndDeimos:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_CERUS_AND_DEIMOS", IDB_CERUS_AND_DEIMOS, hSelf);
	case Eparch:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_EPARCH", IDB_EPARCH, hSelf);
	/* Strikes */
	case IcebroodConstruct:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ICEBROOD_CONSTRUCT", IDB_ICEBROOD_CONSTRUCT, hSelf);
	case TheVoiceAndTheClaw:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_KODANS", IDB_KODANS, hSelf);
	case FraenirOfJormag:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_FRAENIR_OF_JORMAG", IDB_FRAENIR_OF_JORMAG, hSelf);
	case Boneskinner:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_BONESKINNER", IDB_BONESKINNER, hSelf);
	case WhisperOfJormag:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_WHISPER_OF_JORMAG", IDB_WHISPER_OF_JORMAG, hSelf);
	case Freezie:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_FREEZIE", IDB_FREEZIE, hSelf);
	case CaptainMaiTrin:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_CAPTAIN_MAI_TRIN", IDB_CAPTAIN_MAI_TRIN, hSelf);
	case Ankka:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_ANKKA", IDB_ANKKA, hSelf);
	case MinisterLi:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_MINISTER_LI", IDB_MINISTER_LI, hSelf);
	case VoidAmalgamate:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_HARVEST_TEMPLE", IDB_HARVEST_TEMPLE, hSelf);
	case OldLionsCourt:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_WATCHKNIGHT_MARK_II", IDB_WATCHKNIGHT_MARK_II, hSelf);
	case Dagda:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_DAGDA", IDB_DAGDA, hSelf);
	case Cerus:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_CERUS", IDB_CERUS, hSelf);
	default:
		return APIDefs->Textures.GetOrCreateFromResource("TEX_UNDER_CONSTRUCTION", IDB_UNDER_CONSTRUCTION, hSelf);
	}
}


std::string GetKpMeBossToken(Boss boss) {
	switch (boss) {
	/* Raids */
	case ValeGuardian:
		return "Vale Guardian Fragment";
	case Gorseval:
		return "Gorseval Tentacle Piece";
	case Sabetha:
		return "Sabetha Flamethrower Fragment Piece";
	case Slothasor:
		return "Slothasor Mushroom";
	case Matthias:
		return "White Mantle Abomination Crystal";
	case Escort:
		return "Turret Fragment";
	case KeepConstruct:
		return "Keep Construct Rubble";
	case Xera:
		return "Ribbon Scrap";
	case Cairn:
		return "Cairn Fragment";
	case MursaatOverseer:
		return "Recreation Room Floor Fragment";
	case Samarog:
		return "Impaled Prisoner Token";
	case Deimos:
		return "Fragment of Saul's Burden";
	case SoullessHorror:
		return "Desmina's Token";
	case RiverOfSouls:
		return "River of Souls Token";
	case BrokenKing:
		return "Statue Token";
	case Dhuum:
		return "Dhuum's Token";
	case ConjuredAmalgamate:
		return "Conjured Amalgamate Token";
	case TwinLargos:
		return "Twin Largos Token";
	case Qadim:
		return "Qadim's Token";
	case Adina:
		return "Cardinal Adina's Token";
	case Sabir:
		return "Cardinal Sabir's Token";
	case QadimThePeerless:
		return "Ether Djinn's Token";
	case Greer:
		return "Greer's Token";
	case Decima:
		return "Decima's Token";
	case Ura:
		return "Ura's Token";
	default:
		return "Unknown";
	}
}

std::string GetKpMeBossCoffer(Boss boss) {
	switch (boss) {
		/* Raids */
	case ValeGuardian:
		return "Vale Guardian Coffer";
	case Gorseval:
		return "Gorseval's Coffer";
	case Sabetha:
		return "Sabetha's Coffer";
	case Slothasor:
		return "Slothasor's Coffer";
	case Matthias:
		return "Matthias's Coffer";
	case Escort:
		return "McLeod's Coffer";
	case KeepConstruct:
		return "Keep Construct's Coffer";
	case Xera:
		return "Xera's Coffer";
	case Cairn:
		return "Cairn's Coffer";
	case MursaatOverseer:
		return "Mursaat Overseer's Coffer";
	case Samarog:
		return "Samarog's Coffer";
	case Deimos:
		return "Deimos's Coffer";
	case SoullessHorror:
		return "Desmina's Coffer";
	case RiverOfSouls:
		return "River of Souls Coffer";
	case BrokenKing:
		return "Statue of Grenth Coffer";
	case Dhuum:
		return "Dhuum's Coffer";
	case ConjuredAmalgamate:
		return "Conjured Amalgamate's Coffer";
	case TwinLargos:
		return "Twin Largos' Coffer";
	case Qadim:
		return "Qadim's Coffer";
	case Adina:
		return "Cardinal Adina's Coffer";
	case Sabir:
		return "Cardinal Sabir's Coffer";
	case QadimThePeerless:
		return "Qadim the Peerless's Coffer";
	case Greer:
		return "Greer's Coffer";
	case Decima:
		return "Decima's Coffer";
	case Ura:
		return "Ura's Coffer";
		/* Raid CMs */
	case GreerCM:
		return "Greer's Magnificent Coffer";
	case DecimaCM:
		return "Decima's Magnificent Coffer";
	case UraCM:
		return "Ura's Magnificent Coffer";
		/* Strikes */
	case CaptainMaiTrin:
		return "Mai Trin's Coffer";
	case Ankka:
		return "Ankka's Coffer";
	case MinisterLi:
		return 	"Minister Li's Coffer";
	case VoidAmalgamate:
		return "Void's Coffer";
	case OldLionsCourt:
		return "Assault Knights' Coffer";
	case Dagda:
		return "Dagda's Coffer";
	case Cerus:
		return "Cerus's Coffer";
		/* Strike CMs */
	case CaptainMaiTrinCM:
		return "Mai Trin's Magnificent Coffer";
	case AnkkaCM:
		return "Ankka's Magnificent Coffer";
	case MinisterLiCM:
		return 	"Minister Li's Magnificent Coffer";
	case VoidAmalgamateCM:
		return "Void's Magnificent Coffer";
	case OldLionsCourtCM:
		return "Assault Knights' Magnificent Coffer";
	case DagdaCM:
		return "Dagda's Magnificent Coffer";
	case CerusCM:
		return "Cerus's Magnificent Coffer";
	default:
		return "Unknown";
	}
}

Texture* GetCurrencyTexture(std::string name) {
	if (name == "Legendary Insight") return APIDefs->Textures.GetOrCreateFromResource("TEX_LEGENDARY_INSIGHT", IDB_LEGENDARY_INSIGHT, hSelf);
	if (name == "Legendary Divination") return APIDefs->Textures.GetOrCreateFromResource("TEX_LEGENDARY_DIVINATION", IDB_LEGENDARY_DIVINATION, hSelf);
	if (name == "Unstable Fractal Essence") return APIDefs->Textures.GetOrCreateFromResource("TEX_UNSTABLE_FRACTAL_ESSENCE", IDB_UNSTABLE_FRACTAL_ESSENCE, hSelf);
	if (name == "Boneskinner Ritual Vial") return APIDefs->Textures.GetOrCreateFromResource("TEX_BONESKINNER_RITUAL_VIAL", IDB_BONESKINNER_RITUAL_VIAL, hSelf);
	return APIDefs->Textures.GetOrCreateFromResource("TEX_UNDER_CONSTRUCTION", IDB_UNDER_CONSTRUCTION, hSelf);
}