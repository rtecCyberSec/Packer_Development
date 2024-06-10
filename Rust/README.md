# The Rust Path 

So you decided to go the Rusty way for your implementation.

![Rust](https://github.com/S3cur3Th1sSh1t/maldevx33fcon/assets/27858067/9531d79d-a682-4615-ad18-3a4f6891df37)


## Getting started / setup

Download & install [Rust](https://www.rust-lang.org/tools/install) from the website. 

`cargo` will expect a `src` folder in your current working directory containing an `main.rs` file. So create this new directory and copy `packertemplate.rs` into it with the name `main.rs`.

As we are using `cargo` for dependency management, we can than simply use `cargo build --release` with the `Cargo.toml` in our current working directory. All dependencies will be installed automatically.

## How to use the code

The `packertemplate.rs` code is meant to be used as the start for automating the process of
1. Encrypting input payloads (Shellcode, C# Assemblies, PE files)
2. Building/Updating the needed Loader code
3. (Optional) Embedding the encrypted payload inside of the code
4. Compiling the loader, so that we have our final packed binary/dll

All the sub-folders contain potential features, that you can embed/integrate into the Loader. So if you want to for example start with Shellcode packing, check out the `shellcodeexecute` folder. We recommend, to use compiler flags in the loader instead of concatenating string snippets on runtime for the final code.

Encryption/decryption is always the same (which is for this reason also a good IoC and/or AV signature code base).

The provided code base avoids detections for entropy by loading the encrypted payload from a dedicated file on runtime. This also acts as Sandbox-Evasion, because most Sandboxes or Emulators tend to analyze the executable only don't embed other files for analysis.

All provided samples use `nimvoke` for indirect syscall execution. Therefore you don't need to take care for potential Userland-Hooks at all as those are bypassed.

## ToDos / Programming tasks
- :facepunch: intermediate :facepunch: : [rust_syscalls](https://github.com/janoglezcampos/rust_syscalls) is using Hells Gate instead of Tartarus Gate for SSN retrieval. This won't work against some EDRs and needs adjustments. However, there already is [public code](https://github.com/gmh5225/rust-mordor-rs/blob/main/hells_halos_tartarus_gate/src/lib.rs) to accomplish this for your help ;-)
- :v: easy :v: : Build together your first features, e.G. shellcode input with SandBox + AntiDebug checks
- :v: easy :v: : If you want to embed the encrypted payload, you need to tackle entropy. Entropy can for example be lowered down by using "encodings" such as [UUID, MAC, IP](https://github.com/joaoviictorti/RustRedOps/tree/main/Obfuscation/src).
- :v: easy :v: : As an alternative, load the encrypted payload from a dedicated file on runtime to decrypt it afterward.
- :v: easy :v: : Build Sideloading DLLs using Third Party tools such as [Koppeling](https://github.com/monoxgas/Koppeling) 
- :facepunch: intermediate :facepunch: : If you prefer to inject payloads, integrate ThreadlessInject/Poolparty
- :facepunch: intermediate :facepunch: : PE loading is not done with indirect syscalls. Change this.
- :v: easy :v: : Alternative Sandbox Evasion stubs / better AntiDebug techniques
- :facepunch: intermediate :facepunch: : Use Hardware Breakpoints for AMSI/ETW evasion instead of simple patches
- :v: easy :v: : Integrate `real` encryption instead of XOR
- :v: easy :v: : Randomize the encryption key for each packed binary
- :v: easy :v: : Module Stomping
- :v: easy :v: : Environmental keying on a target domain/hostname


## Credits & Code taken from

https://github.com/yamakadi/clroxide

https://github.com/janoglezcampos/rust_syscalls

https://github.com/Thoxy67/rspe

https://github.com/joaoviictorti/RustRedOps/
