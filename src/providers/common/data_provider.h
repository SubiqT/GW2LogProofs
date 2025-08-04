#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <any>
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

struct LinkedAccountData {
	std::string accountName;
	std::map<std::string, ProofData> proofs;
};

struct PlayerProofData {
	std::string accountName;
	std::string profileId;
	std::string profileUrl;
	std::map<std::string, ProofData> proofs;
	bool hasLinkedAccounts;
	std::vector<LinkedAccountData> linkedAccounts;
	std::any rawData; // Provider-specific raw data for dynamic computation
};

class IDataProvider {
public:
	virtual ~IDataProvider() = default;
	virtual std::string GetProviderName() const = 0;
	virtual PlayerProofData LoadPlayerData(const std::string& account) = 0;
	virtual void LoadPlayerDataAsync(const std::string& account, std::function<void(const PlayerProofData&)> callback) = 0;
	virtual bool SupportsLinkedAccounts() const = 0;
	virtual std::vector<std::string> GetSupportedProofTypes() const = 0;
	virtual PlayerProofData ComputeProofsFromRawData(const PlayerProofData& rawData, bool includeLinkedAccounts) const { return rawData; }
};

using DataProviderFactory = std::function<std::unique_ptr<IDataProvider>()>;

#endif