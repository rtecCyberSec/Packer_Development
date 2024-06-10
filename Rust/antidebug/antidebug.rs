// taken from here: https://github.com/joaoviictorti/RustRedOps/blob/main/Anti_Debug/src/main.rs
// If you want to integrate some more advanced Anti-Debug checks:
// https://github.com/LordNoteworthy/al-khaser/tree/master/al-khaser/AntiDebug || https://www.unprotect.it/category/anti-debugging/ 

use std::arch::asm;
use windows::Win32::System::Threading::{GetCurrentThread, PEB, TEB};
use windows::Win32::System::Kernel::NT_TIB;

fn is_debugger_peb() {
    unsafe {
        let peb = get_peb();
        if (*peb).BeingDebugged == 1 {
            println!("[!] Debugger Detected! [2]");
        }
    }
}

// Function to recover PEB
unsafe fn get_peb() -> *mut PEB {
    let teb_offset = ntapi::FIELD_OFFSET!(NT_TIB, Self_) as u32;

    #[cfg(target_arch = "x86_64")]
    {
        let teb = __readgsqword(teb_offset) as *mut TEB;
        return (*teb).ProcessEnvironmentBlock;
    }

    #[cfg(target_arch = "x86")]
    {
        let teb = __readfsdword(teb_offset) as *mut TEB;
        return (*teb).ProcessEnvironmentBlock;
    }
}

#[cfg(target_arch = "x86_64")]
unsafe fn __readgsqword(offset: u32) -> u64 {
    let output: u64;
    asm!(
        "mov {}, gs:[{:e}]",
        lateout(reg) output,
        in(reg) offset,
        options(nostack, pure, readonly),
    );
    output
}

#[cfg(target_arch = "x86")]
unsafe fn __readfsdword(offset: u32) -> u32 {
    let output: u32;
    asm!(
        "mov {:e}, fs:[{:e}]",
        lateout(reg) output,
        in(reg) offset,
        options(nostack, pure, readonly),
    );
    output
}
