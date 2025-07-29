#include <algorithm>
#include <format>
#include <thread>

#include "../core/bosses.h"
#include "../core/log_proofs.h"
#include "../core/settings.h"
#include "../core/shared.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../providers/common/provider_registry.h"
#include "imgui_extensions.h"
#include "ui_renderer.h"

static ImGuiWindowFlags windowFlags = (ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_AlwaysAutoResize);
static ImGuiTableFlags tableFlags = (ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY);

std::vector<std::string> GetDataSources() {
	return ProviderRegistry::Instance().GetAvailableProviders();
}

void DrawKpmeId(const LogProofs::Player& aPlayer) {
	if (aPlayer.proofData && !aPlayer.proofData->profileId.empty()) {
		if (ImGui::TextURL(aPlayer.proofData->profileId.c_str())) {
			ImGui::SetClipboardText(aPlayer.proofData->profileId.c_str());
		}
	} else {
		ImGui::Text("-");
	}
}

void HighlightColumnOnHover() {
	if (!Settings::hoverEnabled)
		return;
	if (ImGui::TableGetColumnFlags(ImGui::TableGetColumnIndex()) & ImGuiTableColumnFlags_IsHovered) {
		ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, Settings::hoverColour);
	}
}

void HighlightRowOnHover(ImGuiTable* table) {
	if (!Settings::hoverEnabled)
		return;
	ImRect rowRect(
			table->WorkRect.Min.x,
			table->RowPosY1,
			table->WorkRect.Max.x,
			table->RowPosY2
	);
	rowRect.ClipWith(table->BgClipRect);

	bool rowHovered = (ImGui::IsMouseHoveringRect(rowRect.Min, rowRect.Max, false) && ImGui::IsWindowHovered(ImGuiHoveredFlags_None) && !ImGui::IsAnyItemHovered());

	if (rowHovered) {
		table->RowBgColor[1] = ImGui::GetColorU32(Settings::hoverColour);
	}
}

void DrawKpmeSummaryTab(const char* tabName, const char* tableName, std::vector<std::string>* proofsArray) {
	if (ImGui::BeginTabItem(tabName)) {
		if (ImGui::BeginTable(tableName, int(proofsArray->size()) + 2, tableFlags)) {
			ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
			ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed, Settings::ColumnSizeKpmeId);
			for (const std::string& proof : *proofsArray) {
				ImGui::TableSetupColumn(proof.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			ImGui::Text("Id");
			for (const std::string& proof : *proofsArray) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
				Texture* texture = GetCurrencyTexture(proof);
				if (texture != nullptr) {
					ImGui::Image((void*) texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
				} else {
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
					for (const LogProofs::Player& p : LogProofs::players) {
						if (!Settings::IncludeMissingAccounts && p.state == ::LoadState::READY && (!p.proofData || p.proofData->profileId.empty())) {
							continue;
						}
						ImGui::TableNextColumn();
						UIRenderer::DrawPlayerAccountName(p);
						ImGui::TableNextColumn();
						HighlightColumnOnHover();
						DrawKpmeId(p);
						for (const std::string& proof : *proofsArray) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							UIRenderer::DrawPlayerProofValue(p, proof);
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

void DrawBossesTab(const char* tabName, const char* tableName, std::vector<Boss>* bossesArray, bool isLegendary) {
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
					ImGui::Image((void*) texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
				} else {
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
					for (const LogProofs::Player& p : LogProofs::players) {
						if (!Settings::IncludeMissingAccounts && p.state == ::LoadState::READY && (!p.proofData || p.proofData->accountName.empty())) {
							continue;
						}
						ImGui::TableNextColumn();
						UIRenderer::DrawPlayerAccountName(p);
						for (Boss& boss : *bossesArray) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							std::string proofId = isLegendary ? std::format("-{}", int(boss)) : std::format("{}", int(boss));
							UIRenderer::DrawPlayerProofValue(p, proofId);
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

void DrawKpmeTokensTab(const char* tabName, const char* tableName, std::vector<Boss>* bossesArray) {
	if (ImGui::BeginTabItem(tabName)) {
		if (ImGui::BeginTable(tableName, int(bossesArray->size()) + 2, tableFlags)) {
			ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
			ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed, Settings::ColumnSizeKpmeId);
			for (const Boss& boss : *bossesArray) {
				ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			ImGui::Text("Id");
			for (const Boss& boss : *bossesArray) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
				Texture* texture = GetBossTexture(boss);
				if (texture != nullptr) {
					ImGui::Image((void*) texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
				} else {
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
					for (const LogProofs::Player& p : LogProofs::players) {
						if (!Settings::IncludeMissingAccounts && p.state == ::LoadState::READY && (!p.proofData || p.proofData->profileId.empty())) {
							continue;
						}
						ImGui::TableNextColumn();
						UIRenderer::DrawPlayerAccountName(p);
						ImGui::TableNextColumn();
						HighlightColumnOnHover();
						DrawKpmeId(p);
						for (const Boss& boss : *bossesArray) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							std::string proofId = GetKpMeBossToken(boss);
							UIRenderer::DrawPlayerProofValue(p, proofId);
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
			for (const Boss& boss : *bossesArray) {
				ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
			}
			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			ImGui::TableNextColumn();
			ImGui::Text("Id");
			for (const Boss& boss : *bossesArray) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
				Texture* texture = GetBossTexture(boss);
				if (texture != nullptr) {
					ImGui::Image((void*) texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
				} else {
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
					for (const LogProofs::Player& p : LogProofs::players) {
						if (!Settings::IncludeMissingAccounts && p.state == ::LoadState::READY && (!p.proofData || p.proofData->profileId.empty())) {
							continue;
						}
						ImGui::TableNextColumn();
						UIRenderer::DrawPlayerAccountName(p);
						ImGui::TableNextColumn();
						HighlightColumnOnHover();
						DrawKpmeId(p);
						for (const Boss& boss : *bossesArray) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							std::string proofId = GetKpMeBossCoffer(boss);
							UIRenderer::DrawPlayerProofValue(p, proofId);
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
		if (Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] != Settings::ShowWindowLogProofs) {
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
			Settings::Save(SettingsPath);
		}
		return;
	}
	ImGui::SetNextWindowSizeConstraints(ImVec2(Settings::MinWindowWidth, Settings::MinWindowHeight), ImVec2(Settings::MaxWindowWidth, Settings::MaxWindowHeight));
	if (ImGui::Begin("Log Proofs", &Settings::ShowWindowLogProofs, windowFlags)) {

		auto dataSources = GetDataSources();
		std::string currentProvider = (Settings::SelectedDataSource == WINGMAN) ? "Wingman" : "KPME";
		if (ImGui::BeginCombo("Source##DataSource", currentProvider.c_str())) {
			for (const auto& provider : dataSources) {
				const bool is_selected = (provider == currentProvider);
				if (ImGui::Selectable(provider.c_str(), is_selected)) {
					Settings::SelectedDataSource = (provider == "Wingman") ? WINGMAN : KPME;
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][SELECTED_DATA_SOURCE] = Settings::SelectedDataSource;
					Settings::Save(SettingsPath);
					// Reload all players with new provider
					LogProofs::ReloadAllPlayersWithProvider(provider);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
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
					DrawBossesTab("Raid LMs", "lmRaidsTable", &sortedRaidLMBosses, true);
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
				// Reload KPME players to apply linked accounts setting
				LogProofs::ReloadKpmePlayersForLinkedAccounts();
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