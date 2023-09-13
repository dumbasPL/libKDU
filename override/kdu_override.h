#pragma once
#include <Windows.h>

// polyfill functions
#define supQueryResourceData overrideQueryResourceData
#define GetProcAddress overrideGetProcAddress
#define LoadLibraryEx overrideLoadLibraryEx

PBYTE overrideQueryResourceData(
    _In_ ULONG_PTR ResourceId,
    _In_ PVOID DllHandle,
    _In_ PULONG DataSize
);

FARPROC WINAPI overrideGetProcAddress(
    _In_ HMODULE hModule,
    _In_ LPCSTR lpProcName
);

HMODULE WINAPI overrideLoadLibraryEx(
    _In_ LPCWSTR lpLibFileName,
    _Reserved_ HANDLE hFile,
    _In_ DWORD dwFlags
);