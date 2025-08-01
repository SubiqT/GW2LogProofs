#include "httpclient.h"
#include "../core/shared.h"

#pragma comment(lib, "urlmon.lib")
#include <format>
#include <sstream>
#include <urlmon.h>

namespace HTTPClient {
	std::string GetRequest(const std::wstring& wUrl) {
		IStream* stream;
		HRESULT result = URLOpenBlockingStream(0, wUrl.c_str(), &stream, 0, 0);
		if (result != 0) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("HTTP request failed with HRESULT: 0x{:08X}", static_cast<unsigned int>(result)).c_str());
			return "";
		}

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
		std::string response = strStream.str();
		return response;
	}
} // namespace HTTPClient