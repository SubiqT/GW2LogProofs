#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <functional>
#include <future>
#include <string>
#include <mutex>

namespace HTTPClient {
	void Initialize();
	std::string GetRequest(const std::wstring& wUrl);
	std::future<std::string> GetRequestAsync(const std::wstring& wUrl);
	void GetRequestAsync(const std::wstring& wUrl, std::function<void(const std::string&)> callback);
	void Shutdown();
} // namespace HTTPClient

#endif