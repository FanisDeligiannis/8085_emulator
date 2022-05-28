# Binaries

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/42d5d2deb6a442cfa587e825724bbe7e)](https://app.codacy.com/gh/FunIsDangerous/8085_emulator?utm_source=github.com&utm_medium=referral&utm_content=FunIsDangerous/8085_emulator&utm_campaign=Badge_Grade_Settings)

- Linux: (ProjectPath)/bin/Release-linux-x86_64/GUI/GUI
- Windows: (ProjectPath)/bin/Release-windows-x86_64/GUI/GUI.exe

---

# Usage

### Windows
Run *GUI.exe* file, and the rest is pretty self explanatory.

OR if you want to open a file straight away:
- Using commandline: ./GUI.exe [filename]
- OR Drag and drop the file to *GUI.exe*

### Linux
- chmod +x GUI
- ./GUI [filename]


---
# Building

  

There are precompiled binaries for Windows and Debian so if it works on your system and you don't want to modify anything, you can skip this.

The binaries (precompiled and built ones) will be in
- Linux: (ProjectPath)/bin/Release-linux-x86_64/GUI/GUI
- Windows: (ProjectPath)/bin/Release-windows-x86_64/GUI/GUI.exe

## Windows

  

### Requirements:
- Modern Visual studio
  
### Steps:

- premake5.exe vs2022 (or any other [action](https://premake.github.io/docs/using-premake))

- Open the produces .sln file and build.

  

## Linux

  ### Requirements
  - Get required packages: sudo apt install libxi-dev libgl-dev
  - You also need a modern enough g++ compiler that supports C++20.

### Steps

- chmod +x premake5

- ./premake5 gmake2

- make config=release

  

## MacOS

  

It ***probably*** works on MacOS, but I have no way to test so I didn't include it. You can download [premake5 for MacOS](https://premake.github.io/download/)

  

---  

# 8085 Assembler

  

## Assembly

  

- [Basic instruction set of 8085](https://www.tutorialspoint.com/microprocessor/microprocessor_8085_instruction_sets.htm)

  

- All *Numbers* and *Addresses* can be defined as:

	-  *DEC number*

	-  *HEX number* followed by H

	-  *Binary number* followed by B
  

-  **ORG**  *Address* --- Following code / data will start at *Address*

  

-  *label*  **EQU**  *value* --- Define a *label* that gets replaced by *value*

  

-  **DB** / **DW**

	-  **DB**  *8 bit number*

	-  **DB** '*character*'

	-  **DB** "*string*"

	-  **DW**  *16 bit number*

  

- EQU **CODE** is predefined to 0800H

  

- Comments with ***;***

  

<details>

<summary>Example: Code to copy the string "Hello, world!" from memory addresses 0x0700... to memory addresses 0x0900...</summary>

  

```asm

ORG 0700H ; All data from below will start at address 0x0700

  

DB "Hello, world!"

  

ORG CODE ; Code execution begins at 0x0800

  

MVI A, 0

  

MVI H, 09H ; HL = 0x0900

MVI L, 0

  

MVI D, 07 ; DE = 0x0700

MVI E, 0

  

loop1:

XCHG ; Get the 0x0700... to HL

  

MOV A,M ; Get the letter into A

XCHG ; Get HL back to 0x0900... space

MOV M,A ; Save A there

INX H ; Increase to 0x0901...

INX D ; Increase to 0x0701...

CPI 00h ; if A = 0, means we hit '\0' and we're done.

JNZ loop1

  

HLT

```

</details>

  
  

## TODO

  

- Add MACROs

- More verbose and better errors

  

## Issues

Instruction set is complete. Mistakes are expected though.

  

---

  
  

# 8085 Emulator

  

## Instructions

- There is currently no support for interrupts and IN/OUT ports.

- Currently, all you can do is:

- Run a program and look at the memory / registers

  

---

  
  

# GUI
Using [Dear ImGui](https://github.com/ocornut/imgui). 
 
 TODO:
 - Step-by-step code execution
 - Add breakpoints
 - Add peripherals
 	- LEDs
 	- Switches
 	- LCD screen (?)
 	- Keyboard
 	- More
 - Probably more coming.
 
