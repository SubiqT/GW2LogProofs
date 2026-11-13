#ifndef WINGMAN_CLIENT_H
#define WINGMAN_CLIENT_H

#include "../../nlohmann/json.hpp"
#include "wingman_types.h"
#include <string>

namespace Wingman {
	std::string KillProofUrl(const std::string& account);
	WingmanResponse ParseResponse(const nlohmann::json& j);
} // namespace Wingman

#endif
