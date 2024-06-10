use windows::Win32::System::SystemInformation::GetTickCount;
use rand::Rng;

fn random_calculations(duration: u32) {
    let start_time = unsafe { GetTickCount() };
    let mut rng = rand::thread_rng();

    while unsafe { GetTickCount() } - start_time < duration {
        let a: u32 = rng.gen_range(1..100);
        let b: u32 = rng.gen_range(1..100);
        let _ = a + b;  // Do a random calculation
    }
}
