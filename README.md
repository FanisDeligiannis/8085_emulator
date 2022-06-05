# Binaries

- Linux: (ProjectPath)/bin/Release-linux-x86_64/GUI/GUI
- Windows: (ProjectPath)/bin/Release-windows-x86_64/GUI/GUI.exe

# Examples

There are lots of examples in the "examples/" folder.

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

  

- Predefined EQU:
	- RST0 EQU 0000H
	- RST1 EQU 0008H
	- RST2 EQU 0010H
	- RST3 EQU 0018H
	- RST4 EQU 0020H
	- RST5 EQU 0028H
	- RST6 EQU 0030H
	- RST7 EQU 0038H
	- RST45 EQU 0024H
	- RST55 EQU 002CH
	- RST65 EQU 0034H
	- RST75 EQU 003CH
	- CODE EQU 0800H

- Comments with ***;***

- Using interrupts:
	- Enable interrupts. (**EI**)
	- Set the interrupt mask appropriately (**SIM**)
	- For INTR, just define "***INTR_ROUTINE***" label and it'll be called upon INTR. Examples in "examples/"
	- For all other interrupts, follow example bellow.
	- By default, all interrupts, except RST0, are a single RET instruction. Overwrite it to JMP to your code instead.
	- By default RST0 jumps to 0800H.

```asm
ORG RST1
	JMP LABEL_NAME
	
ORG CODE

LABEL_NAME:
	; instructions
	RET 
```

  

## TODO

  

- Add MACROs

- More verbose and better errors



## Issues

Instruction set is complete. Mistakes are expected though.




---




# GUI
Using [Dear ImGui](https://github.com/ocornut/imgui). 
 
 TODO:
 - Step-by-step code execution - Done
 - Add breakpoints - Done. To use breakpoints, click the line count number.
 - Add peripherals
 	- LEDs - Done 
 	- Switches - Done
	- Beep - Done 
 	- 7 Segment Display - Done
 	- LCD screen (?)
 	- Keyboard - Done
 	- More
- Greek characters support (?)
 - Probably more coming.
 
