
# 8085 Assembler

## Usage

Usage: 8085_assembler.exe source_file [outfile]

OR 

Drag the sourcefile to the assembler exe.

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

-	Label **CODE** is predefined to 0800H

- Comments with *;*



## TODO

- Add MACROs
- More verbose and better errors

## Issues
Instruction set is complete. Mistakes are expected though.

---

# 8085 Emulator

Not much yet.
