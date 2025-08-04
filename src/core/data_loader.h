#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>

namespace DataLoader {
	void LoadPlayerDataLazy(const std::string& account, const std::string& providerName, const std::string& key);
	void LoadPlayerData(const std::string& account, const std::string& providerName);
	void ReloadAllPlayersWithProvider(const std::string& providerName);

	void Shutdown();
} // namespace DataLoader

#endif