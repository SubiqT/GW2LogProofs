#include <format>
#include <thread>
#include <algorithm>

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

enum DataSource {
	WINGMAN,
	KPME
};

DataSource selectedDataSource = WINGMAN;
std::vector<std::string> dataSources = { "Wingman", "Kpme" };
bool includeAltAccounts = false;

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
							if (p.wingmanState == LogProofs::READY) {
								if (isLegendary) {
									ImGui::Text("%i", p.wingman[std::format("-{}", int(boss))]);
								}
								else {
									ImGui::Text("%i", p.wingman[std::format("{}", int(boss))]);
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

void DrawKpmeSummaryTab(const char* tabName, const char* tableName, std::vector<std::string>* proofsArray) {
	if (ImGui::BeginTabItem(tabName)) {
		if (ImGui::BeginTable(tableName, int(proofsArray->size()) + 2, tableFlags)) {
			ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
			ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 30.0f);
			for (std::string proof : *proofsArray) {
				ImGui::TableSetupColumn(proof.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			ImGui::Text("Id");
			for (std::string proof : *proofsArray) {
				ImGui::TableNextColumn();
				Texture* texture = GetCurrencyTexture(proof);
				if (texture != nullptr) {
					ImGui::Image((void*)texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
				}
				else {
					ImGui::Text(proof.c_str());
				}
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text(proof.c_str());
					ImGui::EndTooltip();
				}
			}
			{
				std::scoped_lock lck(LogProofs::Mutex);
				if (LogProofs::players.size() > 0) {
					for (LogProofs::Player p : LogProofs::players) {
						ImGui::TableNextColumn();
						ImGui::Text(p.account.c_str());
						ImGui::TableNextColumn();
						ImGui::Text(!p.kpme.id.empty() ? p.kpme.id.c_str() : "...");
						for (std::string proof : *proofsArray) {
							ImGui::TableNextColumn();
							if (p.kpmeState == LogProofs::READY) {
								int amount = 0;
								if (includeAltAccounts) {
									if (p.kpme.shared.killproofs.contains(proof)) {
										amount = p.kpme.shared.killproofs.at(proof);
									}
									if (proof == "Legendary Insight") {
										if (p.kpme.shared.killproofs.contains("Legendary Divination")) {
											amount += p.kpme.shared.killproofs.at("Legendary Divination");
										}
									}
									if (proof == "Unstable Fractal Essence") {
										if (p.kpme.shared.killproofs.contains("Unstable Cosmic Essence")) {
											amount += p.kpme.shared.killproofs.at("Unstable Cosmic Essence") * 5;
										}
									}
								}
								if (amount == 0) {
									if (p.kpme.self.killproofs.contains(proof)) {
										amount = p.kpme.self.killproofs.at(proof);
									}
									if (proof == "Legendary Insight") {
										if (p.kpme.self.killproofs.contains("Legendary Divination")) {
											amount += p.kpme.self.killproofs.at("Legendary Divination");
										}
									}
									if (proof == "Unstable Fractal Essence") {
										if (p.kpme.self.killproofs.contains("Unstable Cosmic Essence")) {
											amount += p.kpme.self.killproofs.at("Unstable Cosmic Essence") * 5;
										}
									}
								}
								ImGui::Text("%i", amount);
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


void DrawKpmeTokensTab(const char* tabName, const char* tableName, std::vector<Boss>* bossesArray) {
	if (ImGui::BeginTabItem(tabName)) {
		if (ImGui::BeginTable(tableName, int(bossesArray->size()) + 2, tableFlags)) {
			ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
			ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 30.0f);
			for (Boss boss : *bossesArray) {
				ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			ImGui::Text("Id");
			for (Boss& boss : *bossesArray) {
				ImGui::TableNextColumn();
				Texture* texture = GetBossTexture(boss);
				if (texture != nullptr) {
					ImGui::Image((void*)texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
				}
				else {
					ImGui::Text(GetKpMeBossToken(boss).c_str());
				}
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text(GetKpMeBossToken(boss).c_str());
					ImGui::EndTooltip();
				}
			}
			{
				std::scoped_lock lck(LogProofs::Mutex);
				if (LogProofs::players.size() > 0) {
					for (LogProofs::Player p : LogProofs::players) {
						ImGui::TableNextColumn();
						ImGui::Text(p.account.c_str());
						ImGui::TableNextColumn();
						ImGui::Text(!p.kpme.id.empty() ? p.kpme.id.c_str() : "...");
						for (Boss boss : *bossesArray) {
							ImGui::TableNextColumn();
							if (p.kpmeState == LogProofs::READY) {
								int amount = 0;
								if (includeAltAccounts) {
									if (p.kpme.shared.tokens.contains(GetKpMeBossToken(boss))) {
										amount = p.kpme.shared.tokens.at(GetKpMeBossToken(boss));
									}
									if (p.kpme.shared.coffers.contains(GetKpMeBossCoffer(boss))) {
										amount += p.kpme.shared.coffers.at(GetKpMeBossCoffer(boss)) * 3;  // 1 Coffer == 3 Tokens
									}
								}
								if (amount == 0) {
									if (p.kpme.self.tokens.contains(GetKpMeBossToken(boss))) {
										amount = p.kpme.self.tokens.at(GetKpMeBossToken(boss));
									}
									if (p.kpme.self.coffers.contains(GetKpMeBossCoffer(boss))) {
										amount += p.kpme.self.coffers.at(GetKpMeBossCoffer(boss))  * 3;  // 1 Coffer == 3 Tokens
									}
								}
								ImGui::Text("%i", amount);
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

void DrawKpmeCoffersTab(const char* tabName, const char* tableName, std::vector<Boss>* bossesArray) {
	if (ImGui::BeginTabItem(tabName)) {
		if (ImGui::BeginTable(tableName, int(bossesArray->size()) + 2, tableFlags)) {
			ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
			ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 30.0f);
			for (Boss boss : *bossesArray) {
				ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			ImGui::Text("Id");
			for (Boss& boss : *bossesArray) {
				ImGui::TableNextColumn();
				Texture* texture = GetBossTexture(boss);
				if (texture != nullptr) {
					ImGui::Image((void*)texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
				}
				else {
					ImGui::Text(GetKpMeBossCoffer(boss).c_str());
				}
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text(GetKpMeBossCoffer(boss).c_str());
					ImGui::EndTooltip();
				}
			}
			{
				std::scoped_lock lck(LogProofs::Mutex);
				if (LogProofs::players.size() > 0) {
					for (LogProofs::Player p : LogProofs::players) {
						ImGui::TableNextColumn();
						ImGui::Text(p.account.c_str());
						ImGui::TableNextColumn();
						ImGui::Text(!p.kpme.id.empty() ? p.kpme.id.c_str() : "...");
						for (Boss boss : *bossesArray) {
							ImGui::TableNextColumn();
							if (p.kpmeState == LogProofs::READY) {
								int amount = 0;
								if (includeAltAccounts) {
									if (p.kpme.shared.coffers.contains(GetKpMeBossCoffer(boss))) {
										amount = p.kpme.shared.coffers.at(GetKpMeBossCoffer(boss));
									}
								}
								if (amount == 0) {
									if (p.kpme.self.coffers.contains(GetKpMeBossCoffer(boss))) {
										amount = p.kpme.self.coffers.at(GetKpMeBossCoffer(boss));
									}
								}
								ImGui::Text("%i", amount);
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

		if (ImGui::BeginCombo("Source##DataSource", dataSources[selectedDataSource].c_str())) {
			for (int index = 0; index < dataSources.size(); ++index) {
				const bool is_selected = (dataSources[index] == dataSources[selectedDataSource]);
				if (ImGui::Selectable(dataSources[index].c_str(), is_selected)) {
					selectedDataSource = DataSource(index);
				}
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (selectedDataSource == WINGMAN) {
			if (ImGui::BeginTabBar("##Wingman", ImGuiTabBarFlags_None)) {
				if (Settings::ShowTabRaidsNormal) {
					DrawBossesTab("Raids", "normalRaidsTable", &sortedRaidBosses, false);
				}
				if (Settings::ShowTabRaidsCM) {
					DrawBossesTab("Raid CMs", "cmRaidsTable", &sortedRaidCmBosses, false);
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
		
		if (selectedDataSource == KPME) {
			ImGui::SameLine();
			if (ImGui::Checkbox("Include Linked Accounts", &includeAltAccounts)) {
				// Save to disk once this is a proper setting
			}
			if (ImGui::BeginTabBar("##Kpme", ImGuiTabBarFlags_None)) {
				if (Settings::ShowTabKpmeSummary) { 
					DrawKpmeSummaryTab("Summary", "kpmeSummaryTable", &sortedKpmeSummary);
				}
				if (Settings::ShowTabKpmeRaidTokens) {
					DrawKpmeTokensTab("Raid Tokens", "kpmeRaidsTable", &sortedKpmeRaidBosses);
				}
				if (Settings::ShowTabKpmeStrikeCoffers) {
					DrawKpmeCoffersTab("Strike Coffers", "kpmeStrikesTable", &sortedKpmeStrikeBosses);
				}
				if (Settings::ShowTabKpmeStrikeCMCoffers) {
					DrawKpmeCoffersTab("Strike CM Coffers", "kpmeStrikeCMsTable", &sortedKpmeStrikeCMBosses);
				}
				// Skip titles for now - they can be added with the alternative "profile" view

				ImGui::EndTabBar();
			}
		}
	}
	ImGui::End();

	if (!Settings::ShowWindowLogProofs) { // X button on top right was clicked
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
		Settings::Save(SettingsPath);
	}
}

void ToggleShowWindowLogProofs(const char* keybindIdentifier, bool isRelease) {
	if (isRelease) return;
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
	ImGui::Text("Wingman Tabs");
	if (ImGui::Checkbox("Raids", &Settings::ShowTabRaidsNormal)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_NORMAL] = Settings::ShowTabRaidsNormal;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Raid CMs", &Settings::ShowTabRaidsCM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_CM] = Settings::ShowTabRaidsCM;
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
	ImGui::Text("Kpme Tabs");
	if (ImGui::Checkbox("Summary", &Settings::ShowTabKpmeSummary)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_SUMMARY] = Settings::ShowTabKpmeSummary;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Raid Tokens", &Settings::ShowTabKpmeRaidTokens)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_RAID_TOKENS] = Settings::ShowTabKpmeRaidTokens;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strike Coffers", &Settings::ShowTabKpmeStrikeCoffers)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_STRIKE_COFFERS] = Settings::ShowTabKpmeStrikeCoffers;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strike CM Coffers", &Settings::ShowTabKpmeStrikeCMCoffers)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_STRIKE_CM_COFFERS] = Settings::ShowTabKpmeStrikeCMCoffers;
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

void RegisterQuickAccessShortcut() {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "registering quick access shortcut");
	APIDefs->AddShortcut("SHORTCUT_LOG_PROOFS", "TEX_LOG_NORMAL", "TEX_LOG_HOVER", KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS, "Toggle Log Proofs Window");
}

void DeregisterQuickAccessShortcut() {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "deregistering quick access shortcut");
	APIDefs->RemoveShortcut("SHORTCUT_LOG_PROOFS");
}

void RenderWindowSettings() {
	if (ImGui::Checkbox("Show Quick Access Shortcut", &Settings::ShowQuickAccessShortcut)) {
		Settings::Settings[SHOW_QUICK_ACCESS_SHORTCUT] = Settings::ShowQuickAccessShortcut;
		Settings::Save(SettingsPath);
		if (Settings::ShowQuickAccessShortcut) RegisterQuickAccessShortcut();
		else DeregisterQuickAccessShortcut();
	}
	if (ImGui::Checkbox("Show Window", &Settings::ShowWindowLogProofs)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
		Settings::Save(SettingsPath);
	}
	DrawWindowSizingOptions();
	DrawTabsOptions();
	DrawColumnOptions();
}

