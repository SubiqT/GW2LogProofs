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
		Adina, Sabir, QadimThePeerless
};
std::vector<Boss> sortedRaidCmBosses {
	KeepConstructCM,
		CairnCM, MursaatOverseerCM, SamarogCM, DeimosCM,
		SoullessHorrorCM, DhuumCM,
		ConjuredAmalgamateCM, TwinLargosCM, QadimCM,
		AdinaCM, SabirCM, QadimThePeerlessCM
};
std::vector<Boss> sortedFractalBosses {
	MAMA, SiaxTheCorrupted, EnsolyssOfTheEndlessTorment,
		SkorvaldTheShattered, Artsariiv, Arkk,
		ElementalAiKeeperOfThePeak,
		KanaxaiScytheOfHouseAurkus
};
std::vector<Boss> sortedFractalCMBosses {
	MAMACM, SiaxTheCorruptedCM, EnsolyssOfTheEndlessTormentCM,
		SkorvaldTheShatteredCM, ArtsariivCM, ArkkCM,
		ElementalAiKeeperOfThePeakCM, DarkAiKeeperOfThePeakCM, ElementalAndDarkAiKeeperOfThePeakCM,
		KanaxaiScytheOfHouseAurkusCM
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

const char* GetBossName(Boss boss) {
	boss = Boss(abs(boss));
	switch (boss) {
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
	case ValeGuardian:
		return APIDefs->GetTextureOrCreateFromResource("TEX_VALE_GUARDIAN", IDB_VALE_GUARDIAN, hSelf);
	case Gorseval:
		return APIDefs->GetTextureOrCreateFromResource("TEX_GORSEVAL", IDB_GORSEVAL, hSelf);
	case Sabetha:
		return APIDefs->GetTextureOrCreateFromResource("TEX_SABETHA", IDB_SABETHA, hSelf);
	case Slothasor:
		return APIDefs->GetTextureOrCreateFromResource("TEX_SLOTHASOR", IDB_SLOTHASOR, hSelf);
	case BanditTrio:
		return APIDefs->GetTextureOrCreateFromResource("TEX_NARELLA", IDB_NARELLA, hSelf);
	case Matthias:
		return APIDefs->GetTextureOrCreateFromResource("TEX_MATTHIAS", IDB_MATTHIAS, hSelf);
	case Escort:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ESCORT", IDB_ESCORT, hSelf);
	case KeepConstruct:
		return APIDefs->GetTextureOrCreateFromResource("TEX_KEEP_CONSTRUCT", IDB_KEEP_CONSTRUCT, hSelf);
	case TwistedCastle:
		return APIDefs->GetTextureOrCreateFromResource("TEX_TWISTED_CASTLE", IDB_TWISTED_CASTLE, hSelf);
	case Xera:
		return APIDefs->GetTextureOrCreateFromResource("TEX_XERA", IDB_XERA, hSelf);
	case Cairn:
		return APIDefs->GetTextureOrCreateFromResource("TEX_CAIRN", IDB_CAIRN, hSelf);
	case MursaatOverseer:
		return APIDefs->GetTextureOrCreateFromResource("TEX_MURSAAT_OVERSEER", IDB_MURSAAT_OVERSEER, hSelf);
	case Samarog:
		return APIDefs->GetTextureOrCreateFromResource("TEX_SAMAROG", IDB_SAMAROG, hSelf);
	case Deimos:
		return APIDefs->GetTextureOrCreateFromResource("TEX_DEIMOS", IDB_DEIMOS, hSelf);
	case SoullessHorror:
		return APIDefs->GetTextureOrCreateFromResource("TEX_SOULLESS_HORROR", IDB_SOULLESS_HORROR, hSelf);
	case RiverOfSouls:
		return APIDefs->GetTextureOrCreateFromResource("TEX_RIVER_OF_SOULS", IDB_RIVER_OF_SOULS, hSelf);
	case BrokenKing:
		return APIDefs->GetTextureOrCreateFromResource("TEX_BROKEN_KING", IDB_BROKEN_KING, hSelf);
	case EaterOfSouls:
		return APIDefs->GetTextureOrCreateFromResource("TEX_EATER_OF_SOULS", IDB_EATER_OF_SOULS, hSelf);
	case Eyes:
		return APIDefs->GetTextureOrCreateFromResource("TEX_EYES_OF_FATE", IDB_EYES_OF_FATE, hSelf);
	case Dhuum:
		return APIDefs->GetTextureOrCreateFromResource("TEX_DHUUM", IDB_DHUUM, hSelf);
	case ConjuredAmalgamate:
		return APIDefs->GetTextureOrCreateFromResource("TEX_CONJURED_AMALGAMATE", IDB_CONJURED_AMALGAMATE, hSelf);
	case TwinLargos:
		return APIDefs->GetTextureOrCreateFromResource("TEX_KENUTT", IDB_KENUTT, hSelf);
	case Qadim:
		return APIDefs->GetTextureOrCreateFromResource("TEX_QADIM", IDB_QADIM, hSelf);
	case Adina:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ADINA", IDB_ADINA, hSelf);
	case Sabir:
		return APIDefs->GetTextureOrCreateFromResource("TEX_SABIR", IDB_SABIR, hSelf);
	case QadimThePeerless:
		return APIDefs->GetTextureOrCreateFromResource("TEX_QADIM_THE_PEERLESS", IDB_QADIM_THE_PEERLESS, hSelf);
	case MAMA:
		return APIDefs->GetTextureOrCreateFromResource("TEX_MAMA", IDB_MAMA, hSelf);
	case SiaxTheCorrupted:
		return APIDefs->GetTextureOrCreateFromResource("TEX_SIAX", IDB_SIAX, hSelf);
	case EnsolyssOfTheEndlessTorment:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ENSOLYSS", IDB_ENSOLYSS, hSelf);
	case SkorvaldTheShattered:
		return APIDefs->GetTextureOrCreateFromResource("TEX_SKORVALD", IDB_SKORVALD, hSelf);
	case Artsariiv:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ARTSARIIV", IDB_ARTSARIIV, hSelf);
	case Arkk:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ARKK", IDB_ARKK, hSelf);
	case ElementalAiKeeperOfThePeak:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ELEMENTAL_AI_LIGHT", IDB_ELEMENTAL_AI_LIGHT, hSelf);
	case DarkAiKeeperOfThePeak:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ELEMENTAL_AI_DARK", IDB_ELEMENTAL_AI_DARK, hSelf);
	case ElementalAndDarkAiKeeperOfThePeak:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ELEMENTAL_AI_LIGHT_AND_DARK", IDB_ELEMENTAL_AI_LIGHT_AND_DARK, hSelf);
	case KanaxaiScytheOfHouseAurkus:
		return APIDefs->GetTextureOrCreateFromResource("TEX_KANAXAI", IDB_KANAXAI, hSelf);
	case IcebroodConstruct:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ICEBROOD_CONSTRUCT", IDB_ICEBROOD_CONSTRUCT, hSelf);
	case TheVoiceAndTheClaw:
		return APIDefs->GetTextureOrCreateFromResource("TEX_KODANS", IDB_KODANS, hSelf);
	case FraenirOfJormag:
		return APIDefs->GetTextureOrCreateFromResource("TEX_FRAENIR_OF_JORMAG", IDB_FRAENIR_OF_JORMAG, hSelf);
	case Boneskinner:
		return APIDefs->GetTextureOrCreateFromResource("TEX_BONESKINNER", IDB_BONESKINNER, hSelf);
	case WhisperOfJormag:
		return APIDefs->GetTextureOrCreateFromResource("TEX_WHISPER_OF_JORMAG", IDB_WHISPER_OF_JORMAG, hSelf);
	case Freezie:
		return APIDefs->GetTextureOrCreateFromResource("TEX_FREEZIE", IDB_FREEZIE, hSelf);
	case CaptainMaiTrin:
		return APIDefs->GetTextureOrCreateFromResource("TEX_CAPTAIN_MAI_TRIN", IDB_CAPTAIN_MAI_TRIN, hSelf);
	case Ankka:
		return APIDefs->GetTextureOrCreateFromResource("TEX_ANKKA", IDB_ANKKA, hSelf);
	case MinisterLi:
		return APIDefs->GetTextureOrCreateFromResource("TEX_MINISTER_LI", IDB_MINISTER_LI, hSelf);
	case VoidAmalgamate:
		return APIDefs->GetTextureOrCreateFromResource("TEX_HARVEST_TEMPLE", IDB_HARVEST_TEMPLE, hSelf);
	case OldLionsCourt:
		return APIDefs->GetTextureOrCreateFromResource("TEX_WATCHKNIGHT_MARK_II", IDB_WATCHKNIGHT_MARK_II, hSelf);
	case Dagda:
		return APIDefs->GetTextureOrCreateFromResource("TEX_DAGDA", IDB_DAGDA, hSelf);
	case Cerus:
		return APIDefs->GetTextureOrCreateFromResource("TEX_CERUS", IDB_CERUS, hSelf);
	default:
		return nullptr;
	}
}