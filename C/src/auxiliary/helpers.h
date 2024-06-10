#pragma once

#include "peb.h"

/**
 * Custom implementation of GetModuleHandle.
 * 
 * Walks PEB->Ldr to find the given module. 
 */
HMODULE 
WINAPI 
HlpGetModuleHandle(
    LPCWSTR moduleName
)
{
#ifdef _M_IX86 
    PEB* pPeb = (PEB*)__readfsdword(0x30);
#else
    PEB* pPeb = (PEB*)__readgsqword(0x60);
#endif

    if (moduleName == NULL)
        return (HMODULE)(pPeb->ImageBaseAddress);

    PEB_LDR_DATA* Ldr           = pPeb->Ldr;
    LIST_ENTRY* ModuleList      = NULL;
    ModuleList                  = &Ldr->InMemoryOrderModuleList;
    LIST_ENTRY* pStartListEntry = ModuleList->Flink;

    // start from beginning of InMemoryOrderModuleList and walk all list entries
    for (LIST_ENTRY* pListEntry = pStartListEntry;          
        pListEntry != ModuleList;
        pListEntry = pListEntry->Flink) 
    {

        // get current Data Table Entry
        LDR_DATA_TABLE_ENTRY* pEntry = (LDR_DATA_TABLE_ENTRY*)((BYTE*)pListEntry - sizeof(LIST_ENTRY));

        // check if module is found and return its base address
        if (strcmp((const PCHAR)pEntry->BaseDllName.Buffer, (const PCHAR)moduleName) == 0)
            return (HMODULE)pEntry->DllBase;
    }

    return NULL;

}

/**
 * Custom implementation of GetProcAddress.
 * 
 * Walks EAT of a given module to resolve a function by name.
 * Forwarded functions are not resolved. 
 */
FARPROC 
WINAPI 
HlpGetProcAddress(
    HMODULE hMod, 
    PCHAR sProcName
)
{
    PCHAR pBaseAddr = (PCHAR)hMod;

    // +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~++~+ //
    // TODO FOR YOU: Replace this approach with API Hashing+Salting //
    // +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~++~+ //
    
    // get pointers to headers and directories
    IMAGE_DOS_HEADER* pDosHdr              = (IMAGE_DOS_HEADER*)pBaseAddr;
    IMAGE_NT_HEADERS* pNTHdr               = (IMAGE_NT_HEADERS*)(pBaseAddr + pDosHdr->e_lfanew);
    IMAGE_OPTIONAL_HEADER* pOptionalHdr    = &pNTHdr->OptionalHeader;
    IMAGE_DATA_DIRECTORY* pExportDataDir   = (IMAGE_DATA_DIRECTORY*)(&pOptionalHdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
    IMAGE_EXPORT_DIRECTORY* pExportDirAddr = (IMAGE_EXPORT_DIRECTORY*)(pBaseAddr + pExportDataDir->VirtualAddress);

    // resolve addresses to Export Address Table, table of function names and table of ordinals
    DWORD* pEAT         = (DWORD*)(pBaseAddr + pExportDirAddr->AddressOfFunctions);
    DWORD* pFuncNameTbl = (DWORD*)(pBaseAddr + pExportDirAddr->AddressOfNames);
    WORD* pHintsTbl     = (WORD*)(pBaseAddr + pExportDirAddr->AddressOfNameOrdinals);

    // function address we're looking for
    FARPROC pProcAddr   = NULL;

    // resolve function by ordinal
    if (((DWORD_PTR)sProcName >> 16) == 0) 
    {
        WORD ordinal = (WORD)sProcName & 0xFFFF;    // convert to WORD
        DWORD Base   = pExportDirAddr->Base;        // first ordinal number

        // check if ordinal is not out of scope
        if (ordinal < Base || ordinal >= Base + pExportDirAddr->NumberOfFunctions)
            return NULL;

        // get the function virtual address = RVA + BaseAddr
        pProcAddr = (FARPROC)(pBaseAddr + (DWORD_PTR)pEAT[ordinal - Base]);
    }
    // resolve function by name
    else 
    {
        // parse through table of function names
        for (DWORD i = 0; i < pExportDirAddr->NumberOfNames; ++i) 
        {
            PCHAR sTmpFuncName = (PCHAR)pBaseAddr + (DWORD_PTR)pFuncNameTbl[i];

            if (strcmp(sProcName, sTmpFuncName) == 0) 
            {
                // found, get the function virtual address = RVA + BaseAddr
                pProcAddr = (FARPROC)(pBaseAddr + (DWORD_PTR)pEAT[pHintsTbl[i]]);
                break;
            }
        }
    }

    // forwarded functions are not resolved
    return pProcAddr;
}
