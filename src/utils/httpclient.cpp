#include "httpclient.h"

#pragma comment(lib, "urlmon.lib")
#include <sstream>
#include <urlmon.h>

namespace HTTPClient {
	std::string GetRequest(const std::wstring& wUrl) {
		IStream* stream;
		HRESULT result = URLOpenBlockingStream(0, wUrl.c_str(), &stream, 0, 0);
		if (result != 0) {
			return "An error occured.";
		}

		const unsigned long chunkSize = 128;
		char buffer[chunkSize];
		unsigned long bytesRead;
		std::stringstream strStream;

		stream->Read(buffer, chunkSize, &bytesRead);
		while (bytesRead > 0) {
			strStream.write(buffer, (long long) bytesRead);
			stream->Read(buffer, chunkSize, &bytesRead);
		}
		stream->Release();
		std::string response = strStream.str();
		return response;
	}
} // namespace HTTPClient