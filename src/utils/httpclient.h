#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>

namespace HTTPClient {
	std::string GetRequest(const std::wstring& wUrl);
}

#endif