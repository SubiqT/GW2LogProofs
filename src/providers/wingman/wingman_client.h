#ifndef WINGMAN_CLIENT_H
#define WINGMAN_CLIENT_H

#include "wingman_types.h"

namespace Wingman {
	class WingmanClient {
	public:
		WingmanResponse GetKp(const std::string& account);
	};
} // namespace Wingman

#endif