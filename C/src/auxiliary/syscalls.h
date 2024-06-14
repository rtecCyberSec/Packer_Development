#pragma once

#include <windows.h>
#include <stdio.h>
#include <ntdef.h>

// --------------------------------------

/**
 * Implementation of syscall sorting by @eversinc33
 * 
 * Syscall sorting works by sorting all Zw* syscall wrappers in ntdll.dll by their address
 * and then assigning incrementing SSNs. These can then be called as indirect syscalls, as implemented in "Gate.asm".
 */

typedef enum {
    ViewUnmap = 2
} SECTION_INHERIT;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID    Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef LONG KPRIORITY;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation = 0,
    SystemPerformanceInformation = 2,
    SystemTimeOfDayInformation = 3,
    SystemProcessInformation = 5,
    SystemProcessorPerformanceInformation = 8,
    SystemInterruptInformation = 23,
    SystemExceptionInformation = 33,
    SystemRegistryQuotaInformation = 37,
    SystemLookasideInformation = 45
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    BYTE Reserved1[48];
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    PVOID Reserved2;
    ULONG HandleCount;
    ULONG SessionId;
    PVOID Reserved3;
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG Reserved4;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    PVOID Reserved5;
    SIZE_T QuotaPagedPoolUsage;
    PVOID Reserved6;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER Reserved7[6];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

typedef struct _SYSCALL_ENTRY {
    INT syscallNumber;
    PVOID syscallInstructionAddress;
    CHAR syscallName[MAX_PATH];
} SYSCALL_ENTRY;

// --------------------------------------

// syscall sorting table
#define SYSCALL_TABLE_SIZE 1024
SYSCALL_ENTRY* syscallTable[SYSCALL_TABLE_SIZE] = { 0 };

VOID PrepareSyscall(DWORD dwSycallNr, PVOID dw64Gate);

// --------------------------------------
// Syscalls to resolve

SYSCALL_ENTRY* _NtProtectVirtualMemory;
SYSCALL_ENTRY* _NtAllocateVirtualMemory;

// NtProtectVirtualMemory
NTSTATUS WINAPI Syscall_NtProtectVirtualMemory(
    HANDLE ProcessHandle, 
    PVOID *BaseAddress, 
    SIZE_T *NumberOfBytesToProtect, 
    ULONG NewAccessProtection, 
    PULONG OldAccessProtection
);

// NtAllocateVirtualMemory
NTSTATUS WINAPI Syscall_NtAllocateVirtualMemory(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType,
    ULONG Protect
);

// --------------------------------------

/**
 * Sorting function used internally for qsort() 
 */
INT 
_CompareSyscallEntries(
    const void* a, 
    const void* b
) 
{
    SYSCALL_ENTRY* entryA = *((SYSCALL_ENTRY**)a);
    SYSCALL_ENTRY* entryB = *((SYSCALL_ENTRY**)b);
    return (entryA->syscallInstructionAddress - entryB->syscallInstructionAddress);
}

/**
 * Gets Zw* syscall wrappers from ntdll.dll, sorts them and populates the SYSCALL_ENTRY** syscallTable.
 */
VOID 
PopulateSyscallMap(
    HMODULE ntdllBaseAddr
)
{
    PCHAR pBaseAddr = (PCHAR)ntdllBaseAddr;

    // get pointers to main headers/structures
    IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pBaseAddr;
    IMAGE_NT_HEADERS* pNTHdr = (IMAGE_NT_HEADERS*)(pBaseAddr + pDosHdr->e_lfanew);
    IMAGE_OPTIONAL_HEADER* pOptionalHdr = &pNTHdr->OptionalHeader;
    IMAGE_DATA_DIRECTORY* pExportDataDir = (IMAGE_DATA_DIRECTORY*)(&pOptionalHdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
    IMAGE_EXPORT_DIRECTORY* pExportDirAddr = (IMAGE_EXPORT_DIRECTORY*)(pBaseAddr + pExportDataDir->VirtualAddress);

    // resolve addresses to Export Address Table, table of function names and "table of ordinals"
    DWORD* pEAT = (DWORD*)(pBaseAddr + pExportDirAddr->AddressOfFunctions);
    DWORD* pFuncNameTbl = (DWORD*)(pBaseAddr + pExportDirAddr->AddressOfNames);
    WORD* pHintsTbl = (WORD*)(pBaseAddr + pExportDirAddr->AddressOfNameOrdinals);

    INT currentSyscallNumber = 0;

    // parse through table of function names
    for (INT i=0; i<pExportDirAddr->NumberOfNames; ++i) 
    {
        PCHAR sTmpFuncName = (PCHAR)pBaseAddr + (DWORD_PTR)pFuncNameTbl[i];

        if (sTmpFuncName[0] == 'Z' && sTmpFuncName[1] == 'w') 
        {
            // found a syscall
            PCHAR syscallInstructionAddr = 0;
            PCHAR pProcAddr = pBaseAddr + (DWORD_PTR)pEAT[pHintsTbl[i]];

            int offset = 0;
            while(TRUE) 
            {
                if (*(DWORD*)(pProcAddr + offset) == 0x050f0375)
                {
                    syscallInstructionAddr = pProcAddr + offset + 2; 
                    break;
                }
                offset++;
            } 

            // create syscall table entry
            SYSCALL_ENTRY* currentSyscall = (SYSCALL_ENTRY*)malloc(sizeof(SYSCALL_ENTRY));
            currentSyscall->syscallNumber = 0; // will be filled later when sorting
            currentSyscall->syscallInstructionAddress =  syscallInstructionAddr;
            RtlCopyMemory(currentSyscall->syscallName, sTmpFuncName, strlen(sTmpFuncName) + 1);
            syscallTable[currentSyscallNumber] = currentSyscall;
            currentSyscallNumber++;
        }
    }
  
    // sort by address
    qsort(syscallTable, currentSyscallNumber, sizeof(SYSCALL_ENTRY*), _CompareSyscallEntries);

    // assign numbers by order 
    for (INT i=0; i < currentSyscallNumber; ++i)
    {
        syscallTable[i]->syscallNumber = i;
    }
}

/**
 * Get a pointer to a SYSCALL_ENTRY from the sorted syscall table
 */
SYSCALL_ENTRY* 
GetSyscallEntry(
    PCHAR name
)
{
    for (INT i=0; i<SYSCALL_TABLE_SIZE; ++i)
    {
        SYSCALL_ENTRY* entry = syscallTable[i];
        if (strcmp(entry->syscallName, name) == 0)
        {
            return entry;
        }
    }

    printf("ERROR RESOLVING %s\n", name);
    exit(1);
}

/**
 * Free memory allocated for the SYSCALL_ENTRY structs
 */
VOID 
FreeSyscallMap()
{
    for (INT i=0; i<SYSCALL_TABLE_SIZE; ++i)
    {
        SYSCALL_ENTRY* entry = syscallTable[i];
        free(entry);
    }
}
