#include "httpclient.h"
#include "../core/settings.h"
#include "../core/shared.h"

#pragma comment(lib, "urlmon.lib")
#include <format>
#include <sstream>
#include <urlmon.h>
#include <Windows.h>

namespace HTTPClient {
	std::string GetRequest(const std::wstring& wUrl) {
		int maxRetries = Settings::MaxRetryAttempts;
		for (int attempt = 1; attempt <= maxRetries; ++attempt) {
			IStream* stream;
			HRESULT result = URLOpenBlockingStream(0, wUrl.c_str(), &stream, 0, 0);
			if (result == 0) {
				const unsigned long CHUNK_SIZE = 128;
				char buffer[CHUNK_SIZE];
				unsigned long bytesRead;
				std::stringstream strStream;

				stream->Read(buffer, CHUNK_SIZE, &bytesRead);
				while (bytesRead > 0) {
					strStream.write(buffer, (long long) bytesRead);
					stream->Read(buffer, CHUNK_SIZE, &bytesRead);
				}
				stream->Release();
				return strStream.str();
			}

			// Only retry on internal server errors (5xx)
			bool shouldRetry = (result >= 0x80190190 && result <= 0x801901F3); // HTTP 500-599

			if (attempt == 1) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("HTTP request failed with HRESULT: 0x{:08X}", static_cast<unsigned int>(result)).c_str());
			}

			if (!shouldRetry || attempt >= maxRetries) {
				return "";
			}

			int delayMs = 1000 * (1 << (attempt - 1));
			Sleep(delayMs);
		}
		return "";
	}
} // namespace HTTPClient