#include "libKDU.h"
#include "global.h"

BOOL libKDUMapDriver(
    _In_ ULONG HvciEnabled, 
    _In_ ULONG NtBuildNumber, 
    _In_ ULONG ProviderId, 
    _In_ ULONG ShellVersion, 
    _In_ LPVOID Buffer, 
    _In_opt_ LPWSTR DriverObjectName, 
    _In_opt_ LPWSTR DriverRegistryPath
) {
    DWORD SizeOfImage = 0;
    PVOID pvImage = PELoaderLoadImage(Buffer, &SizeOfImage);
    if (!pvImage) {
        return FALSE;
    }

    KDU_CONTEXT* provContext = KDUProviderCreate(ProviderId, HvciEnabled, NtBuildNumber, ShellVersion, ActionTypeMapDriver);
    if (!provContext) {
        VirtualFree(pvImage, 0, MEM_RELEASE);
        return FALSE;
    }

    if (ShellVersion == KDU_SHELLCODE_V3) {
        if (DriverObjectName)
            ScCreateFixedUnicodeString(&provContext->DriverObjectName, DriverObjectName);
        if (DriverRegistryPath)
            ScCreateFixedUnicodeString(&provContext->DriverRegistryPath, DriverRegistryPath);
    }

    BOOL retVal = provContext->Provider->Callbacks.MapDriver(provContext, pvImage);

    KDUProviderRelease(provContext);

    VirtualFree(pvImage, 0, MEM_RELEASE);

    return retVal;
}
