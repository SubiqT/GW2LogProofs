#include <format>
#include <thread>

#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"

static ImGuiWindowFlags windowFlags =  (
	ImGuiWindowFlags_NoCollapse
	| ImGuiWindowFlags_NoFocusOnAppearing
	| ImGuiWindowFlags_NoNav
	| ImGuiWindowFlags_NoNavFocus
	| ImGuiWindowFlags_AlwaysAutoResize
);
static ImGuiTableFlags tableFlags = (
	ImGuiTableFlags_Borders
	| ImGuiTableFlags_ContextMenuInBody
	| ImGuiTableFlags_NoSavedSettings
	| ImGuiTableFlags_SizingFixedFit
	| ImGuiTableFlags_Hideable
	| ImGuiTableFlags_Sortable
	| ImGuiTableFlags_RowBg
	| ImGuiTableFlags_Resizable
	);


enum Boss {
	ValeGuardian = 15438,
	Gorseval = 15429,
	Sabetha = 15375,
	Slothasor = 16123,
	BanditTrio = 16088,
	Matthias = 16115,
	Escort = 16253,
	KeepConstruct = 16235,
	KeepConstructCM = -16235,
	TwistedCastle = 16247,
	Xera = 16246,
	Cairn = 17194,
	CairnCM = -17194,
	MursaatOverseer = 17172,
	MursaatOverseerCM = -17172,
	Samarog = 17188,
	SamarogCM = -17188,
	Deimos = 17154,
	DeimosCM = -17154,
	SoullessHorror = 19767,
	SoullessHorrorCM = -19767,
	RiverOfSouls = 19828,
	BrokenKing = 19691,
	EaterOfSouls = 19536,
	Eyes = 19651,
	Dhuum = 19450,
	DhuumCM = -19450,
	ConjuredAmalgamate = 43974,
	ConjuredAmalgamateCM = -43974,
	TwinLargos = 21105,
	TwinLargosCM = -21105,
	Qadim = 20934,
	QadimCM = -20934,
	Adina = 22006,
	AdinaCM = -22006,
	Sabir = 21964,
	SabirCM = -21964,
	QadimThePeerless = 22000,
	QadimThePeerlessCM = -22000,
	MAMA = 17021,
	MAMACM = -17021,
	SiaxTheCorrupted = 17028,
	SiaxTheCorruptedCM = -17028,
	EnsolyssOfTheEndlessTorment = 16948,
	EnsolyssOfTheEndlessTormentCM = -16948,
	SkorvaldTheShattered = 17632,
	SkorvaldTheShatteredCM = -17632,
	Artsariiv = 17949,
	ArtsariivCM = -17949,
	Arkk = 17759,
	ArkkCM = -17759,
	ElementalAiKeeperOfThePeak = 23254,
	ElementalAiKeeperOfThePeakCM = -23254,
	DarkAiKeeperOfThePeak = 232542,
	DarkAiKeeperOfThePeakCM = -232542,
	ElementalAndDarkAiKeeperOfThePeak = 232543,
	ElementalAndDarkAiKeeperOfThePeakCM = -232543,
	KanaxaiScytheOfHouseAurkus = 25577,
	KanaxaiScytheOfHouseAurkusCM = -25577
};
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

enum RaidWing {
	SpiritVale,
	SalvationPass,
	StrongholdOfTheFaithful,
	BastionOfThePenitent,
	HallOfChains,
	MythwrightGambit,
	TheKeyOfAhdashim
};
std::vector<RaidWing> sortedRaidWings {
	SpiritVale, SalvationPass, StrongholdOfTheFaithful, BastionOfThePenitent,
	HallOfChains, MythwrightGambit, TheKeyOfAhdashim
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
	default:
		return nullptr;
	}
}

void DrawBossesTab(const char* tabName, const char* tableName, std::vector<Boss>* bossesArray) {
	if (ImGui::BeginTabItem(tabName)) {
		if (ImGui::BeginTable(tableName, int(bossesArray->size()) + 1, tableFlags)) {
			ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 32.f);
			for (Boss& boss : *bossesArray) {
				ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 32.f);
			}
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			for (Boss& boss : *bossesArray) {
				ImGui::TableNextColumn();
				Texture* texture = GetBossTexture(boss);
				if (texture != nullptr) {
					ImGui::Image((void*)texture->Resource, ImVec2(32.f, 32.f));
				}
				else {
					ImGui::Text(GetBossName(boss));
				}
			}
			if (players.size() > 0) {
				for (Player player : players) {
					ImGui::TableNextColumn();
					ImGui::Text(player.account.c_str());
					for (Boss& boss : *bossesArray) {
						ImGui::TableNextColumn();
						ImGui::Text("%i", player.kp[std::format("{}", int(boss))][std::string("total")]);
					}
				}
			}
			ImGui::EndTable();
			if (players.size() == 0) {
				ImGui::Text("No players found... ");
			}
		}
		ImGui::EndTabItem();
	}
}

void RenderWindow() {
	if (!Config.showWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs", &Config.showWindow, windowFlags)) {
		if (ImGui::BeginTabBar("##GameModes", ImGuiTabBarFlags_None)) {
			DrawBossesTab("Normal Raids", "normalRaidsTable", &sortedRaidBosses);
			DrawBossesTab("Raid CMs", "cmRaidsTable", &sortedRaidCmBosses);
			DrawBossesTab("Normal Fractals", "normalFractalsTable", &sortedFractalBosses);
			DrawBossesTab("Fractal CMs", "cmFractalsTable", &sortedFractalCMBosses);
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

void ToggleShowWindow(const char* aIdentifier) {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Keybind {} was pressed.", aIdentifier).c_str());
	Config.showWindow = !Config.showWindow;
}

void RenderSettings() {
	if (ImGui::Button(Config.showWindow ? "Hide Window" : "Show Window")) {
		Config.showWindow = !Config.showWindow;
	}
}
