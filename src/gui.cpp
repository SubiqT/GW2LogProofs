#include <format>
#include <thread>
#include <algorithm>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui_extensions.h"
#include "shared.h"
#include "log_proofs.h"
#include "bosses.h"
#include "settings.h"

static ImGuiWindowFlags windowFlags =  (
	ImGuiWindowFlags_NoCollapse
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

std::vector<std::string> dataSources = { "Wingman", "Kpme" };

void DrawWingmanAccountName(LogProofs::Player aPlayer)
{
	if (!aPlayer.wingman.account.empty())
	{
		if (ImGui::TextURL(aPlayer.account.c_str()))
		{
			ShellExecuteA(0, 0, ("https://gw2wingman.nevermindcreations.de/kp/" + aPlayer.account).c_str(), 0, 0, SW_SHOW);
		}
	}
	else
	{
		ImGui::Text(aPlayer.account.c_str());
	}
}

void DrawKpmeAccountName(LogProofs::Player aPlayer)
{
	if (!aPlayer.kpme.id.empty())
	{
		if (ImGui::TextURL(aPlayer.account.c_str()))
		{
			ShellExecuteA(0, 0, ("https://killproof.me/proof/" + aPlayer.kpme.id).c_str(), 0, 0, SW_SHOW);
		}
	}
	else
	{
		ImGui::Text(aPlayer.account.c_str());
	}
}

void DrawKpmeId(LogProofs::Player aPlayer) {
	if (!aPlayer.kpme.id.empty())
	{
		if (ImGui::TextURL(aPlayer.kpme.id.c_str()))
		{
			ImGui::SetClipboardText(aPlayer.kpme.id.c_str());
		}
	}
	else
	{
		ImGui::Text("-");
	}
}

void HighlightColumnOnHover() {
	if (!Settings::hoverEnabled) return;
	if (ImGui::TableGetColumnFlags(ImGui::TableGetColumnIndex()) & ImGuiTableColumnFlags_IsHovered) {
		ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, Settings::hoverColour);
	}
}

void HighlightRowOnHover(ImGuiTable* table) {
	if (!Settings::hoverEnabled) return;
	ImRect rowRect(
		table->WorkRect.Min.x,
		table->RowPosY1,
		table->WorkRect.Max.x,
		table->RowPosY2
	);
	rowRect.ClipWith(table->BgClipRect);

	bool rowHovered = (
		ImGui::IsMouseHoveringRect(rowRect.Min, rowRect.Max, false) &&
		ImGui::IsWindowHovered(ImGuiHoveredFlags_None) &&
		!ImGui::IsAnyItemHovered()
	);

	if (rowHovered) {
		table->RowBgColor[1] = ImGui::GetColorU32(Settings::hoverColour);
	}
}

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
				HighlightColumnOnHover();
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
						if (!Settings::IncludeMissingAccounts && p.wingman.account.empty() && p.wingmanState == LogProofs::READY) {
							continue;
						}
						ImGui::TableNextColumn();
						DrawWingmanAccountName(p);
						for (Boss& boss : *bossesArray) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							if (p.wingmanState == LogProofs::READY) {
								if (!p.wingman.account.empty()) {
									if (isLegendary) {
										ImGui::Text("%i", p.wingman.kp[std::format("-{}", int(boss))]);
									}
									else {
										ImGui::Text("%i", p.wingman.kp[std::format("{}", int(boss))]);
									}
								}
								else {
									ImGui::Text("-");
								}
							}
							else {
								ImGui::Text("...");
							}
						}
						HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
					}
				}
			}
			if (LogProofs::players.size() == 0) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
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
			ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed, Settings::ColumnSizeKpmeId);
			for (std::string proof : *proofsArray) {
				ImGui::TableSetupColumn(proof.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			HighlightColumnOnHover();
			ImGui::Text("Id");
			for (std::string proof : *proofsArray) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
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
						if (!Settings::IncludeMissingAccounts && p.kpme.id.empty() && p.kpmeState == LogProofs::READY) {
							continue;
						}
						ImGui::TableNextColumn();
						DrawKpmeAccountName(p);
						ImGui::TableNextColumn();
						HighlightColumnOnHover();
						if (p.kpmeState == LogProofs::READY) {
							DrawKpmeId(p);
						}
						else {
							ImGui::Text("...");
						}
						for (std::string proof : *proofsArray) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							if (p.kpmeState == LogProofs::READY) {
								if (!p.kpme.id.empty()) {
									int amount = 0;
									if (Settings::IncludeLinkedAccounts) {
										if (p.kpme.shared.killproofs.contains(proof)) {
											amount = p.kpme.shared.killproofs.at(proof);
										}
										if (proof == "Legendary Insight") {
											if (p.kpme.shared.killproofs.contains("Legendary Divination")) {
												amount += p.kpme.shared.killproofs.at("Legendary Divination");
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
									}
									ImGui::Text("%i", amount);
								}
								else {
									ImGui::Text("-");
								}
							}
							else {
								ImGui::Text("...");
							}
							HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
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
			ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed, Settings::ColumnSizeKpmeId);
			for (Boss boss : *bossesArray) {
				ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			HighlightColumnOnHover();
			ImGui::Text("Id");
			for (Boss& boss : *bossesArray) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
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
						if (!Settings::IncludeMissingAccounts && p.kpme.id.empty() && p.kpmeState == LogProofs::READY) {
							continue;
						}
						ImGui::TableNextColumn();
						HighlightColumnOnHover();
						DrawKpmeAccountName(p);
						ImGui::TableNextColumn();
						HighlightColumnOnHover();
						if (p.kpmeState == LogProofs::READY) {
							DrawKpmeId(p);
						}
						else {
							ImGui::Text("...");
						}
						for (Boss boss : *bossesArray) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							if (p.kpmeState == LogProofs::READY) {
								if (!p.kpme.id.empty()) {
									int amount = 0;
									if (Settings::IncludeLinkedAccounts) {
										if (p.kpme.shared.tokens.contains(GetKpMeBossToken(boss))) {
											amount = p.kpme.shared.tokens.at(GetKpMeBossToken(boss));
										}
										if (p.kpme.shared.coffers.contains(GetKpMeBossCoffer(boss))) {
											amount += p.kpme.shared.coffers.at(GetKpMeBossCoffer(boss)) * 3;  // 1 Coffer == 3 Tokens
										}
										if (p.kpme.shared.tokens.contains(GetKpMeBossCoffer(boss))) {
											amount += p.kpme.shared.tokens.at(GetKpMeBossCoffer(boss)) * 3;  // Coffers show under tokens if opened
										}
									}
									if (amount == 0) {
										if (p.kpme.self.tokens.contains(GetKpMeBossToken(boss))) {
											amount = p.kpme.self.tokens.at(GetKpMeBossToken(boss));
										}
										if (p.kpme.self.coffers.contains(GetKpMeBossCoffer(boss))) {
											amount += p.kpme.self.coffers.at(GetKpMeBossCoffer(boss)) * 3;  // 1 Coffer == 3 Tokens
										}
										if (p.kpme.self.tokens.contains(GetKpMeBossCoffer(boss))) {
											amount += p.kpme.self.tokens.at(GetKpMeBossCoffer(boss)) * 3;  // Coffers show under tokens if opened
										}
									}
									ImGui::Text("%i", amount);
								}
								else {
									ImGui::Text("-");
								}
							}
							else {
								ImGui::Text("...");
							}
						}
						HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
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
			ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed, Settings::ColumnSizeKpmeId);
			for (Boss boss : *bossesArray) {
				ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			HighlightColumnOnHover();
			ImGui::Text("Id");
			for (Boss& boss : *bossesArray) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
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
						if (!Settings::IncludeMissingAccounts && p.kpme.id.empty() && p.kpmeState == LogProofs::READY) {
							continue;
						}
						ImGui::TableNextColumn();
						HighlightColumnOnHover();
						DrawKpmeAccountName(p);
						ImGui::TableNextColumn();
						HighlightColumnOnHover();
						if (p.kpmeState == LogProofs::READY) {
							DrawKpmeId(p);
						}
						else {
							ImGui::Text("...");
						}
						for (Boss boss : *bossesArray) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							if (p.kpmeState == LogProofs::READY) {
								if (!p.kpme.id.empty()) {
									int amount = 0;
									if (Settings::IncludeLinkedAccounts) {
										if (p.kpme.shared.coffers.contains(GetKpMeBossCoffer(boss))) {
											amount = p.kpme.shared.coffers.at(GetKpMeBossCoffer(boss));
										}
										if (p.kpme.shared.tokens.contains(GetKpMeBossCoffer(boss))) {
											amount = p.kpme.shared.tokens.at(GetKpMeBossCoffer(boss));  // Coffers show under tokens if opened
										}
									}
									if (amount == 0) {
										if (p.kpme.self.coffers.contains(GetKpMeBossCoffer(boss))) {
											amount = p.kpme.self.coffers.at(GetKpMeBossCoffer(boss));
										}
										if (p.kpme.self.tokens.contains(GetKpMeBossCoffer(boss))) {
											amount = p.kpme.self.tokens.at(GetKpMeBossCoffer(boss));  // Coffers show under tokens if opened
										}
									}
									ImGui::Text("%i", amount);
								}
								else {
									ImGui::Text("-");
								}
							}
							else {
								ImGui::Text("...");
							}
						}
						HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
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
		if (Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] != Settings::ShowWindowLogProofs)
		{
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
			Settings::Save(SettingsPath);
		}
		return;
	}
	ImGui::SetNextWindowSizeConstraints(ImVec2(Settings::MinWindowWidth, Settings::MinWindowHeight), ImVec2(Settings::MaxWindowWidth,Settings::MaxWindowHeight));
	if (ImGui::Begin("Log Proofs", &Settings::ShowWindowLogProofs, windowFlags)) {

		if (ImGui::BeginCombo("Source##DataSource", dataSources[Settings::SelectedDataSource].c_str())) {
			for (int index = 0; index < dataSources.size(); ++index) {
				const bool is_selected = (dataSources[index] == dataSources[Settings::SelectedDataSource]);
				if (ImGui::Selectable(dataSources[index].c_str(), is_selected)) {
					Settings::SelectedDataSource = DataSource(index);
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][SELECTED_DATA_SOURCE] = Settings::SelectedDataSource;
					Settings::Save(SettingsPath);
				}
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("Missing Accounts", &Settings::IncludeMissingAccounts)) {
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_MISSING_ACCOUNTS] = Settings::IncludeMissingAccounts;
			Settings::Save(SettingsPath);
		}

		if (Settings::SelectedDataSource == WINGMAN) {
			if (ImGui::BeginTabBar("##Wingman", ImGuiTabBarFlags_None)) {
				if (Settings::ShowTabRaidsNormal) {
					DrawBossesTab("Raids", "normalRaidsTable", &sortedRaidBosses, false);
				}
				if (Settings::ShowTabRaidsCM) {
					DrawBossesTab("Raid CMs", "cmRaidsTable", &sortedRaidCmBosses, false);
				}
				if (Settings::ShowTabRaidsLM) {
					DrawBossesTab("Raid LMs", "lmRaidsTable", &sortedRaidLmBosses, true);
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
		
		if (Settings::SelectedDataSource == KPME) {
			ImGui::SameLine();
			if (ImGui::Checkbox("Linked Accounts", &Settings::IncludeLinkedAccounts)) {
				Settings::Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_LINKED_ACCOUNTS] = Settings::IncludeLinkedAccounts;
				Settings::Save(SettingsPath);
			}
			if (ImGui::BeginTabBar("##Kpme", ImGuiTabBarFlags_None)) {
				if (Settings::ShowTabKpmeSummary) { 
					DrawKpmeSummaryTab("Summary", "kpmeSummaryTable", &sortedKpmeSummary);
				}
				if (Settings::ShowTabKpmeRaidTokens) {
					DrawKpmeTokensTab("Raid Tokens", "kpmeRaidsTable", &sortedKpmeRaidBosses);
				}
				if (Settings::ShowTabKpmeRaidCMCoffers) {
					DrawKpmeCoffersTab("Raid CM Coffers", "kpmeRaidCMsTable", &sortedKpmeRaidCMBosses);
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
	if (ImGui::Checkbox("Raid LMs", &Settings::ShowTabRaidsLM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_LM] = Settings::ShowTabRaidsLM;
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
	if (ImGui::Checkbox("Raid CM Coffers", &Settings::ShowTabKpmeRaidCMCoffers)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_RAID_CM_COFFERS] = Settings::ShowTabKpmeRaidCMCoffers;
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
	if (ImGui::SliderFloat("Kpme Id Size", &Settings::ColumnSizeKpmeId, 8.0f, 128.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_KPME_ID_SIZE] = Settings::ColumnSizeKpmeId;
		Settings::Save(SettingsPath);
	}
}

void DrawHoverOptions() {
	ImGui::Text("Hover");
	if (ImGui::Checkbox("Enabled##HoverEnabled", &Settings::hoverEnabled)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][HOVER_ENABLED] = Settings::hoverEnabled;
		Settings::Save(SettingsPath);
	}
	if (ImGui::ColorPicker4("HoverColour##HoverColourPicker", (float*)&Settings::hoverColourBuffer)) {
		Settings::hoverColour = ImGui::ColorConvertFloat4ToU32(Settings::hoverColourBuffer);
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][HOVER_COLOUR] = Settings::hoverColour;
		Settings::Save(SettingsPath);
	}
}

void RegisterQuickAccessShortcut() {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "registering quick access shortcut");
	APIDefs->QuickAccess.Add("SHORTCUT_LOG_PROOFS", "TEX_LOG_NORMAL", "TEX_LOG_HOVER", KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS, "Toggle Log Proofs Window");
}

void DeregisterQuickAccessShortcut() {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "deregistering quick access shortcut");
	APIDefs->QuickAccess.Remove("SHORTCUT_LOG_PROOFS");
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
	DrawColumnOptions();
	DrawTabsOptions();
	DrawHoverOptions();
}

