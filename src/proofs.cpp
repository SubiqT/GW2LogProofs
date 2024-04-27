#pragma comment(lib, "urlmon.lib")

#include <urlmon.h>
#include <sstream>
#include <format>

#include "shared.h"

const std::string kpUrl  = "https://gw2wingman.nevermindcreations.de/api/kp?account={}";

const char* GetProof(void) {
    //const char* url = std::format(kpUrl, account).c_str();
    const char* url = "https://gw2wingman.nevermindcreations.de/api/kp?account=Subi.8014";
    std::wstring wUrl(url, url + strlen(url));

    IStream* stream;
    HRESULT result = URLOpenBlockingStream(0, wUrl.c_str(), &stream, 0, 0);
    if (result != 0)
    {
        return "An error occured.";
    }

    const unsigned long chunkSize = 128;
    char buffer[chunkSize];
    unsigned long bytesRead;
    std::stringstream strStream;

    stream->Read(buffer, chunkSize, &bytesRead);
    while (bytesRead > 0)
    {
        strStream.write(buffer, (long long)bytesRead);
        stream->Read(buffer, chunkSize, &bytesRead);
    }
    stream->Release();
    return strStream.str().c_str();
}
