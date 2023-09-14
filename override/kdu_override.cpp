#include "global.h"
#include "tanikaze.h"
#include <unordered_map>
#include <tuple>

// restore overwritten functions
#undef GetProcAddress
#undef LoadLibraryEx

#define DRV64DLL_FAKE_HANDLE (HMODULE)0x1337

typedef std::unordered_map<int, std::tuple<size_t, const unsigned char*>> ResourcesMap;
extern ResourcesMap Hamakaze_resources;
extern ResourcesMap Tanikaze_resources;

PBYTE overrideQueryResourceData(
    _In_ ULONG_PTR ResourceId,
    _In_ PVOID DllHandle,
    _In_ PULONG DataSize
) {
    if (DllHandle == GetModuleHandle(NULL)) {
        auto resource = Hamakaze_resources.find(ResourceId);
        if (resource != Hamakaze_resources.end()) {
            printf_s("[$] Hello from Hamakaze QueryResourceData polyfill\r\n");
            *DataSize = std::get<0>(resource->second);
            return (PBYTE)std::get<1>(resource->second);
        }
        return NULL;
    }

    if (DllHandle == DRV64DLL_FAKE_HANDLE) {
        auto resource = Tanikaze_resources.find(ResourceId);
        if (resource != Tanikaze_resources.end()) {
            printf_s("[$] Hello from Tanikaze QueryResourceData polyfill\r\n");
            *DataSize = std::get<0>(resource->second);
            return (PBYTE)std::get<1>(resource->second);
        }
        return NULL;
    }

    return ntsupQueryResourceData(ResourceId, DllHandle, DataSize);
}

FARPROC WINAPI overrideGetProcAddress(
    _In_ HMODULE hModule,
    _In_ LPCSTR lpProcName
) {
    if (hModule == DRV64DLL_FAKE_HANDLE && strstr(lpProcName, "gProvTable") != NULL) {
        printf_s("[$] Hello from GetProcAddress polyfill\r\n");
        return (FARPROC)&gProvTable;
    }
    return GetProcAddress(hModule, lpProcName);
}

HMODULE WINAPI overrideLoadLibraryEx(
    _In_ LPCWSTR lpLibFileName,
    _Reserved_ HANDLE hFile,
    _In_ DWORD dwFlags
) {
    if (wcsstr(lpLibFileName, DRV64DLL) != NULL) {
        printf_s("[$] Hello from LoadLibraryEx polyfill\r\n");
        // return a fake handle that we can later identify to load the correct resource
        return DRV64DLL_FAKE_HANDLE;
    }
    return LoadLibraryExW(lpLibFileName, hFile, dwFlags);
}