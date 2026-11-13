#ifndef SETTINGS_H
#define SETTINGS_H

#include <filesystem>

#include "../imgui/imgui.h"

enum DataSource {
	WINGMAN,
	KPME
};

enum LinkedAccountMode {
	HIDE_LINKED,
	COMBINE_LINKED,
	SPLIT_LINKED
};

namespace Settings {
	struct Range {
		float min;
		float max;
	};
	inline constexpr Range WINDOW_SIZE_RANGE {100.0f, 1500.0f};
	inline constexpr Range ACCOUNT_COLUMN_RANGE {40.0f, 400.0f};
	inline constexpr Range PROOF_COLUMN_RANGE {8.0f, 128.0f};
	struct IntRange {
		int min;
		int max;
	};
	inline constexpr IntRange CACHE_TIMEOUT_MINUTES_RANGE {1, 30};
	inline constexpr IntRange RETRY_ATTEMPTS_RANGE {1, 10};

	// Missing, mistyped or out-of-range values keep their defaults.
	void Load(const std::filesystem::path& filePath);
	// Writes every setting to the file passed to Load, preserving keys this version does not know.
	void Save();

	extern bool ShowQuickAccessShortcut;
	extern bool ShowWindowLogProofs;

	extern float MinWindowWidth;
	extern float MinWindowHeight;
	extern float MaxWindowWidth;
	extern float MaxWindowHeight;
	extern bool WindowAutoResize;
	extern bool WindowRestrictSize;

	extern float ColumnSizeAccount;
	extern float ColumnSizeBosses;
	extern float ColumnSizeKpmeId;
	extern float BossIconScale;

	extern DataSource SelectedDataSource;
	extern LinkedAccountMode LinkedAccountsMode;
	extern bool IncludeMissingAccounts;

	extern bool hoverEnabled;
	extern ImU32 hoverColour;
	extern ImVec4 hoverColourBuffer;

	extern int CacheTimeoutMinutes;
	extern int MaxRetryAttempts;
} // namespace Settings

#endif
