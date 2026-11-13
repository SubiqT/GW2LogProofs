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
		std::vector<std::string> accounts;
		std::string selfAccount;
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

	void AddPlayer(const std::string& account) {
		if (account.empty()) return;
		std::scoped_lock lock(mutex);
		if (std::find(accounts.begin(), accounts.end(), account) != accounts.end()) return;
		accounts.push_back(account);
		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player added to tracking: {}", account).c_str());
	}

	void RemovePlayer(const std::string& account) {
		std::scoped_lock lock(mutex);
		if (std::erase(accounts, account)) {
			APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player removed from tracking: {}", account).c_str());
		}
	}

	void SetSelf(const std::string& account) {
		{
			std::scoped_lock lock(mutex);
			selfAccount = account;
		}
		AddPlayer(account);
	}

	bool IsSelf(const std::string& account) {
		std::scoped_lock lock(mutex);
		return account == selfAccount;
	}

	void ClearPlayers() {
		std::scoped_lock lock(mutex);
		std::erase_if(accounts, [](const std::string& account) { return account != selfAccount; });
	}

	std::vector<std::string> Accounts() {
		std::scoped_lock lock(mutex);
		return accounts;
	}
} // namespace PlayerManager
