#include <algorithm>
#include <format>
#include <thread>

#include "../core/boss_registry.h"
#include "../core/bosses.h"
#include "../core/data_loader.h"
#include "../core/player_manager.h"
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

static void DrawPlayerAccountName(const Player& player, const auto* proofData) {
	if (proofData && !proofData->profileUrl.empty()) {
		if (ImGui::TextURL(player.account.c_str())) {
			ShellExecuteA(0, 0, proofData->profileUrl.c_str(), 0, 0, SW_SHOW);
		}
	} else {
		ImGui::Text(player.account.c_str());
	}
}

static void DrawPlayerProofValue(const Player& player, const std::string& proofId, const std::string& providerName) {
	if (player.state == LoadState::READY && player.proofData) {
		auto it = player.proofData->proofs.find(proofId);
		ImGui::Text(it != player.proofData->proofs.end() ? std::to_string(it->second.amount).c_str() : "0");
	} else {
		ImGui::Text(player.state == LoadState::LOADING ? "..." : "0");
	}
}

static void DrawKpmeId(const Player& aPlayer, const auto* proofData) {
	if (proofData && !proofData->profileId.empty()) {
		if (ImGui::TextURL(proofData->profileId.c_str())) {
			ImGui::SetClipboardText(proofData->profileId.c_str());
		}
	} else {
		ImGui::Text("0");
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
	for (const auto& bossEntry : group.bosses) {
		ImGui::TableSetupColumn(GetBossName(bossEntry.boss, bossEntry.type).c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
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
	for (const auto& bossEntry : group.bosses) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		Texture* texture = GetBossTexture(bossEntry.boss);
		if (texture) {
			ImGui::Image((void*) texture->Resource, ImVec2(Settings::ColumnSizeBosses, Settings::ColumnSizeBosses));
		} else {
			ImGui::Text(GetBossName(bossEntry.boss, bossEntry.type).c_str());
		}
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text(GetBossName(bossEntry.boss, bossEntry.type).c_str());
			ImGui::EndTooltip();
		}
	}
}

static void DrawPlayerRow(const Player& p, const BossGroup& group, IBossProvider* provider, bool showKpmeId, const std::string& providerName) {
	// Cache lazy loading data once per row
	auto lazyState = PlayerManager::lazyLoadManager.GetPlayerState(p.account, providerName);
	auto lazyData = PlayerManager::lazyLoadManager.GetPlayerData(p.account, providerName);
	const auto* proofData = (lazyState == LoadState::READY && lazyData) ? lazyData.get() : nullptr;

	if (!proofData || proofData->proofs.empty()) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
	}

	ImGui::TableNextColumn();
	DrawPlayerAccountName(p, proofData);
	if (showKpmeId) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		DrawKpmeId(p, proofData);
	}

	bool isDisabled = !proofData || proofData->proofs.empty();

	for (const auto& currency : group.currencies) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		if (proofData) {
			auto it = proofData->proofs.find(provider->GetProofIdentifier(currency));
			ImGui::Text(it != proofData->proofs.end() ? std::to_string(it->second.amount).c_str() : (isDisabled ? "" : "0"));
		} else {
			ImGui::Text(lazyState == LoadState::LOADING ? "..." : (isDisabled ? "" : "0"));
		}
	}
	for (const auto& bossEntry : group.bosses) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		if (proofData) {
			auto it = proofData->proofs.find(provider->GetProofIdentifier(bossEntry.boss, group.category));
			ImGui::Text(it != proofData->proofs.end() ? std::to_string(it->second.amount).c_str() : (isDisabled ? "" : "0"));
		} else {
			ImGui::Text(lazyState == LoadState::LOADING ? "..." : (isDisabled ? "" : "0"));
		}
	}

	if (!proofData || proofData->proofs.empty()) {
		ImGui::PopStyleColor();
	}

	HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
}

static void DrawGenericTab(const BossGroup& group, IBossProvider* provider, const std::string& providerName, bool showKpmeId = false) {
	if (!ImGui::BeginTabItem(group.name.c_str())) return;
	int columnCount = static_cast<int>(group.currencies.size() + group.bosses.size()) + (showKpmeId ? 2 : 1);
	if (ImGui::BeginTable(group.tableName.c_str(), columnCount, tableFlags)) {
		SetupTableColumns(group, showKpmeId);
		DrawTableHeaders(group, showKpmeId);
		std::scoped_lock lck(PlayerManager::playerMutex);
		if (PlayerManager::players.empty()) {
			ImGui::TableNextColumn();
			ImGui::Text("No players found... ");
		} else {
			// Show all players
			static std::vector<const Player*> visiblePlayers;
			visiblePlayers.clear();
			for (const auto& p : PlayerManager::players) {
				visiblePlayers.push_back(&p);
			}

			// Use clipper for virtualization
			ImGuiListClipper clipper;
			clipper.Begin(static_cast<int>(visiblePlayers.size()));
			while (clipper.Step()) {
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
					DrawPlayerRow(*visiblePlayers[i], group, provider, showKpmeId, providerName);
				}
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
	if (ImGui::BeginCombo("##DataSource", currentProvider.c_str())) {
		for (const auto& provider : GetDataSources()) {
			bool is_selected = (provider == currentProvider);
			if (ImGui::Selectable(provider.c_str(), is_selected)) {
				Settings::SelectedDataSource = (provider == "Wingman") ? WINGMAN : KPME;
				Settings::Settings[WINDOW_LOG_PROOFS_KEY][SELECTED_DATA_SOURCE] = Settings::SelectedDataSource;
				Settings::Save(SettingsPath);
				// Trigger lazy loading for all players with the new provider
				std::scoped_lock lck(PlayerManager::playerMutex);
				for (const auto& player : PlayerManager::players) {
					PlayerManager::lazyLoadManager.RequestPlayerData(player.account, provider);
				}
			}
			if (is_selected) ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

static void DrawControls(const std::string& currentProvider) {
	DrawProviderCombo(currentProvider);
	if (currentProvider == "KPME") {
		ImGui::SameLine();
		if (ImGui::Checkbox("Linked Accounts", &Settings::IncludeLinkedAccounts)) {
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_LINKED_ACCOUNTS] = Settings::IncludeLinkedAccounts;
			Settings::Save(SettingsPath);
			// Clear KPME cache and reload data with new linked accounts setting
			PlayerManager::lazyLoadManager.ClearProviderCache("KPME");
			std::scoped_lock lck(PlayerManager::playerMutex);
			for (const auto& player : PlayerManager::players) {
				PlayerManager::lazyLoadManager.RequestPlayerData(player.account, "KPME");
			}
		}
	}
}

void RenderWindowLogProofs() {
	static bool wasWindowOpen = false;
	bool isWindowOpen = Settings::ShowWindowLogProofs;

	// Detect window state changes
	if (isWindowOpen != wasWindowOpen) {
		PlayerManager::OnWindowStateChanged(isWindowOpen);
		wasWindowOpen = isWindowOpen;
	}

	// Process pending loads when window is open
	if (isWindowOpen) {
		PlayerManager::ProcessPendingLoads();
	}

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

		if (config.useCustomTabs) {
			// Render custom tabs only
			for (const auto& customTab : config.tabs) {
				if (customTab.visible && !customTab.displayName.empty()) {
					try {
						auto bossGroup = bossProvider->CreateCustomBossGroup(customTab);
						// Only render if the boss group has valid content
						if (!bossGroup.bosses.empty() || !bossGroup.currencies.empty()) {
							DrawGenericTab(bossGroup, bossProvider, currentProvider, isKpme);
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
				DrawGenericTab(group, bossProvider, currentProvider, isKpme);
			}
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
	if (!Settings::ShowWindowLogProofs) SaveWindowState();
}