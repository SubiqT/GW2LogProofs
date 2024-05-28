#include <format>
#include <thread>

#include "imgui/imgui.h"
#include "shared.h"
#include "log_proofs.h"
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
	| ImGuiTableFlags_SizingFixedFit
	| ImGuiTableFlags_Hideable
	| ImGuiTableFlags_Sortable
	| ImGuiTableFlags_RowBg
	| ImGuiTableFlags_ScrollX
	| ImGuiTableFlags_ScrollY
	);

void DrawBossesTab(const char* tabName, const char* tableName, std::vector<Boss>* bossesArray , bool isLegendary) {
	if (ImGui::BeginTabItem(tabName)) {
		if (ImGui::BeginTable(tableName, int(bossesArray->size()) + 1, tableFlags)) {
			ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
			for (Boss& boss : *bossesArray) {
				ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			for (Boss& boss : *bossesArray) {
				ImGui::TableNextColumn();
				Texture* texture = GetBossTexture(boss);
				if (texture != nullptr) {
					ImGui::Image((void*)texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
				}
				else {
					ImGui::Text(GetBossName(boss));
				}
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text(GetBossName(boss));
					ImGui::EndTooltip();
				}
			}
			{
				std::scoped_lock lck(LogProofs::Mutex);
				if (LogProofs::players.size() > 0) {
					for (LogProofs::Player p : LogProofs::players) {
						ImGui::TableNextColumn();
						ImGui::Text(p.account.c_str());
						for (Boss& boss : *bossesArray) {
							ImGui::TableNextColumn();
							if (p.state == LogProofs::READY) {
								if (isLegendary) {
									ImGui::Text("%i", p.kp[std::format("-{}", int(boss))]);
								}
								else {
									ImGui::Text("%i", p.kp[std::format("{}", int(boss))]);
								}
							}
							else {
								ImGui::Text("...");
							}
						}
					}
				}
			}
			if (LogProofs::players.size() == 0) {
				ImGui::TableNextColumn();
				ImGui::Text("No players found... ");
			}
			ImGui::EndTable();
			
		}
		ImGui::EndTabItem();
	}
}

void RenderWindowLogProofs() {
	if (!Settings::ShowWindowLogProofs) {
		return;
	}
	ImGui::SetNextWindowSizeConstraints(ImVec2(Settings::MinWindowWidth, Settings::MinWindowHeight), ImVec2(Settings::MaxWindowWidth,Settings::MaxWindowHeight));
	if (ImGui::Begin("Log Proofs", &Settings::ShowWindowLogProofs, windowFlags)) {
		if (ImGui::BeginTabBar("##GameModes", ImGuiTabBarFlags_None)) {
			if (Settings::ShowTabRaidsNormal) {
				DrawBossesTab("Raids", "normalRaidsTable", &sortedRaidBosses, false);
			}
			if (Settings::ShowTabRaidsCM) {
				DrawBossesTab("Raid CMs", "cmRaidsTable", &sortedRaidCmBosses, false);
			}
			if (Settings::ShowTabFractalsNormal) {
				DrawBossesTab("Fractals", "normalFractalsTable", &sortedFractalBosses, false);
			}
			if (Settings::ShowTabFractalsCM) {
				DrawBossesTab("Fractal CMs", "cmFractalsTable", &sortedFractalCMBosses, false);
			}
			if (Settings::ShowTabStrikesNormal) {
				DrawBossesTab("Strikes", "normalStrikesTable", &sortedStrikeBosses, false);
			}
			if (Settings::ShowTabStrikesCM) {
				DrawBossesTab("Strike CMs", "cmStrikesTable", &sortedStrikeCMBosses, false);
			}
			if (Settings::ShowTabStrikesLM) {
				DrawBossesTab("Strike LMs", "lmStrikesTable", &sortedStrikeLMBosses, true);
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

void ToggleShowWindowLogProofs(const char* keybindIdentifier) {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Keybind {} was pressed.", keybindIdentifier).c_str());
	Settings::ShowWindowLogProofs = !Settings::ShowWindowLogProofs;
	Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
	Settings::Save(SettingsPath);
}

void DrawWindowSizingOptions() {
	ImGui::Text("Window Sizing");
	if (ImGui::SliderFloat("Min Width", &Settings::MinWindowWidth, 100.0f, 1500.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_WIDTH] = Settings::MinWindowWidth;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("Max Width", &Settings::MaxWindowWidth, 100.0f, 1500.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_WIDTH] = Settings::MaxWindowWidth;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("Min Height", &Settings::MinWindowHeight, 100.0f, 1500.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_HEIGHT] = Settings::MinWindowHeight;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("Max Height", &Settings::MaxWindowHeight, 100.0f, 1500.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_HEIGHT] = Settings::MaxWindowHeight;
		Settings::Save(SettingsPath);
	}
}

void DrawTabsOptions() {
	ImGui::Text("Tabs");
	if (ImGui::Checkbox("Raids", &Settings::ShowTabRaidsNormal)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_NORMAL] = Settings::ShowTabRaidsNormal;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Raid CMs", &Settings::ShowTabRaidsCM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_CM] = Settings::ShowTabRaidsCM;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Fractals", &Settings::ShowTabFractalsNormal)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_FRACTALS_NORMAL] = Settings::ShowTabRaidsNormal;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Fractal CMs", &Settings::ShowTabFractalsCM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_FRACTALS_CM] = Settings::ShowTabRaidsCM;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strikes", &Settings::ShowTabStrikesNormal)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_NORMAL] = Settings::ShowTabRaidsNormal;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strike CMs", &Settings::ShowTabStrikesCM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_CM] = Settings::ShowTabStrikesCM;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strike LMs", &Settings::ShowTabStrikesLM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_LM] = Settings::ShowTabStrikesLM;
		Settings::Save(SettingsPath);
	}
}

void DrawColumnOptions() {
	ImGui::Text("Columns");
	if (ImGui::SliderFloat("Account Size", &Settings::ColumnSizeAccount, 40.0f, 400.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE] = Settings::ColumnSizeAccount;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("Bosses Size", &Settings::ColumnSizeBosses, 8.0f, 128.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE] = Settings::ColumnSizeBosses;
		Settings::Save(SettingsPath);
	}
}

void RenderWindowSettings() {
	if (ImGui::Checkbox("Show Window", &Settings::ShowWindowLogProofs)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
		Settings::Save(SettingsPath);
	}
	DrawWindowSizingOptions();
	DrawTabsOptions();
	DrawColumnOptions();
}
