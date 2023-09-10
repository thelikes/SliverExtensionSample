#include "pch.h"
#include "extensionutils.h"
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

// argument parsing helper
std::vector<std::wstring> convertArgsBufferToArgv(const char* argsBuffer) {
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, argsBuffer, -1, nullptr, 0);
    WCHAR* wideBuffer = new WCHAR[bufferSize];
    MultiByteToWideChar(CP_UTF8, 0, argsBuffer, -1, wideBuffer, bufferSize);

    int argc;
    LPWSTR* argvW = CommandLineToArgvW(wideBuffer, &argc);

    std::vector<std::wstring> args(argvW, argvW + argc);

    // Cleanup
    LocalFree(argvW);
    delete[] wideBuffer;

    return args;
}

// to help return output to the implant
void Logger::log(const char* format, ...) {
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    output += buffer;
}

void Logger::sendOutput(goCallback callback) {
    callback(output.c_str(), output.length());
}

// string conversion
std::string wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, NULL, NULL);
    return result;
}