# Building

## Windows

- premake5.exe [action](https://premake.github.io/docs/using-premake)
- Visual studio

## Linux

- ./premake5 gmake2
- make

## MacOS

It ***probably*** works on MacOS, but I have no way to test so I didn't include it. You can download [premake5 for MacOS](https://premake.github.io/download/) 

---

# Binaries

In the /bin/ folder you can find binaries for windows, linux under */GUI*


# 8085 Assembler

## Assembly

- [Basic instruction set of 8085](https://www.tutorialspoint.com/microprocessor/microprocessor_8085_instruction_sets.htm)

- All *Numbers* and *Addresses* can be defined as:
	- *DEC number*
	- *HEX number* followed by H
	- *Binary number* followed by B

- **ORG** *Address* --- Following code / data will start at *Address*

-	*label* **EQU** *value* --- Define a *label* that gets replaced by *value*

- **DB** / **DW**
	- **DB** *8 bit number*
	- **DB** '*character*'
	- **DB** "*string*"
	- **DW** *16 bit number*

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
	-  Run a program and look at the memory / registers

---


# GUI

Not much of a GUI *yet*. Currently only serves as a startup project.

[Dear ImGui](https://github.com/ocornut/imgui) implementation coming soon.

*Usage*: ./GUI source.8085

---


# Code

- Most important includes: 
	- ```"assembler.h"```
	- ```"memory.h"```
	- ```"cpu.h"```

- Assembler
	- ```Assembler::GetAssembledMemory(std::string file)```
	- ```Assembler::SaveAssembledMemory(std::string inFileName, std::string outFileName)```

- Emulator
	- ```CPU::CPU(Memory* memory)```
	- ```CPU::CPU(uint8_t* memory, size_t size)```
	- ```std::thread CPU::Run()```

***Note*** More clarity in function names and documentation coming soon.

<details>
<summary>Example</summary>
	
```c++
uint8_t* memory_data = Assembler::GetAssembledMemory("source.8085"); // Create initial binary of a program

CPU* cpu = new CPU(memory_data, 0xffff); // Load it into the CPU

std::thread t = cpu->Run(); // Start the CPU thread

t.join(); // Wait until CPU is done 

printf("Register A: %hhu" , cpu->A->GetUnsigned()); // printf the value of the A register


// -------- Should print "Hello, World!" using the 8085 example above if all goes well	
int i = 0x0900;

while (memory_data[i] != '\0')
{
	printf("%c", memory_data[i]);
	i++;
}

printf("\n");
	
// --------
	
// memory_data now contains the memory dump after the program is done.
// For example, you can look at address 0x1234: memory_data[0x1234]
```
</details>


***Note***: You can print and modify memory (and registers) *during* runtime, but behaviour is not defined and I can't currently guarantee thread-safety
