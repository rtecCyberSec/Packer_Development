import dynlib
import winim/lean

proc NimMain() {.cdecl, importc.}

# recommentation is to not execute from DllMain here, as than you will more likely face LoaderLocks
proc EXPORTNAME(hinstDLL: HINSTANCE, fdwReason: DWORD, lpvReserved: LPVOID): BOOL {.stdcall,exportc, dynlib.} =
    NimMain()
    # Create a Thread on the main function, only needed if the payload is not executed from within NimMain()
    var threadHandle = CreateThread(NULL, 0, main, NULL, 0, NULL)
    CloseHandle(threadHandle)
    
    return true
