
import os

func toByteSeq*(str: string): seq[byte] {.inline.} =
  @(str.toOpenArrayByte(0, str.high))

# This is likely what you want for both encryption and decryption. The others are just examples for an alternative with a hardcoded shellcodeFile.
proc xorCrypt(data: seq[byte]): seq[byte] =
  if data.len > 0:
    # TODO: Randomise this key. Note: The key should be changed in the loader code each time according to the random value from the Packer.
    var key: seq[byte] = @[byte 0x1,byte  0x2,byte  0x3,byte  0x4]
    result = newSeq[byte](data.len)
    for i in 0 ..< data.len:
      result[i] = data[i] xor key[i mod key.len]
  else:
    echo "Data sequence is empty."

proc encryptFile(inputFile: string) =
  var data: seq[byte] = toByteSeq(readFile(inputFile))
  data = xorCrypt(data)
  writeFile(inputFile & ".enc", data)

proc decryptFile(inputFile: string): seq[byte] =
  var data: seq[byte] = toByteSeq(readFile(inputFile))
  data = xorCrypt(data)
  return data


proc encrypt() =
    # take arg[0] as input file and encrypt it
    if paramCount() > 0:
        encryptFile(paramStr(1))
    else:
        echo "No input file provided. Please provide a file to encrypt."

#[
 If you don't want to provide the path to the payload file via argument, the function needs an byte array or string with the content or an hardcoded path whatever you prefer.
 This implementation here won't work for initial access payloads but only for post exploitation.
]#
proc decrypt() = 
    # take arg[0] as input file and decrypt it
    if paramCount() > 0:
        var data: seq[byte] = decryptFile(paramStr(1))
        echo "Decrypted data: ", data
    else:
        echo "No input file provided. Please provide a file to decrypt."

