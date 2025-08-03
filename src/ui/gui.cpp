#include <algorithm>
#include <format>
#include <functional>
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


#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_extensions.h"
#include <Windows.h>


static ImGuiWindowFlags windowFlags = (ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_AlwaysAutoResize);
static ImGuiTableFlags tableFlags = (ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY);

static std::vector<std::string> GetDataSources() {
	return BossRegistry::Instance().GetAvailableProviders();
}

namespace {
	static float bezier_ease(float t) {
		// Fast out slow in cubic bezier approximation
		return t * t * (3.0f - 2.0f * t);
	}

	static float lerp(float x0, float x1, float t) {
		return (1.0f - t) * x0 + t * x1;
	}

	static float sawtooth(float t, int periods) {
		return ImFmod(((float) periods) * t, 1.0f);
	}

	static float interval(float t, float t0, float t1) {
		return t < t0 ? 0.0f : t > t1 ? 1.0f
									  : bezier_ease((t - t0) / (t1 - t0));
	}
} // namespace

static void DrawSpinner() {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems) return;

	ImGuiContext& g = *ImGui::GetCurrentContext();
	const ImGuiStyle& style = g.Style;

	float radius = 6.0f;
	int thickness = 2;
	ImU32 color = ImGui::GetColorU32(ImGuiCol_Text);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size(radius * 2, radius * 2);
	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb);

	const ImVec2 center = ImVec2(pos.x + radius, pos.y + radius);
	const float period = 8.0f;
	const float t = ImFmod((float) g.Time, period) / period;

	const int num_detents = 5;
	const float t_saw = sawtooth(t, num_detents);
	const float head_value = interval(t_saw, 0.0f, 0.5f);
	const float tail_value = interval(t_saw, 0.5f, 1.0f);
	const float rotation_value = sawtooth(t, num_detents);

	const float min_arc = 30.0f / 360.0f * 2.0f * IM_PI;
	const float max_arc = 270.0f / 360.0f * 2.0f * IM_PI;
	const float start_angle = -IM_PI / 2.0f;

	const float a_min = start_angle + tail_value * max_arc + rotation_value * 2.0f * IM_PI;
	const float a_max = a_min + (head_value - tail_value) * max_arc + min_arc;

	window->DrawList->PathClear();
	for (int i = 0; i < 24; i++) {
		const float a = a_min + ((float) i / 24.0f) * (a_max - a_min);
		window->DrawList->PathLineTo(ImVec2(center.x + ImCos(a) * radius, center.y + ImSin(a) * radius));
	}
	window->DrawList->PathStroke(color, false, (float) thickness);
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
		if (player.state == LoadState::LOADING) {
			DrawSpinner();
		} else {
			ImGui::Text("0");
		}
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
	static std::vector<std::pair<std::string, float>> columnSpecs;
	columnSpecs.clear();

	columnSpecs.emplace_back("Account", Settings::ColumnSizeAccount);
	if (showKpmeId) {
		columnSpecs.emplace_back("Id", Settings::ColumnSizeKpmeId);
	}
	for (const auto& currency : group.currencies) {
		columnSpecs.emplace_back(currency, Settings::ColumnSizeBosses);
	}
	for (const auto& bossEntry : group.bosses) {
		columnSpecs.emplace_back(GetBossName(bossEntry.boss, bossEntry.type), Settings::ColumnSizeBosses);
	}

	// Setup all columns at once
	for (size_t i = 0; i < columnSpecs.size(); ++i) {
		ImGuiTableColumnFlags flags = ImGuiTableColumnFlags_WidthFixed;
		if (i == 0) flags |= ImGuiTableColumnFlags_NoHide; // Account column
		else if (i > (showKpmeId ? 1 : 0))
			flags |= ImGuiTableColumnFlags_NoResize; // Boss/currency columns
		ImGui::TableSetupColumn(columnSpecs[i].first.c_str(), flags, columnSpecs[i].second);
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
	// Initialize cached proof identifiers
	group.InitializeCache(provider);

	// Cache lazy loading data once per row
	auto lazyState = PlayerManager::lazyLoadManager.GetPlayerState(p.account, providerName);
	auto lazyData = PlayerManager::lazyLoadManager.GetPlayerData(p.account, providerName);
	const auto* rawProofData = (lazyState == LoadState::READY && lazyData) ? lazyData.get() : nullptr;

	// For providers that support linked accounts, compute proofs dynamically
	std::unique_ptr<PlayerProofData> computedData;
	if (rawProofData && rawProofData->rawData.has_value()) {
		auto dataProvider = ProviderRegistry::Instance().CreateProvider(providerName);
		if (dataProvider && dataProvider->SupportsLinkedAccounts()) {
			// Determine if linked accounts should be included in main computation
			bool includeLinked = (Settings::LinkedAccountsMode == COMBINE_LINKED);
			computedData = std::make_unique<PlayerProofData>(dataProvider->ComputeProofsFromRawData(*rawProofData, includeLinked));
		}
	}
	const auto* proofData = computedData ? computedData.get() : rawProofData;

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

	// Use cached currency identifiers
	for (size_t i = 0; i < group.cachedCurrencyIds.size(); ++i) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		if (proofData) {
			auto it = proofData->proofs.find(group.cachedCurrencyIds[i]);
			ImGui::Text(it != proofData->proofs.end() ? std::to_string(it->second.amount).c_str() : (isDisabled ? "" : "0"));
		} else {
			if (lazyState == LoadState::LOADING) {
				DrawSpinner();
			} else {
				ImGui::Text(isDisabled ? "" : "0");
			}
		}
	}
	// Use cached boss identifiers
	for (size_t i = 0; i < group.cachedBossIds.size(); ++i) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		if (proofData) {
			auto it = proofData->proofs.find(group.cachedBossIds[i]);
			ImGui::Text(it != proofData->proofs.end() ? std::to_string(it->second.amount).c_str() : (isDisabled ? "" : "0"));
		} else {
			if (lazyState == LoadState::LOADING) {
				DrawSpinner();
			} else {
				ImGui::Text(isDisabled ? "" : "0");
			}
		}
	}

	if (!proofData || proofData->proofs.empty()) {
		ImGui::PopStyleColor();
	}

	HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);

	// Draw linked accounts separately if enabled
	if (Settings::LinkedAccountsMode == SPLIT_LINKED && proofData && !proofData->linkedAccounts.empty()) {
		for (const auto& linkedAccount : proofData->linkedAccounts) {
			ImGui::TableNextColumn();
			ImGui::Indent(20.0f);
			ImGui::Text(linkedAccount.accountName.c_str());
			ImGui::Unindent(20.0f);

			if (showKpmeId) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
				ImGui::Text("-");
			}

			// Use cached currency identifiers for linked accounts
			for (size_t i = 0; i < group.cachedCurrencyIds.size(); ++i) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
				auto it = linkedAccount.proofs.find(group.cachedCurrencyIds[i]);
				ImGui::Text(it != linkedAccount.proofs.end() ? std::to_string(it->second.amount).c_str() : "0");
			}

			// Use cached boss identifiers for linked accounts
			for (size_t i = 0; i < group.cachedBossIds.size(); ++i) {
				ImGui::TableNextColumn();
				HighlightColumnOnHover();
				auto it = linkedAccount.proofs.find(group.cachedBossIds[i]);
				ImGui::Text(it != linkedAccount.proofs.end() ? std::to_string(it->second.amount).c_str() : "0");
			}

			HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
		}
	}
}

static void DrawGenericTab(const BossGroup& group, IBossProvider* provider, const std::string& providerName, bool showKpmeId = false) {
	if (!ImGui::BeginTabItem(group.name.c_str())) return;
	int columnCount = static_cast<int>(group.currencies.size() + group.bosses.size()) + (showKpmeId ? 2 : 1);
	if (ImGui::BeginTable(group.tableName.c_str(), columnCount, tableFlags)) {
		SetupTableColumns(group, showKpmeId);
		DrawTableHeaders(group, showKpmeId);

		static std::vector<const Player*> visiblePlayers;
		visiblePlayers.clear();

		{
			std::scoped_lock lck(PlayerManager::playerMutex);
			if (PlayerManager::players.empty()) {
				ImGui::TableNextColumn();
				ImGui::Text("No players found... ");
				ImGui::EndTable();
				ImGui::EndTabItem();
				return;
			}

			// Filter players while holding lock
			for (const auto& p : PlayerManager::players) {
				if (!Settings::IncludeMissingAccounts) {
					auto lazyState = PlayerManager::lazyLoadManager.GetPlayerState(p.account, providerName);
					auto lazyData = PlayerManager::lazyLoadManager.GetPlayerData(p.account, providerName);
					if (lazyState == LoadState::READY && (!lazyData || lazyData->proofs.empty())) {
						continue;
					}
				}
				visiblePlayers.push_back(&p);
			}
		}

		// Render all players without clipping
		for (const auto* player : visiblePlayers) {
			DrawPlayerRow(*player, group, provider, showKpmeId, providerName);
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
		const char* modeNames[] = {"Hide linked accounts", "Combine linked accounts", "Split linked accounts"};
		if (ImGui::BeginCombo("##LinkedMode", modeNames[Settings::LinkedAccountsMode])) {
			for (int i = 0; i < 3; i++) {
				bool is_selected = (Settings::LinkedAccountsMode == i);
				if (ImGui::Selectable(modeNames[i], is_selected)) {
					Settings::LinkedAccountsMode = static_cast<LinkedAccountMode>(i);
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][LINKED_ACCOUNTS_MODE] = Settings::LinkedAccountsMode;
					Settings::Save(SettingsPath);
				}
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
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
	ImGuiWindowFlags flags = windowFlags;
	if (Settings::WindowAutoResize) {
		flags |= ImGuiWindowFlags_AlwaysAutoResize;
	} else {
		flags &= ~ImGuiWindowFlags_AlwaysAutoResize;
	}
	if (Settings::WindowRestrictSize) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(Settings::MinWindowWidth, Settings::MinWindowHeight), ImVec2(Settings::MaxWindowWidth, Settings::MaxWindowHeight));
	}
	if (!ImGui::Begin("Log Proofs", &Settings::ShowWindowLogProofs, flags)) {
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