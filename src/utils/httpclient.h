#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <functional>
#include <string>
#include <string_view>

namespace HTTPClient {
	struct Response {
		// False when no HTTP response was received, for example after a DNS, TLS or timeout failure.
		bool received = false;
		unsigned long status = 0;
		std::string body;
	};

	void Initialize();
	// Callbacks run on a WinHTTP worker thread and are not delivered once Shutdown has started.
	void GetAsync(const std::string& url, std::function<void(const Response&)> callback);
	// Waits for in-flight requests so that no callback runs after the addon is unloaded.
	void Shutdown();

	std::string EncodeUrlComponent(std::string_view value);
} // namespace HTTPClient

#endif
