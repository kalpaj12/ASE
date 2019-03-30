### ASE - 8086 Emulator
ASE is designed to act as a minimal replacement for existing aging emulators.

ASE is currently in active development and as of now supports approx 20-25 common
instructions - Move, jump, set/clear flag values etc.

Building ASE:

1) Replace `clang` with a choice of your own `C` compiler in `Makefile`.

2) Run the provided build file: `./build.sh`

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
```
Flags :
[Flag]:[Bit value] - [CF]:[1]
[Flag]:[Bit value] - [DF]:[1]
[Flag]:[Bit value] - [IF]:[0]
[Flag]:[Bit value] - [PF]:[0]

Register:
[AL] - [c]
[BX] - [2694]
[CX] - []
[DX] - []

Stack :
[Addr]:[Value] - [0x12bd6b8]:[c]

Note: Stack memory cleared.

```
