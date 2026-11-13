#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "lazy_load_manager.h"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct SquadMember {
	std::string account;
	std::string character;
	uint8_t subgroup = 0; // 1-based; 0 when unknown or not in a squad.
	bool commander = false;
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
	// Removes everyone except the local player.
	void ClearPlayers();
	std::vector<SquadMember> Members();
} // namespace PlayerManager

#endif
