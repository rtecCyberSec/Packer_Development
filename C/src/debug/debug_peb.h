#include <windows.h>

/**
 * Checks if we are being debugged by looking at the PEB's BeingDebugged field 
 */
BOOL
IsBeingDebugged_PEB()
{
    #ifdef _M_IX86 
        PEB* pPeb = (PEB*)__readfsdword(0x30);
    #else
        PEB* pPeb = (PEB*)__readgsqword(0x60);
    #endif

    return pPeb->BeingDebugged == TRUE;
}

// +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~ //
// TODO FOR YOU: Add more and be creative. Are we named 'malware' or 'sample'?  //
//               Is wireshark running or some sysinternals tool running?        //
//               Is the hostname following the default DESKTOP-XYZ name scheme? //
// +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~ //