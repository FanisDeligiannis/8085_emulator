#pragma once

#include <string>

//This is my bootloader of 8085.

std::string Bootloader = R"(

RST0 EQU 0000H
RST1 EQU 0008H
RST2 EQU 0010H
RST3 EQU 0018H
RST4 EQU 0020H
RST5 EQU 0028H
RST6 EQU 0030H
RST7 EQU 0038H
RST45 EQU 0024H
RST55 EQU 002CH
RST65 EQU 0034H
RST75 EQU 003CH
CODE EQU 0800H

ORG RST0
	JMP CODE
ORG RST1
	RET
ORG RST2
	RET
ORG RST3
	RET
ORG RST4
	RET
ORG RST5
	RET
ORG RST6
	RET
ORG RST7
	RET
ORG RST45
	RET
ORG RST55
	RET
ORG RST65
	RET
ORG RST75
	RET

ORG 0100H

DCD:
	PUSH PSW ; Don't want to change A
	MVI A,02H 
	OUT 56H ; The 7 segment display expects a 1 in the 2nd LSB in address 56H  
	POP PSW
	RET

STDM:
	PUSH PSW ; Don't want to affect flags
	XCHG ; Get DE to HL so we can use register M
	
	MOV A,M ; Get data at address from HL
	CALL _STDM1 ; Converts it to the proper format.
	OUT 55H ; MSB of 7 segment display is at address 55H
 	INX H ; Next address
	MOV A,M ; Get Data
	CALL _STDM1 ; Convert to the proper
	OUT 54H ; 2nd MSB at address 54H
	INX H ; . . .
	MOV A,M
	CALL _STDM1
	OUT 53H
	INX H
	MOV A,M
	CALL _STDM1
	OUT 52H
	INX H
	MOV A,M
	CALL _STDM1
	OUT 51H
	INX H
	MOV A,M
	CALL _STDM1
	OUT 50H
	XCHG ; Get the value to DE, and get HL back.
	POP PSW
	RET
	
_STDM1: ;CONVERT TO PROPER FORMAT
	ANI 0FH ; We only want the lower 4 bits.
	CPI 0AH 
	JC _STDM3 ; If it's LOWER than 0AH (10)
	
; If not, continue
	
_STDM2: ; BIGGER/EQUAL TO 0AH
	ADI 37H ; This converts it from a number to an ASCII character
			; 47H is 'A'. So: 37H+0AH=47H='A'. etc.
	RET
	
_STDM3: ; LOWER THAN 0AH
	ADI 30H ; This converts it from a number to an ASCII character.
			; 30H is '0'. So 0+30H = '0', 1+30H = '1' etc.
	RET

STDC: ; Store Display characters
	PUSH PSW
	XCHG ; Get DE to HL

	MOV A,M
	OUT 55H ; Simply output it. It's already in the correct format.
 	INX H
	MOV A,M
	OUT 54H
	INX H
	MOV A,M
	OUT 53H
	INX H
	MOV A,M
	OUT 52H
	INX H
	MOV A,M
	OUT 51H
	INX H
	MOV A,M
	OUT 50H

	XCHG
	POP PSW
	RET

CLEARDISPLAY:
	PUSH PSW

	MVI A,00H ; Just set all characters in the display to 00H.
	OUT 50H
	OUT 51H
	OUT 52H
	OUT 53H
	OUT 54H
	OUT 55H

	POP PSW

	RET

BEEP:
	PUSH PSW

	MVI A,B8H
	OUT 62H ; frequency LOW
	MVI A,01H
	OUT 63H ; frequency HIGH

	MVI A,e8H
	OUT 60H ; duration LOW
	MVI A,03H
	OUT 61H ; duration HIGH

	JMP _WAIT_BEEP

BEEPFD:
	PUSH PSW

	MOV A,C
	OUT 62H
	MOV A,B
	OUT 63H

	MOV A,E
	OUT 60H
	MOV A,D
	OUT 61H

_WAIT_BEEP:
	CALL DELA
	IN 60H
	CPI 00H
	JNZ _WAIT_BEEP
	
	CALL DELA
	IN 61H
	CPI 00H
	JNZ _WAIT_BEEP

	POP PSW

	RET


; NOT VERY ACCURATE DUE TO THE WAY THE SIMULATOR WORKS
; Could fix this by changing how the clock cycles are handled in C++

DELA:
	PUSH PSW ; 12 t-states
	PUSH B ; 24 t-states
	PUSH H ; 36 t-states
	
	LXI B,30 ; 46 t-states 
	; 46 Total t-states so far.
	
_DELA1: ; 64 total t-states per loop.
 	CALL DCD ; 16 t-states
	DCX B ; 22 t-states
	
	MOV A,C ; 26 t-states
	CPI 00H ; 33 t-states
	JNZ _DELA1 ; 43 t-states
	
	MOV A,B ; 47 t-states
	CPI 00H ; 54 t-states
	JNZ _DELA1 ; 64 t-states
	
_DONEDELA: ; 40 t-states total
	POP H ; 10 t-states
	POP B ; 20 t-states
	POP PSW ; 30 t-states
	
	RET ; 40 t-states
	
	
DELB:
	PUSH PSW ; Don't affect registers
	PUSH B
	
_DELB1:
	CALL DELA ; Call DELA as many times as BC says. So BC * 1ms.
	
	DCX B
	
	MOV A,C 
	CPI 00H 
	JNZ _DELB1
	
	MOV A,B 
	CPI 00H
	JNZ _DELB1
	
_DONEDELB:
	POP B
	POP PSW
	
	RET

KIND:
	PUSH B ; Don't affect registers
	PUSH PSW
	
_KIND0: ; Scan line 0.
	MVI A,11111110B ; Line 0 
	OUT 28H ; Output it.
	CALL DCD ; Call DCD as a delay between OUT and IN.
	IN 18H
	
	CPI FFH ; if it's FFH, it means no input was given from line 0.
	JZ _KIND1 ; So we check line 1.
	
	MVI B,00H ; if input was given, we check which button was pressed.
	; B will be the value of our pressed button. 
	; It starts at 0 because the first button is 0.
	
	CPI 11111110B ; If first button was pressed, we're done
	JZ _KINDDONE

	INR B ; If not, we move to the 2nd button and increase B.

	CPI 11111101B ; Now B is 1. If 1 is pressed, we're done.
	JZ _KINDDONE
	
	INR B ; If not, we increase B again.
	
	CPI 11111011B ; Now B is 2. If 2 is pressed, we're done
	JZ _KINDDONE
	
	INR B ; Increase B again
	
	CPI 11110111B ; B is 3. Is 3 pressed?
	JZ _KINDDONE


_KIND1: ; Line 1.

	MVI A,11111101B ; Scan line 1.
	OUT 28H
	CALL DCD ; DCD as a delay between OUT and IN 
	IN 18H
	
	CPI FFH ; if it's FFH, move to line 2.
	JZ _KIND2

	MVI B,4H ; Same exact method, but start B at 4H.
	
	CPI 11111110B
	JZ _KINDDONE

	INR B

	CPI 11111101B
	JZ _KINDDONE
	
	INR B
	
	CPI 11111011B
	JZ _KINDDONE
	
	INR B
	
	CPI 11110111B
	JZ _KINDDONE



_KIND2: ; Line 2. Same method as above.
	MVI A,11111011B
	OUT 28H
	CALL DCD
	IN 18H
	
	CPI FFH
	JZ _KIND3

	MVI B,8H
	
	CPI 11111110B
	JZ _KINDDONE

	INR B

	CPI 11111101B
	JZ _KINDDONE
	
	INR B
	
	CPI 11111011B
	JZ _KINDDONE
	
	INR B
	
	CPI 11110111B
	JZ _KINDDONE


_KIND3: ; Line 3. Same method as above.
	MVI A,11110111B
	OUT 28H
	CALL DCD
	IN 18H
	
	CPI FFH
	JZ _KIND0
	
	MVI B,CH
	
	CPI 11111110B
	JZ _KINDDONE

	INR B

	CPI 11111101B
	JZ _KINDDONE
	
	INR B
	
	CPI 11111011B
	JZ _KINDDONE
	
	INR B
	
_KINDDONE:
	POP PSW ; We pushed before.
	
	MOV A,B ; After POP PSW, we get B (B = output) to A.
			; That's because A should be our output
			; That way, we affect A, but NOT flags.
	
	POP B ; B is not affected.
	
	RET

ORG CODE
)";