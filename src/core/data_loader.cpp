#include "data_loader.h"
#include "../providers/common/provider_registry.h"
#include "../utils/httpclient.h"
#include "player_manager.h"
#include "shared.h"
#include <format>

namespace DataLoader {

	static long long GetPlayerIndex(const std::string& account) {
		for (Player& p : PlayerManager::players)
			if (p.account == account) return std::addressof(p) - std::addressof(PlayerManager::players.at(0));
		return -1;
	}

	void LoadPlayerDataLazy(const std::string& account, const std::string& providerName, const std::string& key) {
		try {
			auto provider = ProviderRegistry::Instance().CreateProvider(providerName);
			if (!provider) {
				PlayerManager::lazyLoadManager.OnLoadFailed(key);
				return;
			}

			provider->LoadPlayerDataAsync(account, [key](const PlayerProofData& data) {
				PlayerManager::lazyLoadManager.OnLoadComplete(key, std::make_unique<PlayerProofData>(data));
			});
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to load {} data for {}: {}", providerName, account, e.what()).c_str());
			PlayerManager::lazyLoadManager.OnLoadFailed(key);
		}
	}

	void LoadPlayerData(const std::string& account, const std::string& providerName) {
		try {
			auto provider = ProviderRegistry::Instance().CreateProvider(providerName);
			if (!provider) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Provider not found: {}", providerName).c_str());
				return;
			}

			provider->LoadPlayerDataAsync(account, [account, providerName](const PlayerProofData& data) {
				try {
					std::scoped_lock lck(PlayerManager::playerMutex);
					long long index = GetPlayerIndex(account);
					if (index == -1) {
						APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Player not found when loading data: {}", account).c_str());
						return;
					}

					// Check if data is empty (failed request)
					if (data.accountName.empty() && data.proofs.empty()) {
						PlayerManager::players[index].state = ::LoadState::FAILED;
						APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to load {} data for {}", providerName, account).c_str());
						return;
					}

					PlayerManager::players[index].proofData = std::make_unique<PlayerProofData>(data);
					PlayerManager::players[index].state = ::LoadState::READY;
					PlayerManager::players[index].providerName = providerName;

					APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Loaded {} data for {}", providerName, account).c_str());
				} catch (const std::exception& e) {
					APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in async callback for {} data for {}: {}", providerName, account, e.what()).c_str());
					std::scoped_lock lck(PlayerManager::playerMutex);
					long long index = GetPlayerIndex(account);
					if (index != -1) {
						PlayerManager::players[index].state = ::LoadState::FAILED;
					}
				}
			});
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error loading {} data for {}: {}", providerName, account, e.what()).c_str());

			std::scoped_lock lck(PlayerManager::playerMutex);
			long long index = GetPlayerIndex(account);
			if (index != -1) {
				PlayerManager::players[index].state = ::LoadState::FAILED;
			}
		}
	}

	void ReloadAllPlayersWithProvider(const std::string& providerName) {
		std::scoped_lock lck(PlayerManager::playerMutex);
		for (auto& player : PlayerManager::players) {
			player.state = ::LoadState::LOADING;
			player.proofData = nullptr;
			player.providerName = "";
			std::string account = player.account;
			LoadPlayerData(account, providerName);
		}
	}


	void Shutdown() {
		HTTPClient::Shutdown();
	}
} // namespace DataLoader