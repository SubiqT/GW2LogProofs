#include <format>
#include <thread>

#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"
#include "bosses.h"
#include "settings.h"

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

void RenderWindowLogProofs() {
	if (!Settings::ShowWindowLogProofs) {
		return;
	}
	if (ImGui::Begin("Log Proofs", &Settings::ShowWindowLogProofs, windowFlags)) {
		if (ImGui::BeginTabBar("##GameModes", ImGuiTabBarFlags_None)) {
			if (Settings::ShowTabRaidsNormal) {
				DrawBossesTab("Normal Raids", "normalRaidsTable", &sortedRaidBosses);
			}
			if (Settings::ShowTabRaidsCM) {
				DrawBossesTab("Raid CMs", "cmRaidsTable", &sortedRaidCmBosses);
			}
			if (Settings::ShowTabFractalsNormal) {
				DrawBossesTab("Normal Fractals", "normalFractalsTable", &sortedFractalBosses);
			}
			if (Settings::ShowTabFractalsCM) {
				DrawBossesTab("Fractal CMs", "cmFractalsTable", &sortedFractalCMBosses);
			}
			if (Settings::ShowTabStrikesNormal) {
				DrawBossesTab("Normal Strikes", "normalStrikesTable", &sortedStrikeBosses);
			}
			if (Settings::ShowTabStrikesCM) {
				DrawBossesTab("Strike CMs", "cmStrikesTable", &sortedStrikeCMBosses);
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

void ToggleShowWindowLogProofs(const char* keybindIdentifier) {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Keybind {} was pressed.", keybindIdentifier).c_str());
	Settings::ShowWindowLogProofs = !Settings::ShowWindowLogProofs;
	Settings::Settings[SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
	Settings::Save(SettingsPath);
}

void RenderWindowSettings() {
	ImGui::TextDisabled("ShowTabRaidsNormal");
	if (ImGui::Checkbox("Enabled##ShowTabRaidsNormal", &Settings::ShowTabRaidsNormal)) {
		Settings::Settings[SHOW_TAB_RAIDS_NORMAL] = Settings::ShowTabRaidsNormal;
		Settings::Save(SettingsPath);
	}
	ImGui::TextDisabled("ShowTabRaidsCM");
	if (ImGui::Checkbox("Enabled##ShowTabRaidsCM", &Settings::ShowTabRaidsCM)) {
		Settings::Settings[SHOW_TAB_RAIDS_CM] = Settings::ShowTabRaidsCM;
		Settings::Save(SettingsPath);
	}
	ImGui::TextDisabled("ShowTabFractalsNormal");
	if (ImGui::Checkbox("Enabled##ShowTabFractalsNormal", &Settings::ShowTabFractalsNormal)) {
		Settings::Settings[SHOW_TAB_FRACTALS_NORMAL] = Settings::ShowTabRaidsNormal;
		Settings::Save(SettingsPath);
	}
	ImGui::TextDisabled("ShowTabFractalsCM");
	if (ImGui::Checkbox("Enabled##ShowTabFractalsCM", &Settings::ShowTabFractalsCM)) {
		Settings::Settings[SHOW_TAB_FRACTALS_CM] = Settings::ShowTabRaidsCM;
		Settings::Save(SettingsPath);
	}
	ImGui::TextDisabled("ShowTabStrikesNormal");
	if (ImGui::Checkbox("Enabled##ShowTabStrikesNormal", &Settings::ShowTabStrikesNormal)) {
		Settings::Settings[SHOW_TAB_STRIKES_NORMAL] = Settings::ShowTabRaidsNormal;
		Settings::Save(SettingsPath);
	}
	ImGui::TextDisabled("ShowTabStrikesCM");
	if (ImGui::Checkbox("Enabled##ShowTabStrikesCM", &Settings::ShowTabStrikesCM)) {
		Settings::Settings[SHOW_TAB_STRIKES_CM] = Settings::ShowTabStrikesCM;
		Settings::Save(SettingsPath);
	}
}
