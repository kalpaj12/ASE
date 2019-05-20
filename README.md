### ASE - 8086 Emulator
ASE is designed to act as a minimal replacement for existing aging emulators.

ASE is currently in active development (as a hobby project, you are welcome to contribute!) and as of now supports approx 30-40 common
instructions - Move, jump, set/clear flag values etc.

### Building ASE:

Run the provided build script `./build.sh`.

Building ASE generates some warnings (not the C language's warnings, but the functions in the ASE itself generate them). You can silence them by executing `export DIW=TRUE` before running the build script. Note that you might need to `unset` it before running your programs or else your runtime warnings will be silenced too.

### Compiler support:
You will need a decent C compiler. ASE relies on compiler specific functions such as `__builtin_popcount` which are not standardised. Any fairly decent compiler such as GCC or Clang should support them.

If you wish to build ASE for Windows, you'll need a GCC port or WSL. ASE remains un-tested for GCC ports.

### Running ASE:

`./ase file.asm --all-flags`

### Supported command line args
```
-a : Enable all (below) emulator specified flags
-d : Enable debug mode
-f : Show flag contents
-h : Show help (this) screen
-m : Show memory contents
-r : Show register contents
-s : Show stack contents
-v : Show version info
```

Note: Warnings are generated for:
1. Not initialising DS or ES before use.
2. Moving to a dirty memory location (not initialised prior to using).
3. Possible register size mismatch.

Disabling warnings is not recommended.

### Sample program

```asm
ORG 100h

MOV AL, 12         ; This is a comment line!
MOV BX, 9876

STC
JC L1

PUSH BX
L1: PUSH AX

STD
```

### Console output
![Alt text](/docs/images/ASE.png?raw=true "Title")
