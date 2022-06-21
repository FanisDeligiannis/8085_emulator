#pragma once

#include <stdio.h>
#include <cstdint>
#include <string>

#include "source_file.h"
#include "assembler.h"

//The entire instruction set, used in assembling the binary from text.

struct Instruction
{
    uint8_t OPCODE;
    char OPERAND[16];
    uint8_t bytes;
    bool (*ACTION)(int bytes, SourceFile* source, uint8_t* _Memory);
    char ARGS[16];
} typedef Instruction;

uint16_t FindLabel(const std::string& label, SourceFile* source);

uint8_t GetNextRegister(SourceFile* source, bool a = true, bool m = true);

uint8_t GetNextDoubleRegister(SourceFile* source, bool h = true, bool sp = false, bool psw = false);

uint8_t GetImmediate8(SourceFile* source);

uint16_t GetImmediate16(SourceFile* source);


bool ACI(int bytes, SourceFile* source, uint8_t* _Memory);
bool ADC(int bytes, SourceFile* source, uint8_t* _Memory);
bool ADD(int bytes, SourceFile* source, uint8_t* _Memory);
bool ADI(int bytes, SourceFile* source, uint8_t* _Memory);
bool ANA(int bytes, SourceFile* source, uint8_t* _Memory);
bool ANI(int bytes, SourceFile* source, uint8_t* _Memory);
bool CALL(int bytes, SourceFile* source, uint8_t* _Memory);
bool CC(int bytes, SourceFile* source, uint8_t* _Memory);
bool CM(int bytes, SourceFile* source, uint8_t* _Memory);
bool CMA(int bytes, SourceFile* source, uint8_t* _Memory);
bool CMC(int bytes, SourceFile* source, uint8_t* _Memory);
bool CMP(int bytes, SourceFile* source, uint8_t* _Memory);
bool CNC(int bytes, SourceFile* source, uint8_t* _Memory);
bool CNZ(int bytes, SourceFile* source, uint8_t* _Memory);
bool CP(int bytes, SourceFile* source, uint8_t* _Memory);
bool CPE(int bytes, SourceFile* source, uint8_t* _Memory);
bool CPI(int bytes, SourceFile* source, uint8_t* _Memory);
bool CPO(int bytes, SourceFile* source, uint8_t* _Memory);
bool CZ(int bytes, SourceFile* source, uint8_t* _Memory);
bool DAA(int bytes, SourceFile* source, uint8_t* _Memory);
bool DAD(int bytes, SourceFile* source, uint8_t* _Memory);
bool DCR(int bytes, SourceFile* source, uint8_t* _Memory);
bool DCX(int bytes, SourceFile* source, uint8_t* _Memory);
bool DI(int bytes, SourceFile* source, uint8_t* _Memory);
bool EI(int bytes, SourceFile* source, uint8_t* _Memory);
bool HLT(int bytes, SourceFile* source, uint8_t* _Memory);
bool IN(int bytes, SourceFile* source, uint8_t* _Memory);
bool INR(int bytes, SourceFile* source, uint8_t* _Memory);
bool INX(int bytes, SourceFile* source, uint8_t* _Memory);
bool JC(int bytes, SourceFile* source, uint8_t* _Memory);
bool JM(int bytes, SourceFile* source, uint8_t* _Memory);
bool JMP(int bytes, SourceFile* source, uint8_t* _Memory);
bool JNC(int bytes, SourceFile* source, uint8_t* _Memory);
bool JNZ(int bytes, SourceFile* source, uint8_t* _Memory);
bool JP(int bytes, SourceFile* source, uint8_t* _Memory);
bool JPE(int bytes, SourceFile* source, uint8_t* _Memory);
bool JPO(int bytes, SourceFile* source, uint8_t* _Memory);
bool JZ(int bytes, SourceFile* source, uint8_t* _Memory);
bool LDA(int bytes, SourceFile* source, uint8_t* _Memory);
bool LDAX(int bytes, SourceFile* source, uint8_t* _Memory);
bool LHLD(int bytes, SourceFile* source, uint8_t* _Memory);
bool LXI(int bytes, SourceFile* source, uint8_t* _Memory);
bool MOV(int bytes, SourceFile* source, uint8_t* _Memory);
bool MVI(int bytes, SourceFile* source, uint8_t* _Memory);
bool NOP(int bytes, SourceFile* source, uint8_t* _Memory);
bool ORA(int bytes, SourceFile* source, uint8_t* _Memory);
bool ORI(int bytes, SourceFile* source, uint8_t* _Memory);
bool OUT(int bytes, SourceFile* source, uint8_t* _Memory);
bool PCHL(int bytes, SourceFile* source, uint8_t* _Memory);
bool POP(int bytes, SourceFile* source, uint8_t* _Memory);
bool PUSH(int bytes, SourceFile* source, uint8_t* _Memory);
bool RAL(int bytes, SourceFile* source, uint8_t* _Memory);
bool RAR(int bytes, SourceFile* source, uint8_t* _Memory);
bool RC(int bytes, SourceFile* source, uint8_t* _Memory);
bool RET(int bytes, SourceFile* source, uint8_t* _Memory);
bool RIM(int bytes, SourceFile* source, uint8_t* _Memory);
bool RLC(int bytes, SourceFile* source, uint8_t* _Memory);
bool DSUB(int bytes, SourceFile* source, uint8_t* _Memory);
bool RM(int bytes, SourceFile* source, uint8_t* _Memory);
bool RNC(int bytes, SourceFile* source, uint8_t* _Memory);
bool RNZ(int bytes, SourceFile* source, uint8_t* _Memory);
bool RP(int bytes, SourceFile* source, uint8_t* _Memory);
bool RPE(int bytes, SourceFile* source, uint8_t* _Memory);
bool RPO(int bytes, SourceFile* source, uint8_t* _Memory);
bool RRC(int bytes, SourceFile* source, uint8_t* _Memory);
bool RST(int bytes, SourceFile* source, uint8_t* _Memory);
bool RZ(int bytes, SourceFile* source, uint8_t* _Memory);
bool SBB(int bytes, SourceFile* source, uint8_t* _Memory);
bool SBI(int bytes, SourceFile* source, uint8_t* _Memory);
bool SHLD(int bytes, SourceFile* source, uint8_t* _Memory);
bool SIM(int bytes, SourceFile* source, uint8_t* _Memory);
bool SPHL(int bytes, SourceFile* source, uint8_t* _Memory);
bool STA(int bytes, SourceFile* source, uint8_t* _Memory);
bool STAX(int bytes, SourceFile* source, uint8_t* _Memory);
bool STC(int bytes, SourceFile* source, uint8_t* _Memory);
bool SUB(int bytes, SourceFile* source, uint8_t* _Memory);
bool SUI(int bytes, SourceFile* source, uint8_t* _Memory);
bool XCHG(int bytes, SourceFile* source, uint8_t* _Memory);
bool XRA(int bytes, SourceFile* source, uint8_t* _Memory);
bool XRI(int bytes, SourceFile* source, uint8_t* _Memory);
bool XTHL(int bytes, SourceFile* source, uint8_t* _Memory);

extern Instruction Instructions[256];