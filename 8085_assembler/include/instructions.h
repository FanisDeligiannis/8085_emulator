#pragma once

#include <stdio.h>
#include <cstdint>
#include <string>

#include "source_file.h"
#include "assembler.h"

namespace InternalAssembler
{

    //The entire instruction set, used in assembling the binary from text.

    struct Instruction
    {
        uint8_t OPCODE;
        char OPERAND[16];
        uint8_t bytes;
        bool (*ACTION)(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
        char ARGS[16];
    } typedef Instruction;

    uint16_t FindLabel(const std::string& label, std::shared_ptr<SourceFile> source);

    uint8_t GetNextRegister(std::shared_ptr<SourceFile> source, bool a = true, bool m = true);

    uint8_t GetNextDoubleRegister(std::shared_ptr<SourceFile> source, bool h = true, bool sp = false, bool psw = false);

    uint8_t GetImmediate8(std::shared_ptr<SourceFile> source);

    uint16_t GetImmediate16(std::shared_ptr<SourceFile> source);


    bool ACI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool ADC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool ADD(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool ADI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool ANA(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool ANI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CALL(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CM(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CMA(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CMC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CMP(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CNC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CNZ(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CP(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CPE(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CPI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CPO(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool CZ(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool DAA(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool DAD(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool DCR(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool DCX(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool DI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool EI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool HLT(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool IN(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool INR(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool INX(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JM(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JMP(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JNC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JNZ(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JP(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JPE(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JPO(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool JZ(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool LDA(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool LDAX(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool LHLD(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool LXI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool MOV(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool MVI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool NOP(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool ORA(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool ORI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool OUT(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool PCHL(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool POP(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool PUSH(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RAL(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RAR(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RET(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RIM(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RLC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool DSUB(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RM(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RNC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RNZ(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RP(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RPE(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RPO(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RRC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RST(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool RZ(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool SBB(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool SBI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool SHLD(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool SIM(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool SPHL(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool STA(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool STAX(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool STC(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool SUB(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool SUI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool XCHG(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool XRA(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool XRI(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);
    bool XTHL(int bytes, std::shared_ptr<SourceFile> source, uint8_t* _Memory);

    extern Instruction Instructions[256];

}