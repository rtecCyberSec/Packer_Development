// Code from here: https://github.com/mttaggart/OffensiveNotion/blob/c12d02892a1169f18c4a85c50dbfae0dfe1e20cd/agent/src/env_check.rs#L76

use windows::{
    core::{PWSTR, PCWSTR},
    Win32::{
        NetworkManagement::NetManagement::{
            NetGetJoinInformation,
            NetApiBufferFree,
            NetSetupUnknownStatus,
            NetSetupDomainName,
        },
    }
};

/// Check if the machine is joined to a domain
fn is_domain_joined() -> bool {
    let mut join_status = NetSetupUnknownStatus;
    let mut name_buffer = PWSTR(std::ptr::null_mut());

    // Check the domain join information
    if unsafe {
        NetGetJoinInformation(PCWSTR(std::ptr::null_mut()), &mut name_buffer, &mut join_status)
    } != 0 {
        return false;
    }

    // Free the buffer that `NetGetJoinInformation` allocated
    unsafe { NetApiBufferFree(name_buffer.0.cast()) };

    // Return true if the machine is joined to a domain
    join_status == NetSetupDomainName
}
