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

constexpr float LINKED_ACCOUNT_INDENT = 20.0f;
constexpr float LOOKUP_INPUT_WIDTH = 200.0f;
constexpr const char* FAILED_TEXT = "?";

enum class ColumnKind {
	ACCOUNT,
	CHARACTER,
	SUBGROUP,
	PROFILE_ID,
	PROOF
};

struct Column {
	ColumnKind kind;
	std::string name;
	std::string proofId; // Only for PROOF columns.
	Texture* texture = nullptr;
};

struct PlayerRow {
	SquadMember member;
	PlayerLoad load;
};

static std::vector<Column> BuildColumns(const BossGroup& group, bool showProfileId) {
	std::vector<Column> columns {{ColumnKind::ACCOUNT, "Account"}};
	if (Settings::ShowCharacterNames) columns.push_back({ColumnKind::CHARACTER, "Character"});
	columns.push_back({ColumnKind::SUBGROUP, "Group"});
	if (showProfileId) columns.push_back({ColumnKind::PROFILE_ID, "Id"});
	for (const auto& currency : group.currencies) {
		columns.push_back({ColumnKind::PROOF, currency, currency, GetCurrencyTexture(currency)});
	}
	for (const auto& boss : group.bosses) {
		columns.push_back({ColumnKind::PROOF, GetBossName(*boss.boss, boss.type), boss.proofId, GetBossTexture(*boss.boss)});
	}
	return columns;
}

static void SetupTableColumns(const std::vector<Column>& columns) {
	for (const auto& column : columns) {
		switch (column.kind) {
			case ColumnKind::ACCOUNT:
				ImGui::TableSetupColumn(column.name.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, Settings::ColumnSizeAccount);
				break;
			case ColumnKind::CHARACTER:
				// No width given, so ImGui fits the column to the longest character name.
				ImGui::TableSetupColumn(column.name.c_str(), ImGuiTableColumnFlags_WidthFixed);
				break;
			case ColumnKind::SUBGROUP:
				ImGui::TableSetupColumn(column.name.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultHide);
				break;
			case ColumnKind::PROFILE_ID:
				ImGui::TableSetupColumn(column.name.c_str(), ImGuiTableColumnFlags_WidthFixed, Settings::ColumnSizeKpmeId);
				break;
			case ColumnKind::PROOF:
				ImGui::TableSetupColumn(column.name.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, Settings::ColumnSizeBosses);
				break;
		}
	}
}

static void DrawTableHeaders(const std::vector<Column>& columns) {
	ImGui::TableSetupScrollFreeze(1, 1);
	ImGui::TableNextRow(ImGuiTableRowFlags_Headers, Settings::BossIconScale + ImGui::GetStyle().CellPadding.y * 2.0f);
	for (const auto& column : columns) {
		if (column.kind == ColumnKind::PROOF) {
			DrawHeaderCell(("##" + column.proofId).c_str(), column.name, column.texture);
		} else {
			ImGui::TableNextColumn();
			ImGui::TableHeader(column.name.c_str());
		}
	}
}

static const ProofCounts& CountsForSelectedMode(const PlayerProofs& proofs) {
	return Settings::LinkedAccountsMode == COMBINE_LINKED ? proofs.combined : proofs.own;
}

// Explains a row without proof counts; null when the row has data or is still loading.
static const char* MissingDataReason(const PlayerLoad& load, const IProvider& provider) {
	if (load.state == LoadState::FAILED) return "Could not reach the service; retrying";
	if (load.state != LoadState::READY) return nullptr;
	if (load.proofs && !CountsForSelectedMode(*load.proofs).empty()) return nullptr;
	return provider.MissingDataDescription();
}

static void DrawCommanderIcon() {
	Texture* texture = GetCommanderTexture();
	if (!texture) return;
	float height = ImGui::GetTextLineHeight();
	// The wiki tag is 70x74, so keep its aspect ratio at text height.
	ImGui::Image((void*) texture->Resource, ImVec2(height * texture->Width / texture->Height, height));
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Commander");
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
}

static void DrawAccountName(const SquadMember& member, const PlayerProofs* proofs, const char* missingReason) {
	if (member.commander && Settings::ShowCommanderTag) DrawCommanderIcon();
	if (proofs && !proofs->profileUrl.empty()) {
		if (ImGui::TextURL(member.account.c_str())) {
			ShellExecuteA(0, 0, proofs->profileUrl.c_str(), 0, 0, SW_SHOW);
		}
	} else {
		ImGui::TextUnformatted(member.account.c_str());
	}
	if (ImGui::IsItemHovered() && (member.addedManually || missingReason)) {
		ImGui::BeginTooltip();
		if (missingReason) ImGui::TextUnformatted(missingReason);
		if (member.addedManually) ImGui::TextUnformatted(member.inSquad ? "Looked up manually" : "Looked up manually, not in your squad");
		ImGui::EndTooltip();
	}
}

static void DrawProfileId(const PlayerProofs* proofs) {
	if (!proofs || proofs->profileId.empty()) return;
	if (ImGui::TextURL(proofs->profileId.c_str())) {
		ImGui::SetClipboardText(proofs->profileId.c_str());
	}
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Click to copy");
}

static void DrawCount(const ProofCounts* counts, const std::string& proofId, LoadState state, const char* missingText) {
	if (counts) {
		auto it = counts->find(proofId);
		if (it == counts->end()) {
			ImGui::TextUnformatted(missingText);
			return;
		}
		const ProofAmount& amount = it->second;
		ImGui::TextUnformatted(std::to_string(amount.Total(Settings::TokensPerCoffer)).c_str());
		if (amount.coffers > 0 && ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Tokens: %d\nCoffers: %d (%d tokens each)", amount.items, amount.coffers, Settings::TokensPerCoffer);
		}
	} else if (state == LoadState::LOADING) {
		DrawSpinner();
	} else {
		ImGui::TextUnformatted(missingText);
	}
}

static void DrawPlayerRow(const PlayerRow& row, const std::vector<Column>& columns, const IProvider& provider) {
	const auto& [state, proofs] = row.load;
	const ProofCounts* counts = proofs ? &CountsForSelectedMode(*proofs) : nullptr;
	const bool hasProofs = counts && !counts->empty();
	const char* missingReason = MissingDataReason(row.load, provider);
	const char* missingText = hasProofs ? "0" : state == LoadState::FAILED ? FAILED_TEXT : Settings::UnavailableText.c_str();

	if (!hasProofs) ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
	for (const auto& column : columns) {
		ImGui::TableNextColumn();
		switch (column.kind) {
			case ColumnKind::ACCOUNT:
				DrawAccountName(row.member, proofs.get(), missingReason);
				break;
			case ColumnKind::CHARACTER:
				ImGui::TextUnformatted(row.member.character.c_str());
				break;
			case ColumnKind::SUBGROUP:
				if (row.member.subgroup) ImGui::TextUnformatted(std::to_string(row.member.subgroup).c_str());
				break;
			case ColumnKind::PROFILE_ID:
				HighlightColumnOnHover();
				DrawProfileId(proofs.get());
				break;
			case ColumnKind::PROOF:
				HighlightColumnOnHover();
				DrawCount(counts, column.proofId, state, missingText);
				break;
		}
	}
	if (!hasProofs) ImGui::PopStyleColor();
	HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);

	if (Settings::LinkedAccountsMode != SPLIT_LINKED || !proofs) return;
	for (const auto& linkedAccount : proofs->linkedAccounts) {
		for (const auto& column : columns) {
			ImGui::TableNextColumn();
			if (column.kind == ColumnKind::ACCOUNT) {
				ImGui::Indent(LINKED_ACCOUNT_INDENT);
				ImGui::TextUnformatted(linkedAccount.accountName.c_str());
				ImGui::Unindent(LINKED_ACCOUNT_INDENT);
			} else if (column.kind == ColumnKind::PROOF) {
				HighlightColumnOnHover();
				DrawCount(&linkedAccount.proofs, column.proofId, LoadState::READY, "0");
			}
		}
		HighlightRowOnHover(ImGui::GetCurrentContext()->CurrentTable);
	}
}

static std::variant<std::string, int> SortKey(const PlayerRow& row, const Column& column) {
	switch (column.kind) {
		case ColumnKind::ACCOUNT:
			return row.member.account;
		case ColumnKind::CHARACTER:
			return row.member.character;
		case ColumnKind::SUBGROUP:
			return row.member.subgroup;
		case ColumnKind::PROFILE_ID:
			return row.load.proofs ? row.load.proofs->profileId : std::string();
		case ColumnKind::PROOF: {
			if (!row.load.proofs) return -1;
			const auto& counts = CountsForSelectedMode(*row.load.proofs);
			auto it = counts.find(column.proofId);
			return it != counts.end() ? it->second.Total(Settings::TokensPerCoffer) : 0;
		}
	}
	return 0;
}

// Sorts by the clicked column; rows keep squad join order while no column is sorted.
static void SortRows(std::vector<PlayerRow>& rows, const std::vector<Column>& columns) {
	const ImGuiTableSortSpecs* specs = ImGui::TableGetSortSpecs();
	if (!specs || specs->SpecsCount == 0) return;
	const ImGuiTableColumnSortSpecs& spec = specs->Specs[0];
	if (spec.ColumnIndex < 0 || spec.ColumnIndex >= static_cast<int>(columns.size())) return;
	const Column& column = columns[spec.ColumnIndex];
	const bool descending = spec.SortDirection == ImGuiSortDirection_Descending;
	std::stable_sort(rows.begin(), rows.end(), [&](const PlayerRow& a, const PlayerRow& b) {
		return descending ? SortKey(b, column) < SortKey(a, column) : SortKey(a, column) < SortKey(b, column);
	});
}

static void DrawGenericTab(const BossGroup& group, IProvider* provider) {
	if (!ImGui::BeginTabItem(group.name.c_str())) return;
	const std::string providerName = provider->Name();
	const auto columns = BuildColumns(group, provider->ShowsProfileId());
	// ImGui keys saved column widths, order and sorting by table ID, so each column layout gets its own ID.
	const std::string tableId = Settings::ShowCharacterNames ? group.tableName : group.tableName + "NoCharacter";
	if (ImGui::BeginTable(tableId.c_str(), static_cast<int>(columns.size()), tableFlags)) {
		SetupTableColumns(columns);
		DrawTableHeaders(columns);

		std::vector<PlayerRow> rows;
		for (auto& member : PlayerManager::Members()) {
			PlayerManager::lazyLoadManager.RequestPlayerData(member.account, providerName);
			PlayerLoad load = PlayerManager::lazyLoadManager.GetPlayerLoad(member.account, providerName);
			if (!Settings::IncludeMissingAccounts && load.state == LoadState::READY && (!load.proofs || CountsForSelectedMode(*load.proofs).empty())) continue;
			rows.push_back({std::move(member), std::move(load)});
		}
		if (rows.empty()) {
			ImGui::TableNextColumn();
			ImGui::TextUnformatted("No players found... ");
		}
		SortRows(rows, columns);
		for (const auto& row : rows) {
			DrawPlayerRow(row, columns, *provider);
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

static void DrawPlayerLookup() {
	static std::string lookup;
	ImGui::SetNextItemWidth(LOOKUP_INPUT_WIDTH);
	bool submitted = ImGui::InputString("##Lookup", lookup, ImGuiInputTextFlags_EnterReturnsTrue, "Account name or KP ID");
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Look up a player who is not in your squad");
	ImGui::SameLine();
	submitted |= ImGui::Button("Add");
	if (submitted && PlayerManager::AddManualPlayer(lookup)) {
		lookup.clear();
	}
	if (PlayerManager::HasManualPlayers()) {
		ImGui::SameLine();
		if (ImGui::Button("Clear added")) PlayerManager::ClearManualPlayers();
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
	DrawPlayerLookup();
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