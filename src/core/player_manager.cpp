#include "player_manager.h"
#include "../providers/common/provider_registry.h"
#include "settings.h"
#include "shared.h"
#include <format>

namespace PlayerManager {
	std::vector<Player> players;
	std::string selfAccountName;
	std::mutex playerMutex;
	LazyLoadManager lazyLoadManager(
			[](const std::string& account, const std::string& provider, FetchCallback callback) {
				IProvider* source = ProviderRegistry::Instance().Find(provider);
				if (!source) {
					callback({FetchStatus::FAILED});
					return;
				}
				source->FetchAsync(account, std::move(callback));
			},
			[] { return LoadPolicy {std::chrono::minutes(Settings::CacheTimeoutMinutes), Settings::MaxRetryAttempts}; }
	);

	static long long GetPlayerIndex(const std::string& account) {
		for (Player& p : players)
			if (p.account == account) return std::addressof(p) - std::addressof(players.at(0));
		return -1;
	}

	void AddPlayer(const PlayerInfo& playerInfo) {
		std::scoped_lock lck(playerMutex);
		for (const Player& p : players)
			if (p.account == playerInfo.account) return;

		players.push_back({playerInfo.account});

		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player added to tracking: {}", playerInfo.account).c_str());
	}

	void RemovePlayer(const PlayerInfo& playerInfo) {
		std::scoped_lock lck(playerMutex);
		long long index = GetPlayerIndex(playerInfo.account);
		if (index != -1) {
			players.erase(players.begin() + index);
			APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player removed from tracking: {}", playerInfo.account).c_str());
		}
	}

	void SetSelf(const PlayerInfo& playerInfo) {
		selfAccountName = playerInfo.account;
		AddPlayer(playerInfo);
	}

	void ClearPlayers() {
		std::scoped_lock lck(playerMutex);
		long long index = GetPlayerIndex(selfAccountName);
		if (index == -1) {
			players.clear();
		} else {
			Player self = std::move(players[index]);
			players.clear();
			players.emplace_back(std::move(self));
		}
	}

} // namespace PlayerManager