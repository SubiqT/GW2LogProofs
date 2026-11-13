#ifndef KPME_CLIENT_H
#define KPME_CLIENT_H

#include "../../nlohmann/json.hpp"
#include "kpme_types.h"
#include <string>

namespace Kpme {
	std::string KillProofUrl(const std::string& account);
	KpmeResponse ParseResponse(const nlohmann::json& j);
} // namespace Kpme

#endif
