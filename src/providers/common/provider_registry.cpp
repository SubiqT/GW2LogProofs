#include "provider_registry.h"

ProviderRegistry& ProviderRegistry::Instance() {
	static ProviderRegistry instance;
	return instance;
}

void ProviderRegistry::Register(std::unique_ptr<IProvider> provider) {
	providers_.push_back(std::move(provider));
}

IProvider* ProviderRegistry::Find(std::string_view name) const {
	for (const auto& provider : providers_) {
		if (provider->Name() == name) return provider.get();
	}
	return nullptr;
}
