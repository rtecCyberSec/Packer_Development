section .text

global PrepareSyscall
PrepareSyscall:
    xor r11, r11  ; set r11 to 0
    xor r10, r10  ; set r10 to 0
    mov r11, rcx  ; move argument 1 (SSN) into r11
    mov r10, rdx  ; move argument 2 (Syscall Instruction address) into r10
    ret

global Syscall_NtAllocateVirtualMemory
Syscall_NtAllocateVirtualMemory:
    push r10
    xor rax, rax
    mov r10, rcx
    mov eax, r11d
    ret

global Syscall_NtProtectVirtualMemory
Syscall_NtProtectVirtualMemory:
    push r10
    xor rax, rax
    mov r10, rcx
    mov eax, r11d
    ret

; add more when needed