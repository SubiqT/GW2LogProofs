#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "lazy_load_manager.h"
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

struct SquadMember {
	std::string account;
	std::string character;
	uint8_t subgroup = 0; // 1-based; 0 when unknown or not in a squad.
	bool commander = false;
	bool inSquad = false;
	// Added through the lookup box; kept when they leave the squad until the lookups are cleared.
	bool addedManually = false;
};

// Fields left empty keep the member's current value, so sources that lack a field do not erase it.
struct SquadMemberUpdate {
	std::string account;
	std::optional<std::string> character;
	std::optional<uint8_t> subgroup;
	std::optional<bool> commander;
};

// Squad roster, updated from Nexus event threads and read by the render thread.
namespace PlayerManager {
	extern LazyLoadManager lazyLoadManager;

	void UpdatePlayer(const SquadMemberUpdate& update);
	void RemovePlayer(const std::string& account);
	void SetSelf(const SquadMemberUpdate& update);
	bool IsSelf(const std::string& account);
	// Removes everyone except the local player and manual lookups.
	void ClearPlayers();
	// Accepts an account name or KP ID typed by the user; returns false if the text is not a plausible id.
	bool AddManualPlayer(std::string_view id);
	void ClearManualPlayers();
	bool HasManualPlayers();
	std::vector<SquadMember> Members();
} // namespace PlayerManager

#endif
