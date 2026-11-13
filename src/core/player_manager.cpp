#include "player_manager.h"
#include "../providers/common/provider_registry.h"
#include "settings.h"
#include "shared.h"
#include <algorithm>
#include <format>
#include <mutex>

namespace PlayerManager {
	namespace {
		std::mutex mutex;
		std::vector<SquadMember> members;
		std::string selfAccount;

		void UpdateLocked(const SquadMemberUpdate& update) {
			auto it = std::find_if(members.begin(), members.end(), [&](const SquadMember& m) { return m.account == update.account; });
			if (it == members.end()) {
				members.push_back({update.account});
				it = std::prev(members.end());
				APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player added to tracking: {}", update.account).c_str());
			}
			if (update.character && !update.character->empty()) it->character = *update.character;
			if (update.subgroup) it->subgroup = *update.subgroup;
			if (update.commander) it->commander = *update.commander;
		}
	} // namespace

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

	void UpdatePlayer(const SquadMemberUpdate& update) {
		if (update.account.empty()) return;
		std::scoped_lock lock(mutex);
		UpdateLocked(update);
	}

	void RemovePlayer(const std::string& account) {
		std::scoped_lock lock(mutex);
		if (std::erase_if(members, [&](const SquadMember& m) { return m.account == account; })) {
			APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player removed from tracking: {}", account).c_str());
		}
	}

	void SetSelf(const SquadMemberUpdate& update) {
		if (update.account.empty()) return;
		std::scoped_lock lock(mutex);
		selfAccount = update.account;
		UpdateLocked(update);
	}

	bool IsSelf(const std::string& account) {
		std::scoped_lock lock(mutex);
		return account == selfAccount;
	}

	void ClearPlayers() {
		std::scoped_lock lock(mutex);
		std::erase_if(members, [](const SquadMember& m) { return m.account != selfAccount; });
		for (auto& self : members) {
			self.subgroup = 0;
			self.commander = false;
		}
	}

	std::vector<SquadMember> Members() {
		std::scoped_lock lock(mutex);
		return members;
	}
} // namespace PlayerManager
