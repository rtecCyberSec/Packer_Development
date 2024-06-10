#include <windows.h>

/**
 * Checks whether a system is domain joined by enumerating the registry.
 */
BOOL 
IsDomainJoined() 
{
    HKEY  hKey;
    LONG  lResult;
    CHAR  domainName[MAX_PATH];
    DWORD dwType = REG_SZ;
    DWORD dwSize = sizeof(domainName);

    lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
        0,
        KEY_READ,
        &hKey
    );
    
    if (lResult != ERROR_SUCCESS) 
    {
        printf("[!] Error opening registry key");
        exit(1);
    }

    lResult = RegQueryValueExA(hKey, "Domain", NULL, &dwType, (LPBYTE)domainName, &dwSize);
    RegCloseKey(hKey);

    printf("Domain: %ls", lResult);

    return (lResult == ERROR_SUCCESS && dwSize > 1);
}

// +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ //
// TODO FOR YOU: Add environmental keying by checking if the domain matches your target. //
//               Add a --domain "domain-name" option to the packer.                      //
//               Also, there are more ways to find out if youre in a sandbox... :)       //
// +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ //