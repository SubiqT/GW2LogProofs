#include <algorithm>
#include <format>
#include <functional>
#include <thread>
#include <variant>


#include "../core/boss_group.h"
#include "../core/bosses.h"
#include "../core/textures.h"

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
static ImGuiTableFlags tableFlags = (ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY);

namespace {
	static float bezier_ease(float t) {

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

static void DrawPlayerAccountName(const std::string& account, const PlayerProofs* proofData) {
	if (proofData && !proofData->profileUrl.empty()) {
		if (ImGui::TextURL(account.c_str())) {
			ShellExecuteA(0, 0, proofData->profileUrl.c_str(), 0, 0, SW_SHOW);
		}
	} else {
		ImGui::TextUnformatted(account.c_str());
	}
}

static void DrawKpmeId(const PlayerProofs* proofData) {
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
		static std::unordered_map<std::string, std::string> bossNameCache;
		std::string key = std::to_string(bossEntry.boss->id) + "_" + std::to_string(int(bossEntry.type));
		auto it = bossNameCache.find(key);
		if (it == bossNameCache.end()) {
			it = bossNameCache.emplace(key, GetBossName(*bossEntry.boss, bossEntry.type)).first;
		}
		columnSpecs.emplace_back(it->second, Settings::ColumnSizeBosses);
	}


	for (size_t i = 0; i < columnSpecs.size(); ++i) {
		ImGuiTableColumnFlags flags = ImGuiTableColumnFlags_WidthFixed;
		if (i == 0) flags |= ImGuiTableColumnFlags_NoHide; // Account column
		else if (i > (showKpmeId ? 1 : 0))
			flags |= ImGuiTableColumnFlags_NoResize; // Boss/currency columns
		ImGui::TableSetupColumn(columnSpecs[i].first.c_str(), flags, columnSpecs[i].second);
	}
}

// TableHeader provides click-to-sort and the column context menu; the icon is drawn over its empty label.
static void DrawHeaderCell(const char* id, const std::string& name, Texture* texture) {
	ImGui::TableNextColumn();
	ImVec2 start = ImGui::GetCursorPos();
	ImGui::TableHeader(texture ? id : name.c_str());
	if (texture) {
		ImGui::SetCursorPos(start);
		float padding = (Settings::ColumnSizeBosses - Settings::BossIconScale) * 0.5f;
		if (padding > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::Image((void*) texture->Resource, ImVec2(Settings::BossIconScale, Settings::BossIconScale));
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("%s", name.c_str());
	}
}

static void DrawTableHeaders(const BossGroup& group, bool showKpmeId) {
	ImGui::TableSetupScrollFreeze(1, 1);
	ImGui::TableNextRow(ImGuiTableRowFlags_Headers, Settings::BossIconScale + ImGui::GetStyle().CellPadding.y * 2.0f);
	ImGui::TableNextColumn();
	ImGui::TableHeader("Account");
	if (showKpmeId) {
		ImGui::TableNextColumn();
		ImGui::TableHeader("Id");
	}
	for (const auto& currency : group.currencies) {
		DrawHeaderCell(("##" + currency).c_str(), currency, GetCurrencyTexture(currency));
	}
	for (const auto& bossEntry : group.bosses) {
		DrawHeaderCell(("##" + bossEntry.proofId).c_str(), GetBossName(*bossEntry.boss, bossEntry.type), GetBossTexture(*bossEntry.boss));
	}
}

static const ProofCounts& CountsForSelectedMode(const PlayerProofs& proofs) {
	return Settings::LinkedAccountsMode == COMBINE_LINKED ? proofs.combined : proofs.own;
}

static void DrawCountCell(const ProofCounts* counts, const std::string& proofId, LoadState state, const char* missingText) {
	ImGui::TableNextColumn();
	HighlightColumnOnHover();
	if (counts) {
		auto it = counts->find(proofId);
		ImGui::TextUnformatted(it != counts->end() ? std::to_string(it->second).c_str() : missingText);
	} else if (state == LoadState::LOADING) {
		DrawSpinner();
	} else {
		ImGui::TextUnformatted(missingText);
	}
}

struct PlayerRow {
	std::string account;
	PlayerLoad load;
};

static void DrawPlayerRow(const PlayerRow& row, const BossGroup& group, bool showKpmeId) {
	const std::string& account = row.account;
	const auto& [state, proofs] = row.load;
	const ProofCounts* counts = proofs ? &CountsForSelectedMode(*proofs) : nullptr;
	const bool hasProofs = counts && !counts->empty();
	const char* missingText = hasProofs ? "0" : "";

	if (!hasProofs) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
	}

	ImGui::TableNextColumn();
	DrawPlayerAccountName(account, proofs.get());
	if (showKpmeId) {
		ImGui::TableNextColumn();
		HighlightColumnOnHover();
		DrawKpmeId(proofs.get());
	}
	for (const auto& currency : group.currencies) {
		DrawCountCell(counts, currency, state, missingText);
	}
	for (const auto& boss : group.bosses) {
		DrawCountCell(counts, boss.proofId, state, missingText);
	}

	if (!hasProofs) {
		ImGui::PopStyleColor();
	}

	HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);

	if (Settings::LinkedAccountsMode != SPLIT_LINKED || !proofs) return;
	for (const auto& linkedAccount : proofs->linkedAccounts) {
		ImGui::TableNextColumn();
		ImGui::Indent(20.0f);
		ImGui::TextUnformatted(linkedAccount.accountName.c_str());
		ImGui::Unindent(20.0f);

		if (showKpmeId) {
			ImGui::TableNextColumn();
			HighlightColumnOnHover();
			ImGui::Text("-");
		}
		for (const auto& currency : group.currencies) {
			DrawCountCell(&linkedAccount.proofs, currency, LoadState::READY, "0");
		}
		for (const auto& boss : group.bosses) {
			DrawCountCell(&linkedAccount.proofs, boss.proofId, LoadState::READY, "0");
		}

		HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
	}
}

static int ProofCount(const PlayerRow& row, const std::string& proofId) {
	if (!row.load.proofs) return -1;
	const auto& counts = CountsForSelectedMode(*row.load.proofs);
	auto it = counts.find(proofId);
	return it != counts.end() ? it->second : 0;
}

// Sorts by the clicked column; rows keep squad join order while no column is sorted.
static void SortRows(std::vector<PlayerRow>& rows, const BossGroup& group, bool showKpmeId) {
	const ImGuiTableSortSpecs* specs = ImGui::TableGetSortSpecs();
	if (!specs || specs->SpecsCount == 0) return;
	const ImGuiTableColumnSortSpecs& spec = specs->Specs[0];
	const bool descending = spec.SortDirection == ImGuiSortDirection_Descending;
	const int firstProofColumn = showKpmeId ? 2 : 1;

	auto key = [&](const PlayerRow& row) -> std::variant<std::string, int> {
		if (spec.ColumnIndex == 0) return row.account;
		if (spec.ColumnIndex < firstProofColumn) return row.load.proofs ? row.load.proofs->profileId : std::string();
		size_t proofIndex = spec.ColumnIndex - firstProofColumn;
		if (proofIndex < group.currencies.size()) return ProofCount(row, group.currencies[proofIndex]);
		proofIndex -= group.currencies.size();
		return proofIndex < group.bosses.size() ? ProofCount(row, group.bosses[proofIndex].proofId) : 0;
	};
	std::stable_sort(rows.begin(), rows.end(), [&](const PlayerRow& a, const PlayerRow& b) {
		return descending ? key(b) < key(a) : key(a) < key(b);
	});
}

static void DrawGenericTab(const BossGroup& group, IProvider* provider) {
	const std::string providerName = provider->Name();
	const bool showKpmeId = provider->ShowsProfileId();
	if (!ImGui::BeginTabItem(group.name.c_str())) return;
	int columnCount = static_cast<int>(group.currencies.size() + group.bosses.size()) + (showKpmeId ? 2 : 1);
	if (ImGui::BeginTable(group.tableName.c_str(), columnCount, tableFlags)) {
		SetupTableColumns(group, showKpmeId);
		DrawTableHeaders(group, showKpmeId);

		std::vector<PlayerRow> rows;
		for (auto& account : PlayerManager::Accounts()) {
			PlayerManager::lazyLoadManager.RequestPlayerData(account, providerName);
			PlayerLoad load = PlayerManager::lazyLoadManager.GetPlayerLoad(account, providerName);
			if (!Settings::IncludeMissingAccounts && load.state == LoadState::READY && (!load.proofs || CountsForSelectedMode(*load.proofs).empty())) continue;
			rows.push_back({std::move(account), std::move(load)});
		}
		if (rows.empty()) {
			ImGui::TableNextColumn();
			ImGui::TextUnformatted("No players found... ");
		}
		SortRows(rows, group, showKpmeId);
		for (const auto& row : rows) {
			DrawPlayerRow(row, group, showKpmeId);
		}
		ImGui::EndTable();
	}
	ImGui::EndTabItem();
}


static void DrawProviderCombo(const std::string& currentProvider) {
	if (ImGui::BeginCombo("##DataSource", currentProvider.c_str())) {
		for (const auto& registered : ProviderRegistry::Instance().All()) {
			const std::string provider = registered->Name();
			bool is_selected = (provider == currentProvider);
			if (ImGui::Selectable(provider.c_str(), is_selected)) {
				Settings::SelectedDataSource = (provider == "Wingman") ? WINGMAN : KPME;
				Settings::Save();
			}
			if (is_selected) ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

static void DrawControls(const IProvider& provider) {
	DrawProviderCombo(provider.Name());
	if (provider.SupportsLinkedAccounts()) {
		ImGui::SameLine();
		const char* modeNames[] = {"Hide linked accounts", "Combine linked accounts", "Split linked accounts"};
		if (ImGui::BeginCombo("##LinkedMode", modeNames[Settings::LinkedAccountsMode])) {
			for (int i = 0; i < 3; i++) {
				bool is_selected = (Settings::LinkedAccountsMode == i);
				if (ImGui::Selectable(modeNames[i], is_selected)) {
					Settings::LinkedAccountsMode = static_cast<LinkedAccountMode>(i);
					Settings::Save();
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


	if (isWindowOpen != wasWindowOpen) {
		if (!isWindowOpen) Settings::Save();
		wasWindowOpen = isWindowOpen;
	}


	if (!Settings::ShowWindowLogProofs) {
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
	IProvider* bossProvider = ProviderRegistry::Instance().Find(currentProvider);
	if (bossProvider) DrawControls(*bossProvider);
	if (bossProvider && ImGui::BeginTabBar(("##" + currentProvider).c_str(), ImGuiTabBarFlags_None)) {


		auto config = TabConfigManager::Instance().GetProviderConfig(currentProvider);

		if (config.useCustomTabs) {

			for (const auto& customTab : config.tabs) {
				if (customTab.visible && !customTab.displayName.empty()) {
					try {
						auto bossGroup = bossProvider->CreateCustomBossGroup(customTab);

						if (!bossGroup.bosses.empty() || !bossGroup.currencies.empty()) {
							DrawGenericTab(bossGroup, bossProvider);
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
				DrawGenericTab(group, bossProvider);
			}
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
	if (!Settings::ShowWindowLogProofs) Settings::Save();
}