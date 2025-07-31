#include <algorithm>
#include <format>
#include <thread>

#include "../core/boss_registry.h"
#include "../core/bosses.h"
#include "../core/log_proofs.h"
#include "../core/settings.h"
#include "../core/shared.h"
#include "../core/tab_config.h"
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

static void SetupTableColumns(const BossGroup& group, bool showKpmeId) {
	ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
	if (showKpmeId) {
		ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed, Settings::ColumnSizeKpmeId);
	}
	for (const auto& currency : group.currencies) {
		ImGui::TableSetupColumn(currency.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
	}
	for (const auto& boss : group.bosses) {
		ImGui::TableSetupColumn(GetBossName(boss), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
	}
}

static void DrawTableHeaders(const BossGroup& group, bool showKpmeId) {
	ImGui::TableSetupScrollFreeze(1, 1);
	ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
	ImGui::TableNextColumn();
	ImGui::Text("Account");
	if (showKpmeId) {
		ImGui::TableNextColumn();
		ImGui::Text("Id");
	}
	for (const auto& currency : group.currencies) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		Texture* texture = GetCurrencyTexture(currency);
		if (texture) {
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
	for (const auto& boss : group.bosses) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		Texture* texture = GetBossTexture(boss);
		if (texture) {
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

static void DrawPlayerRow(const LogProofs::Player& p, const BossGroup& group, IBossProvider* provider, bool showKpmeId) {
	ImGui::TableNextColumn();
	DrawPlayerAccountName(p);
	if (showKpmeId) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		DrawKpmeId(p);
	}
	for (const auto& currency : group.currencies) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		DrawPlayerProofValue(p, provider->GetProofIdentifier(currency));
	}
	for (const auto& boss : group.bosses) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		DrawPlayerProofValue(p, provider->GetProofIdentifier(boss, group.category));
	}
	HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
}

static void DrawGenericTab(const BossGroup& group, IBossProvider* provider, bool showKpmeId = false) {
	if (!ImGui::BeginTabItem(group.name.c_str())) return;
	int columnCount = static_cast<int>(group.currencies.size() + group.bosses.size()) + (showKpmeId ? 2 : 1);
	if (ImGui::BeginTable(group.tableName.c_str(), columnCount, tableFlags)) {
		SetupTableColumns(group, showKpmeId);
		DrawTableHeaders(group, showKpmeId);
		std::scoped_lock lck(LogProofs::Mutex);
		if (LogProofs::players.empty()) {
			ImGui::TableNextColumn();
			ImGui::Text("No players found... ");
		} else {
			for (const auto& p : LogProofs::players) {
				if (!Settings::IncludeMissingAccounts && p.state == LoadState::READY && (!p.proofData || (showKpmeId ? p.proofData->profileId.empty() : p.proofData->accountName.empty()))) {
					continue;
				}
				DrawPlayerRow(p, group, provider, showKpmeId);
			}
		}
		ImGui::EndTable();
	}
	ImGui::EndTabItem();
}


static void SaveWindowState() {
	Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
	Settings::Save(SettingsPath);
}

static void DrawProviderCombo(const std::string& currentProvider) {
	if (ImGui::BeginCombo("Source##DataSource", currentProvider.c_str())) {
		for (const auto& provider : GetDataSources()) {
			bool is_selected = (provider == currentProvider);
			if (ImGui::Selectable(provider.c_str(), is_selected)) {
				Settings::SelectedDataSource = (provider == "Wingman") ? WINGMAN : KPME;
				Settings::Settings[WINDOW_LOG_PROOFS_KEY][SELECTED_DATA_SOURCE] = Settings::SelectedDataSource;
				Settings::Save(SettingsPath);
				LogProofs::ReloadAllPlayersWithProvider(provider);
			}
			if (is_selected) ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

static void DrawControls(const std::string& currentProvider) {
	DrawProviderCombo(currentProvider);
	ImGui::SameLine();
	if (ImGui::Checkbox("Missing Accounts", &Settings::IncludeMissingAccounts)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_MISSING_ACCOUNTS] = Settings::IncludeMissingAccounts;
		Settings::Save(SettingsPath);
	}
	if (currentProvider == "KPME") {
		ImGui::SameLine();
		if (ImGui::Checkbox("Linked Accounts", &Settings::IncludeLinkedAccounts)) {
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_LINKED_ACCOUNTS] = Settings::IncludeLinkedAccounts;
			Settings::Save(SettingsPath);
			LogProofs::ReloadKpmePlayersForLinkedAccounts();
		}
	}
}

void RenderWindowLogProofs() {
	if (!Settings::ShowWindowLogProofs) {
		SaveWindowState();
		return;
	}
	ImGui::SetNextWindowSizeConstraints(ImVec2(Settings::MinWindowWidth, Settings::MinWindowHeight), ImVec2(Settings::MaxWindowWidth, Settings::MaxWindowHeight));
	if (!ImGui::Begin("Log Proofs", &Settings::ShowWindowLogProofs, windowFlags)) {
		ImGui::End();
		return;
	}
	std::string currentProvider = (Settings::SelectedDataSource == WINGMAN) ? "Wingman" : "KPME";
	DrawControls(currentProvider);
	IBossProvider* bossProvider = BossRegistry::Instance().GetProvider(currentProvider);
	if (bossProvider && ImGui::BeginTabBar(("##" + currentProvider).c_str(), ImGuiTabBarFlags_None)) {
		bool isKpme = (currentProvider == "KPME");

		// Ensure provider configuration exists
		Settings::EnsureProviderConfigExists(currentProvider);
		auto config = TabConfigManager::Instance().GetProviderConfig(currentProvider);

		if (config.useCustomTabs && !config.tabs.empty()) {
			// Render custom tabs
			for (const auto& customTab : config.tabs) {
				if (customTab.visible && !customTab.displayName.empty()) {
					try {
						auto bossGroup = bossProvider->CreateCustomBossGroup(customTab);
						// Only render if the boss group has valid content
						if (!bossGroup.bosses.empty() || !bossGroup.currencies.empty()) {
							DrawGenericTab(bossGroup, bossProvider, isKpme);
						}
					} catch (...) {
						// Skip invalid tabs silently
						continue;
					}
				}
			}
		} else {
			// Render default tabs
			for (const auto& group : bossProvider->GetBossGroups()) {
				DrawGenericTab(group, bossProvider, isKpme);
			}
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
	if (!Settings::ShowWindowLogProofs) SaveWindowState();
}