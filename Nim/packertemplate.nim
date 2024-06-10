import strutils
import strformat
import os

echo """

--------------------------------------
Most important TODO for you
< INSERT ASCII BANNER HERE >
--------------------------------------

"""

func toByteSeq*(str: string): seq[byte] {.inline.} =
  @(str.toOpenArrayByte(0, str.high))


#[ 

The idea here is to load the loader template code from disk - maybe do some modifications on runtime and re-drop it as loader.nim.
To choose which features will get executed we'll set different compiler flags.

]#

proc xorCrypt(data: seq[byte]): seq[byte] =
  if data.len > 0:
    # TODO: Randomise this key. Note: The key should be changed in the loader code each time according to the random value from the Packer.
    echo "You have to encrypt here"
    return data

# As slurp embeds the content in the packer on compile time, you should re-compile after doing modifications to template.nim
const full_stub = slurp"template.nim"


# TODO: Add compiler flags, you want to use for the loader
var cmd = "nim c -d:release --opt:speed --out:loader.exe "

import sequtils

var 
    input_file = ""
    embed = true
    shellcodeFile = ""
    encryptedPayload: seq[byte]
    inputBytes: seq[byte]

# Query user options for --messageBox or --randomCalculation
# Late TODO: using a library for argument parsing might be more suitable, as with more features this will likely get exhausting ;-)
let args = commandLineParams()

if args.contains("--messageBox"):
    cmd = cmd & "-d:USE_MESSAGEBOX "

if args.contains("--randomCalculation"):
    cmd = cmd & "-d:USE_CALCULATION "

if args.contains("--input"):
    let inputIndex = args.find("--input")
    if inputIndex != -1 and inputIndex < args.len - 1:
        input_file = args[inputIndex + 1]

# No input file, nothing todo for a Packer.
if input_file == "":
    echo "Please specify a payload file with --input <file>"
    quit(1)

if args.contains("--shellcodeFile"):
    embed = false
    let shellcodeIndex = args.find("--shellcodeFile")
    if shellcodeIndex != -1 and shellcodeIndex < args.len - 1:
        shellcodeFile = args[shellcodeIndex + 1]

if embed:
    cmd = cmd & "-d:embedded "

# TODO: actually encrypt the input_file with the given function and either store it in a variable or write it to a file depending on the user options
inputBytes = toByteSeq(readFile(input_file))
if (embed):
    # Embedding it as int sequence will lead to long compile time for bigger payloads. As an alternative, the
    # encrypted payload could be written to disk and embedded in the loader template via slurp"filename.bin"
    # than, it's stored as string and has to be converted to byte sequence for decryption.
    # However, storing it as int sequence will lead to a very low entropy and bigger resulting files ;-) Your choice.
    encryptedPayload = xorCrypt(inputBytes)
else:
    writeFile(shellcodeFile, xorCrypt(inputBytes))

# TODO: UUID, MAC or other encodings as alternative to the int sequence used here.
let embedded_payload = fmt"""{encryptedPayload}"""

var final_stub = ""
if(embed):
    # replace the placeholder with the actual encrypted payload
    final_stub = full_stub.replace("REPLACE_encryptedPayload", embedded_payload)
else:
    final_stub = full_stub.replace("REPLACE_shellcodeFile", shellcodeFile)
    # replace REPLACE_encryptedPayload with an empty byte sequence
    final_stub = final_stub.replace("REPLACE_encryptedPayload", "toByteSeq(\"\")")


# write the loader to a file
let loader_file = "loader.nim"
writeFile(loader_file, final_stub)

# compile the loader
cmd = cmd & loader_file

echo "Compiling loader with command: ", cmd
discard os.execShellCmd(cmd)
