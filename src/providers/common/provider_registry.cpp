#include "provider_registry.h"

ProviderRegistry& ProviderRegistry::Instance() {
	static ProviderRegistry instance;
	return instance;
}

void ProviderRegistry::RegisterProvider(const std::string& name, DataProviderFactory factory) {
	providers_[name] = factory;
}

std::unique_ptr<IDataProvider> ProviderRegistry::CreateProvider(const std::string& name) {
	auto it = providers_.find(name);
	if (it != providers_.end()) {
		return it->second();
	}
	return nullptr;
}

