// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "extensionutils.h" // callback and argument utilities
#include <cstdint>
#include <string>
#include <iostream>

// exported function which is called by sliver
extern "C" {
    __declspec(dllexport) int __cdecl Go(char* argsBuffer, uint32_t bufferSize, goCallback callback);
}
int Go(char* argsBuffer, uint32_t bufferSize, goCallback callback)
{
    // stores our response to the implant
    Logger logger;
    
    // our arguments
    std::wstring wusername;
    std::wstring wpassword;

    // parse the arcuments
    auto args = convertArgsBufferToArgv(argsBuffer);

    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == L"--username" && i + 1 < args.size()) {
            wusername = args[i + 1];
            i++;  // skip the next value as it's already used
        }
        else if (args[i] == L"--password" && i + 1 < args.size()) {
            wpassword = args[i + 1];
            i++;  // skip the next value as it's already used
        }
    }

    // do stuff ...
    if (wusername.empty() || wpassword.empty()) {
        // bail
        logger.log("[!] Invalid arguments!");
        logger.sendOutput(callback);
        return -1;
    }

    logger.log("[*] Username: %s\n", wstringToString(wusername).c_str());
    logger.log("[*] Password: %s\n", wstringToString(wpassword).c_str());

    // send output to implant
    logger.log("[*] ExtensionSample executed successfully.");
    logger.sendOutput(callback);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

