// A program to help debug the extension
#include <iostream>
#include <windows.h>

typedef int (*execCallback)(const char*, int);
typedef int (*ExecFunction)(char*, uint32_t, execCallback);

int callback(const char* data, int size) {
    std::cout << "[harness] Received output:\n" << std::endl;
    std::cout << std::string(data, size) << std::endl;
    return 0;
}

void usage() {
    std::cout << "[harness] Usage: harness.exe --dll [PATH_TO_DLL] --entry [ENTRY_FUNCTION_NAME] --args [ARGS_TO_PASS]" << std::endl;
    std::cout << "[harness] Example: harness.exe --dll your.dll --entry Go --args \"your args\"" << std::endl;
}

int main(int argc, char* argv[]) {
    const char* dllPath = nullptr;
    const char* entryName = nullptr;
    const char* args = nullptr;

    std::cout << "[harness] Starting..." << std::endl;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--dll") == 0 && i + 1 < argc) {
            dllPath = argv[++i];
        }
        else if (strcmp(argv[i], "--entry") == 0 && i + 1 < argc) {
            entryName = argv[++i];
        }
        else if (strcmp(argv[i], "--args") == 0 && i + 1 < argc) {
            args = argv[++i];
        }
    }

    if (!dllPath || !entryName || !args) {
        std::cerr << "[harness] Missing arguments!" << std::endl;
        usage();
        return -1;
    }

    std::cout << "[harness] Loading library: " << dllPath << std::endl;

    HMODULE hDll = LoadLibraryA(dllPath);
    if (!hDll) {
        std::cerr << "[harness] Failed to load DLL." << std::endl;
        return -1;
    }

    std::cout << "[harness] Executing function: " << entryName << std::endl;
    std::cout << "[harness] Passing arguments: " << args << std::endl;

    ExecFunction goFunc = reinterpret_cast<ExecFunction>(GetProcAddress(hDll, entryName));
    if (!goFunc) {
        std::cerr << "[harness] Failed to get entry function." << std::endl;
        FreeLibrary(hDll);
        return -1;
    }

    goFunc(const_cast<char*>(args), strlen(args), callback);

    std::cout << "\n[harness] Finished. Press any key to exit..." << std::endl;
    std::cin.get();

    FreeLibrary(hDll);
    return 0;
}
