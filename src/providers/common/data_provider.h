#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

enum class ProofType {
	KILL_PROOF,
	TOKEN,
	COFFER,
	TITLE
};

struct ProofData {
	std::string identifier;
	int amount;
	ProofType type;
	std::string displayName;
	std::string url;
};

struct PlayerProofData {
	std::string accountName;
	std::string profileId;
	std::string profileUrl;
	std::map<std::string, ProofData> proofs;
	bool hasLinkedAccounts;
};

class IDataProvider {
public:
	virtual ~IDataProvider() = default;
	virtual std::string GetProviderName() const = 0;
	virtual PlayerProofData LoadPlayerData(const std::string& account) = 0;
	virtual bool SupportsLinkedAccounts() const = 0;
	virtual std::vector<std::string> GetSupportedProofTypes() const = 0;
};

using DataProviderFactory = std::function<std::unique_ptr<IDataProvider>()>;

#endif