#include "shared.h"
#include "bosses.h"

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
		return APIDefs->GetTextureOrCreateFromURL("vale_guardian", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Vale_Guardian_Trophy.png");
	case Gorseval:
		return APIDefs->GetTextureOrCreateFromURL("gorseval", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Gorseval_Trophy.png");
	case Sabetha:
		return APIDefs->GetTextureOrCreateFromURL("sabetha", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Sabetha_Trophy.png");
	case Slothasor:
		return APIDefs->GetTextureOrCreateFromURL("slothasor", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Slothasor_Trophy.png");
	case BanditTrio:
		return APIDefs->GetTextureOrCreateFromURL("bandit_trio", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Narella.png");
	case Matthias:
		return APIDefs->GetTextureOrCreateFromURL("matthias", "https://gw2wingman.nevermindcreations.de", "/static/Silver_White_Mantle_Abomination_Trophy.png");
	case Escort:
		return APIDefs->GetTextureOrCreateFromURL("escort", "https://gw2wingman.nevermindcreations.de", "/static/Bloodstone_Turret_Fragment.png");
	case KeepConstruct:
		return APIDefs->GetTextureOrCreateFromURL("keep_construct", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Keep_Construct_Trophy.png");
	case TwistedCastle:
		return APIDefs->GetTextureOrCreateFromURL("twisted_castle", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Siege_the_Stronghold_Trophy.png");
	case Xera:
		return APIDefs->GetTextureOrCreateFromURL("xera", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Xera_Trophy.png");
	case Cairn:
		return APIDefs->GetTextureOrCreateFromURL("cairn", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Cairn_the_Indomitable_Trophy.png");
	case MursaatOverseer:
		return APIDefs->GetTextureOrCreateFromURL("mursaat_overseer", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Mursaat_Overseer_Trophy.png");
	case Samarog:
		return APIDefs->GetTextureOrCreateFromURL("samarog", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Samarog_Trophy.png");
	case Deimos:
		return APIDefs->GetTextureOrCreateFromURL("deimos", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Deimos_Trophy.png");
	case SoullessHorror:
		return APIDefs->GetTextureOrCreateFromURL("soulless_horror", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Desmina_Trophy.png");
	case RiverOfSouls:
		return APIDefs->GetTextureOrCreateFromURL("river_of_souls", "https://gw2wingman.nevermindcreations.de", "/static/Silver_River_of_Souls_Trophy.png");
	case BrokenKing:
		return APIDefs->GetTextureOrCreateFromURL("broken_king", "https://gw2wingman.nevermindcreations.de", "/static/Icon_BrokenKing.png");
	case EaterOfSouls:
		return APIDefs->GetTextureOrCreateFromURL("eater_of_souls", "https://gw2wingman.nevermindcreations.de", "/static/Icon_EaterOfSouls.png");
	case Eyes:
		return APIDefs->GetTextureOrCreateFromURL("eyes", "https://gw2wingman.nevermindcreations.de", "/static/Icon_Eyes.png");
	case Dhuum:
		return APIDefs->GetTextureOrCreateFromURL("dhuum", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Dhuum_Trophy.png");
	case ConjuredAmalgamate:
		return APIDefs->GetTextureOrCreateFromURL("conjured_amalgamate", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Conjured_Amalgamate_Trophy.png");
	case TwinLargos:
		return APIDefs->GetTextureOrCreateFromURL("twin_largos", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Twin_Largos_Trophy.png");
	case Qadim:
		return APIDefs->GetTextureOrCreateFromURL("qadim", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Qadim_Trophy.png");
	case Adina:
		return APIDefs->GetTextureOrCreateFromURL("adina", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Cardinal_Adina_Trophy.png");
	case Sabir:
		return APIDefs->GetTextureOrCreateFromURL("sabir", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Cardinal_Sabir_Trophy.png");
	case QadimThePeerless:
		return APIDefs->GetTextureOrCreateFromURL("qadim_the_pearless", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Ether_Djinn_Trophy.png");
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