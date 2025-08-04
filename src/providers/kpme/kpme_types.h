#ifndef KPME_TYPES_H
#define KPME_TYPES_H

#include <map>
#include <string>
#include <vector>

namespace Kpme {
	struct KpSummary {
		std::map<std::string, int> killproofs;
		std::map<std::string, int> tokens;
		std::map<std::string, int> coffers;
		std::map<std::string, std::string> titles;
	};

	struct LinkedAccount {
		std::string name;
		KpSummary data;
	};

	struct KpmeResponse {
		std::string id;
		KpSummary self;
		KpSummary shared;
		std::vector<LinkedAccount> linked_accounts;
	};
} // namespace Kpme

#endif