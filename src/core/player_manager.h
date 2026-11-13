#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "lazy_load_manager.h"
#include <string>
#include <vector>

// Squad roster, updated from Nexus event threads and read by the render thread.
namespace PlayerManager {
	extern LazyLoadManager lazyLoadManager;

	void AddPlayer(const std::string& account);
	void RemovePlayer(const std::string& account);
	void SetSelf(const std::string& account);
	bool IsSelf(const std::string& account);
	// Removes everyone except the local player.
	void ClearPlayers();
	std::vector<std::string> Accounts();
} // namespace PlayerManager

#endif
