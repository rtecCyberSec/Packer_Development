use std::env;
use std::fs::File;
use std::io::prelude::*;

fn main() {

    // print an ASCII art
    println!(r#"
    ---------------------------
    Most important TODO for you
    < INSERT ASCII BANNER HERE >
    ---------------------------
    "#);

    let args: Vec<String> = env::args().collect();

    let mut do_message_box = false;
    let mut do_calculation = false;
    let mut embedded_payload = true;
    let mut input_file = "".to_string();
    let mut shellcode_file = "".to_string();
    

    /* 

    This way of storing the loader code has advantages and downsides. Storing it on disk would make it much easier for you to maintain the code.
    But you would also need to copy all dependency files to the Payload gen system, so it's a tradeoff.

    For this workshop, faster coding and easier maintenance is better, so you should uncomment the code below and use the one from above.
    Reading comments is important, as it can save you a lot of time and effort. ;-)

    */
    let mut loader_stub = String::new();
    let mut file = File::open("template.rs").expect("Unable to open file");
    file.read_to_string(&mut loader_stub).expect("Unable to read file");
    
    /* Uncommented, as this is meant to be used as alternative
    let loader_imports : String = r#"
    // TODO: Add all other nessesary imports from the snippets you're using here
    // The existing ones can be removed obviously, as this is exemplary code ;-)

    use windows::Win32::UI::WindowsAndMessaging::{MessageBoxA, MB_OK};
    use windows::core::{PSTR, s};
    use std::ffi::c_void;
    
    #[cfg(feature = "embedded")]
    const ENCPAYLOAD: &[u8] = &[];  // replace with the encrypted payload data

    fn main() {

    "#.to_string();

    let loader_rs : String = r#"
    
    // TODO: Add all the code snippets here, that you want to use in the loader with corresponding feature flags

        #[cfg(feature = "messagebox")]
        unsafe {
            MessageBoxA(
                None,
                s!("Hello World"),
                s!("Hello"),
                MB_OK,
            );
        }

        #[cfg(feature = "calculation")]
        fn calculate()
        {
            let mut result = 0;
            for i in 0..10000 {
                result += i;
            }
            println!("Result: {}", result);
        }
        #[cfg(feature = "calculation")]
        calculate();
    "#.to_string();

    let main_close: String = r#"
    }
    "#.to_string();
    */
    
    // In a long term run, this argument handling will likely suck, escpecially with more and more features. 
    // A library could instead help out here later.
    for i in 0..args.len() {
        match args[i].as_str() {
            // TODO: remove/replace the exemplary values with the ones you need/want for your packer - regarding on your feature choice.
            "--messageBox" => do_message_box = true,
            "--randomCalculation" => do_calculation = true,
            "--shellcodeFile" if i < args.len() - 1 => shellcode_file = args[i + 1].clone(), // file to use for the encrypted payload, this disables embedded feature
            "--input" if i < args.len() - 1 => input_file = args[i + 1].clone(),
            _ => {}
        }
    }

    if shellcode_file != "" {
        embedded_payload = false;
    }

    /* Uncommented, as this is meant to be used as alternative
    let mut loader_stub = String::new();
    loader_stub.push_str(&loader_imports);
    loader_stub.push_str(&loader_rs);
    loader_stub.push_str(&main_close);
    */

    // TODO: You may need more compiler flags later on
    let mut compile_command = " build --release ".to_string();
    if do_message_box {
        // TODO: Replace with actual needed feature names and add new ones for each feature you want to use
        compile_command.push_str("--features messagebox ");
    }
    if do_calculation {
        compile_command.push_str("--features calculation ");
    }
    if embedded_payload {
        compile_command.push_str("--features embedded ");
    }
    else
    {
        compile_command.push_str("--features payloadFile ");
    }

    if input_file != "" {
        println!("[*] Input file: {}", input_file);
        // read input_file from disk and overwrite "const ENCPAYLOAD: &[u8] = &[];" from loader_stub with the content of the file
        let mut file = File::open(input_file).expect("Unable to open file");
        let mut data: Vec<u8> = Vec::new();
        file.read_to_end(&mut data).expect("Unable to read file");
        // encrypt content
        // -> ToDo for you ;-)
        // overwrite the content of ENCPAYLOAD with the encrypted data
        if embedded_payload {
            loader_stub = loader_stub.replace("const ENCPAYLOAD: &[u8] = &[];", &format!("const ENCPAYLOAD: &[u8] = &{:?};", data));
        }
        else
        {
            println!("[*] Shellcode file: {}", shellcode_file);
            // TODO: If you want to not embed the payload, but toload it from disk or a remote webserver or from somewhere else, "const ENCPAYLOAD: &[u8] = &[];" should be removed completely
            // And a function to load the payload from the desired location should be implemented instead.            
            loader_stub = loader_stub.replace("const ENCPAYLOAD: &[u8] = &[];", "");
            // Than, you should also write the encrypted content to disk, so that the operator (you?) can place it accordingly with the loader before execution.
        }
        
        compile_command.push_str("--features embedded ");
    }
    else
    {
        println!("Please provide an input file with --input <filename>");
        return;
    }
    
    compile_command.push_str(" --manifest-path ./loader/Cargo.toml");

    #[cfg(target_os = "linux")]
    compile_command.push_str(" --target x86_64-pc-windows-gnu");

    // create a new subdirectory ./loader - we need to create a new directory for the loader source code plus
    // add a Cargo.toml file to it
    std::fs::create_dir_all("loader").expect("Unable to create directory");
    // create another subdirectors ./loader/src
    std::fs::create_dir_all("loader/src").expect("Unable to create directory");

    // TODO: if you want to create a loader DLL, you need to save the file as loader/src/lib.rs instead of loader/src/main.rs
    // Also make sure to delete old existing files, as otherwise cargo will fail to compile with main.rs and lib.rs being there at the same time.
    let mut file = File::create("./loader/src/main.rs").expect("Unable to create file");
    file.write_all(loader_stub.as_bytes()).expect("Unable to write data");


    println!("Compile command: {}", compile_command);

    // compile the loader
    let mut path_to_cargo_project = std::env::current_dir().unwrap();
    compiler(&mut path_to_cargo_project, &compile_command).expect("Failed to compile loader");
}

use std::env::set_current_dir;
use std::path::PathBuf;

fn compiler(path_to_cargo_project: &mut PathBuf, compile_command: &String) -> Result<(), Box<dyn std::error::Error>> {
    let path_to_cargo_folder = path_to_cargo_project.clone();
    set_current_dir(&path_to_cargo_folder)?;
    let output = std::process::Command::new("cargo")
        .env("CFLAGS", "-lrt")
        .env("LDFLAGS", "-lrt")
        .env("RUSTFLAGS", "-C target-feature=+crt-static") // without linking here, the payload might not execute on systems where the corresponding DLL is missing
        .env("RUSTFLAGS", "-A warnings")
        .args(compile_command.split_whitespace())
        .output()?;


    println!("stderr: {}", String::from_utf8_lossy(&output.stderr));
    
    if output.status.success() {
        return Ok(());
    }
    else
    {
        println!("[-] Failed to compile!\r\n\r\n");
        let error_message = String::from_utf8_lossy(&output.stderr);
        return Err(Box::new(std::io::Error::new(std::io::ErrorKind::Other, error_message.to_string())));
    }
}
