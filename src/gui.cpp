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
	TwistedCastle = 16247,
	Xera = 16246,
	Cairn = 17194,
	MursaatOverseer = 17172,
	Samarog = 17188,
	Deimos = 17154,
	SoullessHorror = 19767,
	RiverOfSouls = 19828,
	BrokenKing = 19691,
	EaterOfSouls = 19536,
	Eyes = 19651,
	Dhuum = 19450,
	ConjuredAmalgamate = 43974,
	TwinLargos = 21105,
	Qadim = 20934,
	Adina = 22006,
	Sabir = 21964,
	QadimThePeerless = 22000
};
Boss sortedBosses[26] = {
	ValeGuardian, Gorseval, Sabetha,
	Slothasor, BanditTrio, Matthias,
	Escort, KeepConstruct, TwistedCastle, Xera,
	Cairn, MursaatOverseer, Samarog, Deimos,
	SoullessHorror, RiverOfSouls, BrokenKing, EaterOfSouls, Eyes, Dhuum,
	ConjuredAmalgamate, TwinLargos, Qadim,
	Adina, Sabir, QadimThePeerless
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
RaidWing sortedRaidWings[7] = {
	SpiritVale, SalvationPass, StrongholdOfTheFaithful, BastionOfThePenitent,
	HallOfChains, MythwrightGambit, TheKeyOfAhdashim
};

const char* GetBossName(Boss boss) {
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
	default:
		return "Unknown";
	}
}

Texture* GetBossTexture(Boss boss) {
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
	default:
		return nullptr;
	}
}

void RenderWindow() {
	if (!Config.showWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs", &Config.showWindow, windowFlags)) {
		if (ImGui::BeginTabBar("##GameModes", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem("Raids")) {
				if (ImGui::BeginTable("raidsTable", 27, tableFlags)) {
					ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 32.f);
					for (Boss& boss : sortedBosses) {
						ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 32.f);
					}
					ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
					ImGui::TableNextColumn();
					ImGui::Text("Account");
					for (Boss& boss : sortedBosses) {
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
							for (Boss& boss : sortedBosses) {
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
