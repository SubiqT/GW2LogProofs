#ifndef BOSSES_H
#define BOSSES_H

#include <cstdint>
#include <span>
#include <string>

#include "../resource.h"

enum class BossType {
	NORMAL,
	CM,
	LCM
};

BossType BossTypeFromString(const std::string& s);

enum class ProofKind {
	KILL_PROOF,
	TOKEN,
	COFFER
};

enum class Content {
	RAID,
	FRACTAL,
	STRIKE
};

namespace WingmanModes {
	inline constexpr uint8_t NONE = 0;
	inline constexpr uint8_t NORMAL = 1 << 0;
	inline constexpr uint8_t CM = 1 << 1;
	inline constexpr uint8_t LCM = 1 << 2;
} // namespace WingmanModes

struct TextureResource {
	const char* id;
	int resourceId;
};

struct BossInfo {
	// Elite Insights species id, or Wingman's synthetic id for split encounters such as the Ai phases.
	int id;
	const char* name;
	TextureResource texture;
	Content content;
	uint8_t wingmanModes;
	const char* kpmeToken = nullptr;
	const char* kpmeCoffer = nullptr;
	const char* kpmeCmCoffer = nullptr;

	bool HasWingmanMode(BossType type) const;
};

// Bosses in the order they appear in tables.
std::span<const BossInfo> AllBosses();
const BossInfo* FindBoss(int id);

std::string GetBossName(const BossInfo& boss, BossType type);
std::string GetBossName(const BossInfo& boss, BossType type, ProofKind kind);

#endif
