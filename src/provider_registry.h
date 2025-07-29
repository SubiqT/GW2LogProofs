#ifndef PROVIDER_REGISTRY_H
#define PROVIDER_REGISTRY_H

#include "data_provider.h"
#include <unordered_map>

class ProviderRegistry {
public:
    static ProviderRegistry& Instance();
    
    void RegisterProvider(const std::string& name, DataProviderFactory factory);
    std::unique_ptr<IDataProvider> CreateProvider(const std::string& name);
    std::vector<std::string> GetAvailableProviders() const;

private:
    std::unordered_map<std::string, DataProviderFactory> providers_;
};

#endif