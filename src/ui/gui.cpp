#include <algorithm>
#include <format>
#include <thread>

#include "../core/boss_registry.h"
#include "../core/bosses.h"
#include "../core/log_proofs.h"
#include "../core/settings.h"
#include "../core/shared.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../providers/common/provider_registry.h"
#include "imgui_extensions.h"
#include <Windows.h>


static ImGuiWindowFlags windowFlags = (ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_AlwaysAutoResize);
static ImGuiTableFlags tableFlags = (ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY);

static std::vector<std::string> GetDataSources() {
	return BossRegistry::Instance().GetAvailableProviders();
}

static void DrawPlayerAccountName(const LogProofs::Player& player) {
	if (player.proofData && !player.proofData->profileUrl.empty()) {
		if (ImGui::TextURL(player.account.c_str())) {
			ShellExecuteA(0, 0, player.proofData->profileUrl.c_str(), 0, 0, SW_SHOW);
		}
	} else {
		ImGui::Text(player.account.c_str());
	}
}

static void DrawPlayerProofValue(const LogProofs::Player& player, const std::string& proofId) {
	if (player.state == LoadState::READY && player.proofData) {
		auto it = player.proofData->proofs.find(proofId);
		if (it != player.proofData->proofs.end()) {
			ImGui::Text("%i", it->second.amount);
		} else {
			ImGui::Text("-");
		}
	} else if (player.state == LoadState::LOADING) {
		ImGui::Text("...");
	} else {
		ImGui::Text("-");
	}
}

static void DrawKpmeId(const LogProofs::Player& aPlayer) {
	if (aPlayer.proofData && !aPlayer.proofData->profileId.empty()) {
		if (ImGui::TextURL(aPlayer.proofData->profileId.c_str())) {
			ImGui::SetClipboardText(aPlayer.proofData->profileId.c_str());
		}
	} else {
		ImGui::Text("-");
	}
}

static void HighlightColumnOnHover() {
	if (!Settings::hoverEnabled)
		return;
	if (ImGui::TableGetColumnFlags(ImGui::TableGetColumnIndex()) & ImGuiTableColumnFlags_IsHovered) {
		ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, Settings::hoverColour);
	}
}

static void HighlightRowOnHover(ImGuiTable* table) {
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

static void DrawGenericTab(const BossGroup& group, IBossProvider* provider, bool showKpmeId = false) {
	if (ImGui::BeginTabItem(group.name.c_str())) {
		int columnCount = static_cast<int>((group.category == BossCategory::SUMMARY ? group.currencies.size() : group.bosses.size())) + (showKpmeId ? 2 : 1);
		if (ImGui::BeginTable(group.tableName.c_str(), columnCount, tableFlags)) {
			ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
			if (showKpmeId) {
				ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed, Settings::ColumnSizeKpmeId);
			}

			if (group.category == BossCategory::SUMMARY) {
				for (const std::string& currency : group.currencies) {
					ImGui::TableSetupColumn(currency.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
				}
			} else {
				for (const Boss& boss : group.bosses) {
					ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
				}
			}

			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableNextColumn();
			ImGui::Text("Account");
			if (showKpmeId) {
				ImGui::TableNextColumn();
				ImGui::Text("Id");
			}

			if (group.category == BossCategory::SUMMARY) {
				for (const std::string& currency : group.currencies) {
					ImGui::TableNextColumn();
					HighlightColumnOnHover();
					Texture* texture = GetCurrencyTexture(currency);
					if (texture != nullptr) {
						ImGui::Image((void*) texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
					} else {
						ImGui::Text(currency.c_str());
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text(currency.c_str());
						ImGui::EndTooltip();
					}
				}
			} else {
				for (const Boss& boss : group.bosses) {
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
			}

			{
				std::scoped_lock lck(LogProofs::Mutex);
				if (LogProofs::players.size() > 0) {
					for (const LogProofs::Player& p : LogProofs::players) {
						if (!Settings::IncludeMissingAccounts && p.state == ::LoadState::READY && (!p.proofData || (showKpmeId ? p.proofData->profileId.empty() : p.proofData->accountName.empty()))) {
							continue;
						}
						ImGui::TableNextColumn();
						DrawPlayerAccountName(p);
						if (showKpmeId) {
							ImGui::TableNextColumn();
							HighlightColumnOnHover();
							DrawKpmeId(p);
						}

						if (group.category == BossCategory::SUMMARY) {
							for (const std::string& currency : group.currencies) {
								ImGui::TableNextColumn();
								HighlightColumnOnHover();
								std::string proofId = provider->GetProofIdentifier(currency);
								DrawPlayerProofValue(p, proofId);
							}
						} else {
							for (const Boss& boss : group.bosses) {
								ImGui::TableNextColumn();
								HighlightColumnOnHover();
								std::string proofId = provider->GetProofIdentifier(boss, group.category);
								DrawPlayerProofValue(p, proofId);
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

		IBossProvider* bossProvider = BossRegistry::Instance().GetProvider(currentProvider);
		if (bossProvider) {
			auto bossGroups = bossProvider->GetBossGroups();
			bool isKpme = (currentProvider == "KPME");

			if (isKpme) {
				ImGui::SameLine();
				if (ImGui::Checkbox("Linked Accounts", &Settings::IncludeLinkedAccounts)) {
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_LINKED_ACCOUNTS] = Settings::IncludeLinkedAccounts;
					Settings::Save(SettingsPath);
					LogProofs::ReloadKpmePlayersForLinkedAccounts();
				}
			}

			if (ImGui::BeginTabBar(("##" + currentProvider).c_str(), ImGuiTabBarFlags_None)) {
				for (const auto& group : bossGroups) {
					DrawGenericTab(group, bossProvider, isKpme);
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