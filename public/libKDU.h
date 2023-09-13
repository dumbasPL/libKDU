#pragma once
#include <Windows.h>

BOOL libKDUMapDriver(
    _In_ ULONG HvciEnabled,
    _In_ ULONG NtBuildNumber,
    _In_ ULONG ProviderId,
    _In_ ULONG ShellVersion,
    _In_ LPVOID DriverData,
    _In_opt_ LPWSTR DriverObjectName,
    _In_opt_ LPWSTR DriverRegistryPath
);