// If some of you guys tries to load Mimikatz with this PE-Loader (We know, some will) - it fails.
// Ask us about it and we can give some hints on how to modify the source to "fix" this.

// I know, this is soo lazy :-)
use rspe::{reflective_loader};

fn peload(bytes_to_load: Vec<u8>)  {
    unsafe {
        reflective_loader(bytes_to_load.clone());

        // Using Threads (useful to bind 2nd exe to execute at the same time):
        // Currently not in use, but can be used to load the pe file in a separate thread
        // let handle = std::thread::spawn(move || {
        //     pe::loader::reflective_loader(bytes_to_load.clone());
        // });
        // let _ = handle.join();
    };
}
