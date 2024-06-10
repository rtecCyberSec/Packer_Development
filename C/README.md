# The C Path

So you decided to go the C way for your implementation.

<img src="https://github.com/rtecCyberSec/Packer-Development/assets/51821028/4ac96d92-418a-465d-9f76-de3b2e74b981" width="200">

## Getting started / setup

Install the packages needed for compiling and running the packer.py script:

`apt install -y gcc-mingw-w64 nasm python3`

## How to use the code

The `packer.py` code is meant to be used as the start for automating the process of
1. Encrypting input payloads (Shellcode, C# Assemblies, PE files)
2. Creating `defines.h` which holds the parameters that decide whether include certain code snippets into our loader or not 
4. Compiling the loader, so that we have our final packed PE

All the sub-folders contain potential features.

Syscall sorting is already implemented for you for indirect syscall execution. Therefore you don't need to take care for potential Userland-Hooks, as those are bypassed via indirect syscalls. Some functions here use regular Win32 API calls, so you might want to change that ;)

There are `// TODO for you: [...]` blocks scattered all over the code snippets. These give you some hints on what you can implement. Alternatively, the below items can be a starting point:

## ToDos / Programming tasks

- :v: easy :v: : Check out `pack.py` to familiarize yourself with the code and fill in the todos
- :v: easy :v: : To lower entropy, and additionally evade some sandboxes, implement storing the payload in a separate file
- :v: easy :v: : Automate the building of Sideloading DLLs using Third Party tools such as [Koppeling](https://github.com/monoxgas/Koppeling) 
- :facepunch: intermediate :facepunch: : If you prefer to inject payloads, integrate ThreadlessInject/Poolparty
- :v: easy :v: : Alternative Sandbox Evasion / AntiDebug techniques
- :v: easy :v: : Adjust `helpers.h` to use API Hashing and Salting
- :facepunch: intermediate :facepunch: : Use Hardware Breakpoints for AMSI/ETW evasion instead of simple patches
- :v: easy :v: : Add Module Stomping
- :v: easy :v: : Add an option for creating a service binary for lateral movement execution
- :v: easy :v: : Environmental keying on a target domain/hostname
- 😠 hard 😠 : Adjust .NET execution to add reading output and passing arguments

## Credits & Code taken from

PE Injection is an adjusted version of: 

https://github.com/aaaddress1/RunPE-In-Memory/tree/master
