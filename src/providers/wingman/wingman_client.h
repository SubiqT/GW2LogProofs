#ifndef WINGMAN_CLIENT_H
#define WINGMAN_CLIENT_H

#include "wingman_types.h"
#include <functional>

namespace Wingman {
	class WingmanClient {
	public:
		WingmanResponse GetKp(const std::string& account);
		void GetKpAsync(const std::string& account, std::function<void(const WingmanResponse&)> callback);
	};
} // namespace Wingman

#endif