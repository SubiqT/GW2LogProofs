#ifndef PROVIDER_REGISTRY_H
#define PROVIDER_REGISTRY_H

#include "provider.h"
#include <memory>
#include <string_view>
#include <vector>

class ProviderRegistry {
public:
	static ProviderRegistry& Instance();

	void Register(std::unique_ptr<IProvider> provider);
	IProvider* Find(std::string_view name) const;
	// In registration order.
	const std::vector<std::unique_ptr<IProvider>>& All() const { return providers_; }

private:
	std::vector<std::unique_ptr<IProvider>> providers_;
};

#endif
