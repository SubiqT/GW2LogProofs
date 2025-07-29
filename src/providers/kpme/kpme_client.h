#ifndef KPME_CLIENT_H
#define KPME_CLIENT_H

#include "kpme_types.h"

namespace Kpme {
	class KpmeClient {
	public:
		KpmeResponse GetKp(const std::string& account);
	};
} // namespace Kpme

#endif