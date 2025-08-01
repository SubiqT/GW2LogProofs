#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <cstdint>
#include <string>

struct PlayerInfo {
	std::string account;
	std::string character;
	uintptr_t id = 0;        // Tracker-specific ID (0 if not applicable)
	uint32_t instanceId = 0; // Instance-specific ID (0 if not applicable)
	std::string source;      // Tracker name that provided this info
	uint64_t timestamp = 0;  // When this info was received
};

#endif