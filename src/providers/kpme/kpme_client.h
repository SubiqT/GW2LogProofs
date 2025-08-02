#ifndef KPME_CLIENT_H
#define KPME_CLIENT_H

#include "kpme_types.h"
#include <functional>

namespace Kpme {
	class KpmeClient {
	public:
		KpmeResponse GetKp(const std::string& account);
		void GetKpAsync(const std::string& account, std::function<void(const KpmeResponse&)> callback);
	};
} // namespace Kpme

#endif