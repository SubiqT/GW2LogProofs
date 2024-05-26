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
		return "The Voice and The Claw";
	case FraenirOfJormag:
		return "Fraenir of Jormag";
	case Boneskinner:
		return "BoneSkinner";
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
		return "Void Amalgamate";
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
		return APIDefs->GetTextureOrCreateFromResource("TEX_BANDIT_TRIO", IDB_BANDIT_TRIO, hSelf);
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
		return APIDefs->GetTextureOrCreateFromURL("mama", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Watchknight.png");
	case SiaxTheCorrupted:
		return APIDefs->GetTextureOrCreateFromURL("siax", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Toxic_Nimross.png");
	case EnsolyssOfTheEndlessTorment:
		return APIDefs->GetTextureOrCreateFromURL("ensolyss", "https://gw2wingman.nevermindcreations.de", "/static/EnsolyssIcon.png");
	case SkorvaldTheShattered:
		return APIDefs->GetTextureOrCreateFromURL("skorvald", "https://gw2wingman.nevermindcreations.de", "/static/Male_Norn_Holo-Dancer.png");
	case Artsariiv:
		return APIDefs->GetTextureOrCreateFromURL("artsariiv", "https://gw2wingman.nevermindcreations.de", "/static/Female_Norn_Holo-Dancer.png");
	case Arkk:
		return APIDefs->GetTextureOrCreateFromURL("arkk", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Inquest_Extinguisher.png");
	case ElementalAiKeeperOfThePeak:
		return APIDefs->GetTextureOrCreateFromURL("elemental_ai", "https://gw2wingman.nevermindcreations.de", "/static/ElementalAi.png");
	case DarkAiKeeperOfThePeak:
		return APIDefs->GetTextureOrCreateFromURL("dark_ai", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Liadri_the_Concealing_Dark.png");
	case ElementalAndDarkAiKeeperOfThePeak:
		return APIDefs->GetTextureOrCreateFromURL("full_ai", "https://gw2wingman.nevermindcreations.de", "/static/AiFullEncounter.png");
	case KanaxaiScytheOfHouseAurkus:
		return APIDefs->GetTextureOrCreateFromURL("kanaxai", "https://gw2wingman.nevermindcreations.de", "/static/Kanaxai.png");
	case IcebroodConstruct:
		return APIDefs->GetTextureOrCreateFromURL("icebrood_construct", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Icebrood_Construct.png");
	case TheVoiceAndTheClaw:
		return APIDefs->GetTextureOrCreateFromURL("the_voice_and_the_claw", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Kodan_Icehammer.png");
	case FraenirOfJormag:
		return APIDefs->GetTextureOrCreateFromURL("fraenir_of_jormag", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Shaman_Of_Jormag.png");
	case Boneskinner:
		return APIDefs->GetTextureOrCreateFromURL("boneskinner", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Boneskinner.png");
	case WhisperOfJormag:
		return APIDefs->GetTextureOrCreateFromURL("whisper_of_jormag", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Whisper_of_Jormag.png");
	case Freezie:
		return APIDefs->GetTextureOrCreateFromURL("freezie", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Freezie.png");
	case CaptainMaiTrin:
		return APIDefs->GetTextureOrCreateFromURL("captain_mai_trin", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Mai_Trin.png");
	case Ankka:
		return APIDefs->GetTextureOrCreateFromURL("ankka", "https://gw2wingman.nevermindcreations.de", "/static/Ankka.png");
	case MinisterLi:
		return APIDefs->GetTextureOrCreateFromURL("minister_li", "https://gw2wingman.nevermindcreations.de", "/static/Li.png");
	case VoidAmalgamate:
		return APIDefs->GetTextureOrCreateFromURL("void_amalgamate", "https://gw2wingman.nevermindcreations.de", "/static/VA.png");
	case OldLionsCourt:
		return APIDefs->GetTextureOrCreateFromURL("old_lions_court", "https://gw2wingman.nevermindcreations.de", "/static/OLC.png");
	case Dagda:
		return APIDefs->GetTextureOrCreateFromURL("dagda", "https://gw2wingman.nevermindcreations.de", "/static/Dagda.png");
	case Cerus:
		return APIDefs->GetTextureOrCreateFromURL("cerus", "https://gw2wingman.nevermindcreations.de", "/static/Cerus.png");
	default:
		return nullptr;
	}
}