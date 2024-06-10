# If you want to integrate some more advanced Anti-Debug checks:
# https://github.com/LordNoteworthy/al-khaser/tree/master/al-khaser/AntiDebug || https://www.unprotect.it/category/anti-debugging/ 


when defined(WIN64):
  const
    PEB_OFFSET* = 0x30
else:
  const
    PEB_OFFSET* = 0x60

type
  ND_LDR_DATA_TABLE_ENTRY* {.bycopy.} = object
    InMemoryOrderLinks*: LIST_ENTRY
    InInitializationOrderLinks*: LIST_ENTRY
    DllBase*: PVOID
    EntryPoint*: PVOID
    SizeOfImage*: ULONG
    FullDllName*: UNICODE_STRING
    BaseDllName*: UNICODE_STRING

  PND_LDR_DATA_TABLE_ENTRY* = ptr ND_LDR_DATA_TABLE_ENTRY
  ND_PEB_LDR_DATA* {.bycopy.} = object
    Length*: ULONG
    Initialized*: UCHAR
    SsHandle*: PVOID
    InLoadOrderModuleList*: LIST_ENTRY
    InMemoryOrderModuleList*: LIST_ENTRY
    InInitializationOrderModuleList*: LIST_ENTRY

  PND_PEB_LDR_DATA* = ptr ND_PEB_LDR_DATA
  ND_PEB* {.bycopy.} = object
    Reserved1*: array[2, BYTE]
    BeingDebugged*: BYTE
    Reserved2*: array[1, BYTE]
    Reserved3*: array[2, PVOID]
    Ldr*: PND_PEB_LDR_DATA

  PND_PEB* = ptr ND_PEB

proc RtlGetCurrentPeb*(): pointer 
    {.discardable, stdcall, dynlib: "ntdll", importc: "RtlGetCurrentPeb".}

proc GetPPEB * (p: culong): P_PEB = 
  return cast[P_PEB](RtlGetCurrentPeb())

proc AmIDebugged*(): bool =
    var Peb: PPEB = GetPPEB(PEB_OFFSET)
    var BeingDebugged = bool(Peb.BeingDebugged)
    if (BeingDebugged):
      echo "This program prints unicorns!"
      echo """
               _.
            ;=',_
           S" .--`
          sS  \\__
       __.' ( \\-->
    _=/    _./-/\\
   ((\\( /-'   -'l
    ) |/ \\\\
snd  \\\\  \\
        `~ `~
"""
      return true
    else:
      return false
