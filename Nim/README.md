# The Nim Path

So you decided to go the Nim way for your implementation.

![nim](https://github.com/rtecCyberSec/Packer-Development/assets/27858067/09428c3f-b458-4d2a-84f3-b5aeecc64f3e)

## Getting started / setup

Download & install [Nim](https://nim-lang.org/install.html) from the website. Version 2.0.4 should be fine at the time of publication for this repository.

<u>Windows</u>:
Download & install [MinGW-w64](https://sourceforge.net/projects/mingw-w64/files/).

<u>Linux</u>:
`apt-get install mingw-64`

<u>Install dependencies</u>
`nimble install ptr_math checksums NimProtect winim https://github.com/nbaertsch/nimvoke`

## How to use the code

The `packertemplate.nim` code is meant to be used as the start for automating the process of
1. Encrypting input payloads (Shellcode, C# Assemblies, PE files)
2. Building/Updating the needed Loader code
3. (Optional) Embedding the encrypted payload inside of the code
4. Compiling the loader, so that we have our final packed binary/dll

All the sub-folders contain potential features, that you can embed/integrate into the Loader. So if you want to for example start with Shellcode packing, check out the `shellcodeexecute` folder. We recommend, to use compiler flags in the loader instead of concatenating string snippets on runtime for the final code - however exemplary code for both is provided.

Encryption/decryption is always the same (which is for this reason also a good IoC and/or AV signature code base).

The provided code base avoids detections for entropy by loading the encrypted payload from a dedicated file on runtime. This also acts as Sandbox-Evasion, because most Sandboxes or Emulators tend to analyze the executable only don't embed other files for analysis.

All provided samples use `nimvoke` for indirect syscall execution. Therefore you don't need to take care for potential Userland-Hooks at all as those are bypassed.

## ToDos / Programming tasks

- :v: easy :v: : Build together your first features, e.G. shellcode input with SandBox + AntiDebug checks
- :v: easy :v: : If you want to embed the encrypted payload, you need to tackle entropy. Entropy can for example be lowered down by using "encodings" such as [UUID, MAC, IP](https://github.com/SaadAhla/Shellcode-Hide/tree/main/2%20-%20Encoding). These examples need to be adjusted for Nim arrays.
- :v: easy :v: : As an alternative, load the encrypted payload from a dedicated file on runtime to decrypt it afterward.
- :v: easy :v: : Build Sideloading DLLs using Third Party tools such as [Koppeling](https://github.com/monoxgas/Koppeling) or [Custom definition file generation](https://github.com/byt3bl33d3r/NimDllSideload)
- :facepunch: intermediate :facepunch: : If you prefer to inject payloads, integrate ThreadlessInject/Poolparty
- :facepunch: intermediate :facepunch: : PE loading is not done with indirect syscalls. Change this.
- :v: easy :v: : Alternative Sandbox Evasion stubs / better AntiDebug techniques
- :facepunch: intermediate :facepunch: : Use Hardware Breakpoints for AMSI/ETW evasion instead of simple patches
- :v: easy :v: : Integrate `real` encryption instead of XOR
- :v: easy :v: : Module Stomping
- :v: easy :v: : Environmental keying on a target domain/hostname


## Credits & Code taken from

https://github.com/nbaertsch/nimvoke

https://github.com/khchen/winim
