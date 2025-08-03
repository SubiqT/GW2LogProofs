#include "boss_registry.h"

BossRegistry& BossRegistry::Instance() {
	static BossRegistry instance;
	return instance;
}

void BossRegistry::RegisterProvider(const std::string& providerName, std::unique_ptr<IBossProvider> provider) {
	providers_[providerName] = std::move(provider);
}

IBossProvider* BossRegistry::GetProvider(const std::string& providerName) const {
	auto it = providers_.find(providerName);
	return (it != providers_.end()) ? it->second.get() : nullptr;
}

std::vector<std::string> BossRegistry::GetAvailableProviders() const {
	std::vector<std::string> providers;
	for (const auto& pair : providers_) {
		providers.push_back(pair.first);
	}
	return providers;
}

void BossGroup::InitializeCache(const IBossProvider* provider) const {
	if (cacheInitialized) return;

	cachedCurrencyIds.clear();
	for (const auto& currency : currencies) {
		cachedCurrencyIds.push_back(provider->GetProofIdentifier(currency));
	}

	cachedBossIds.clear();
	for (const auto& bossEntry : bosses) {
		cachedBossIds.push_back(provider->GetProofIdentifier(bossEntry.boss, bossEntry.type));
	}

	cacheInitialized = true;
}