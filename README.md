### ASE - 8086 Emulator
ASE is designed to act as a minimal replacement for existing aging emulators.

ASE is currently in active development (as a hobby project, you are welcome to contribute!) and as of now supports approx 30-40 common
instructions - Move, jump, set/clear flag values etc.

### Building ASE:

Run the provided build script `./build.sh`.

You will need a decent C compiler (gcc/clang). ASE cannot be built for Windows as it relies on `getopt` to parse command-line arguments. However you can build ASE from within WSL.

### Running ASE:

`./ase file.asm --all-flags`

### Supported command line args
```
-a --all-flags: Enable all (below) emulator specific flags
-f : Show flag contents
-h : Show help (this) screen
-m : Show memory contents
-r : Show register contents
-s : Show stack contents
-v : Show version info
-w --no-warns: Disable warnings
```

Note: Warnings are generated for:
1. Not initialising DS or ES before use.
2. Moving to a dirty memory location (not initialised prior to using).
3. Possible register size mismatch.

Disabling warnings is not recommended.

### Sample program

```asm
;; Sample progam to demonstrate ASE.
;; Note that ASE does not really run the asm code at the h/w
;; level but instead parses the source file, tokenises it and
;; runs the parser giving you the feel that the code is running.

MOV AL, 12
MOV BX, 9876

STC
JC L1

PUSH BX
L1: PUSH AX

STD
```

### Console output
![Alt text](/docs/images/ASE.png?raw=true "Title")
