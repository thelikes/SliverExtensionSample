#pragma once

// provided for communication with the implant
typedef int (*goCallback)(const char*, int);

// argument parsing
#include <vector>
#include <string>
std::vector<std::wstring> convertArgsBufferToArgv(const char* argsBuffer);

// output for callback
class Logger {
private:
    std::string output;
public:
    void log(const char* format, ...);
    void sendOutput(goCallback callback);
};

// string conversion
std::string wstringToString(const std::wstring& wstr);