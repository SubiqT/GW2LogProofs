#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <functional>
#include <string>
#include <mutex>

namespace HTTPClient {
	void Initialize();

	void GetRequestAsync(const std::wstring& wUrl, std::function<void(const std::string&)> callback);
	void Shutdown();
} // namespace HTTPClient

#endif