#pragma once

#include <stdio.h>
#include <cstdint>
#include <string>

#include "source_file.h"
#include "assembler.h"

struct Instruction
{
    uint8_t OPCODE;
    char OPERAND[16];
    uint8_t bytes;
    uint8_t*(*ACTION)(int bytes, SourceFile* source);
    char ARGS[16];
} typedef Instruction;

//TODO: LOTS OF ERROR CHECKING

uint8_t* ACI(int bytes, SourceFile* source)
{
    uint8_t* ret = (uint8_t*)calloc(bytes, sizeof(uint8_t));
    if (ret == nullptr)
    {
        printf("Error allocating memory");
        exit(1);
    }

    ret[0] = (uint8_t) 0xCE;

    std::string nextWord = source->Next();
    ret[1] = StringToUInt8(nextWord);

    return ret;
}

uint8_t* ADC(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* ADD(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* ADI(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* ANA(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* ANI(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* CALL(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* CC(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* CM(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* CMA(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* CMC(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* CMP(int bytes, SourceFile* source)
{
    return nullptr;
}

uint8_t* CNC(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* CNZ(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* CP(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* CPE(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* CPI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* CPO(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* CZ(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* DAA(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* DAD(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* DCR(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* DCX(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* DI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* EI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* HLT(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* IN(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* INR(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* INX(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JC(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JM(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JMP(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JNC(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JNZ(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JP(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JPE(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JPO(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* JZ(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* LDA(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* LDAX(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* LHLD(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* LXI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* MOV(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* MVI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* NOP(int bytes, SourceFile* source)
{
    printf("%s", "nop run!");

    return nullptr;
}

uint8_t* ORA(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* ORI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* OUT(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* PCHL(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* POP(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* PUSH(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RAL(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RAR(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RC(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RET(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RIM(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RLC(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RM(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RNC(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RNZ(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RP(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RPE(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RPO(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RRC(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RST(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* RZ(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* SBB(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* SBI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* SHLD(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* SIM(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* SPHL(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* STA(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* STAX(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* STC(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* SUB(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* SUI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* XCHG(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* XRA(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* XRI(int bytes, SourceFile* source)
{

    return nullptr;
}

uint8_t* XTHL(int bytes, SourceFile* source)
{

    return nullptr;
}

//SP = Stack pointer
//PC = Program Counter 
//P = Port Address
//Ad = Address
//La = Label

Instruction Instructions[] = {
    {0x0, "NOP", 1, NOP, ""},
    {0x1, "LXI", 3, LXI, "B"},
    {0x2, "STAX", 1, STAX, "B"},
    {0x3, "INX", 1, INX, "B"},
    {0x4, "INR", 1, INR, "B"},
    {0x5, "DCR", 1, DCR, "B"},
    {0x6, "MVI", 2, MVI, "B N"},
    {0x7, "RLC", 1, RLC, ""},
    {},
    {0x9, "DAD", 1, DAD, "B"},
    {0xa, "LDAX", 1, LDAX, "B"},
    {0xb, "DCX", 1, DCX, "B"},
    {0xc, "INR", 1, INR, "C"},
    {0xd, "DCR", 1, DCR, "C"},
    {0xe, "MVI", 2, MVI, "C N"},
    {0xf, "RRC", 1, RRC, ""},
    {},
    {0x11, "LXI", 3, LXI, "D"},
    {0x12, "STAX", 1, STAX, "D"},
    {0x13, "INX", 1, INX, "D"},
    {0x14, "INR", 1, INR, "D"},
    {0x15, "DCR", 1, DCR, "D"},
    {0x16, "MVI", 2, MVI, "D N"},
    {0x17, "RAL", 1, RAL, ""},
    {},
    {0x19, "DAD", 1, DAD, "D"},
    {0x1a, "LDAX", 1, LDAX, "D"},
    {0x1b, "DCX", 1, DCX, "D"},
    {0x1c, "INR", 1, INR, "E"},
    {0x1d, "DCR", 1, DCR, "E"},
    {0x1e, "MVI", 2, MVI, "E N"},
    {0x1f, "RAR", 1, RAR, ""},
    {0x20, "RIM", 1, RIM, ""},
    {0x21, "LXI", 3, LXI, "H"},
    {0x22, "SHLD", 3, SHLD, "Ad"},
    {0x23, "INX", 1, INX, "H"},
    {0x24, "INR", 1, INR, "H"},
    {0x25, "DCR", 1, DCR, "H"},
    {0x26, "MVI", 2, MVI, "H N"},
    {0x27, "DAA", 1, DAA, ""},
    {},
    {0x29, "DAD", 1, DAD, "H"},
    {0x2a, "LHLD", 3, LHLD, "Ad"},
    {0x2b, "DCX", 1, DCX, "H"},
    {0x2c, "INR", 1, INR, "L"},
    {0x2d, "DCR", 1, DCR, "L"},
    {0x2e, "MVI", 2, MVI, "L N"},
    {0x2f, "CMA", 1, CMA, ""},
    {0x30, "SIM", 1, SIM, ""},
    {0x31, "LXI", 3, LXI, "SP"},
    {0x32, "STA", 3, STA, "Ad"},
    {0x33, "INX", 1, INX, "SP"},
    {0x34, "INR", 1, INR, "M"},
    {0x35, "DCR", 1, DCR, "M"},
    {0x36, "MVI", 2, MVI, "M N"},
    {0x37, "STC", 1, STC, ""},
    {},
    {0x39, "DAD", 1, DAD, "SP"},
    {0x3a, "LDA", 3, LDA, "Ad"},
    {0x3b, "DCX", 1, DCX, "SP"},
    {0x3c, "INR", 1, INR, "A"},
    {0x3d, "DCR", 1, DCR, "A"},
    {0x3e, "MVI", 2, MVI, "A N"},
    {0x3f, "CMC", 1, CMC, ""},
    {0x40, "MOV", 1, MOV, "B B"},
    {0x41, "MOV", 1, MOV, "B C"},
    {0x42, "MOV", 1, MOV, "B D"},
    {0x43, "MOV", 1, MOV, "B E"},
    {0x44, "MOV", 1, MOV, "B H"},
    {0x45, "MOV", 1, MOV, "B L"},
    {0x46, "MOV", 1, MOV, "B M"},
    {0x47, "MOV", 1, MOV, "B A"},
    {0x48, "MOV", 1, MOV, "C B"},
    {0x49, "MOV", 1, MOV, "C C"},
    {0x4a, "MOV", 1, MOV, "C D"},
    {0x4b, "MOV", 1, MOV, "C E"},
    {0x4c, "MOV", 1, MOV, "C H"},
    {0x4d, "MOV", 1, MOV, "C L"},
    {0x4e, "MOV", 1, MOV, "C M"},
    {0x4f, "MOV", 1, MOV, "C A"},
    {0x50, "MOV", 1, MOV, "D B"},
    {0x51, "MOV", 1, MOV, "D C"},
    {0x52, "MOV", 1, MOV, "D D"},
    {0x53, "MOV", 1, MOV, "D E"},
    {0x54, "MOV", 1, MOV, "D H"},
    {0x55, "MOV", 1, MOV, "D L"},
    {0x56, "MOV", 1, MOV, "D M"},
    {0x57, "MOV", 1, MOV, "D A"},
    {0x58, "MOV", 1, MOV, "E B"},
    {0x59, "MOV", 1, MOV, "E C"},
    {0x5a, "MOV", 1, MOV, "E D"},
    {0x5b, "MOV", 1, MOV, "E E"},
    {0x5c, "MOV", 1, MOV, "E H"},
    {0x5d, "MOV", 1, MOV, "E L"},
    {0x5e, "MOV", 1, MOV, "E M"},
    {0x5f, "MOV", 1, MOV, "E A"},
    {0x60, "MOV", 1, MOV, "H B"},
    {0x61, "MOV", 1, MOV, "H C"},
    {0x62, "MOV", 1, MOV, "H D"},
    {0x63, "MOV", 1, MOV, "H E"},
    {0x64, "MOV", 1, MOV, "H H"},
    {0x65, "MOV", 1, MOV, "H L"},
    {0x66, "MOV", 1, MOV, "H M"},
    {0x67, "MOV", 1, MOV, "H A"},
    {0x68, "MOV", 1, MOV, "L B"},
    {0x69, "MOV", 1, MOV, "L C"},
    {0x6a, "MOV", 1, MOV, "L D"},
    {0x6b, "MOV", 1, MOV, "L E"},
    {0x6c, "MOV", 1, MOV, "L H"},
    {0x6d, "MOV", 1, MOV, "L L"},
    {0x6e, "MOV", 1, MOV, "L M"},
    {0x6f, "MOV", 1, MOV, "L A"},
    {0x70, "MOV", 1, MOV, "M B"},
    {0x71, "MOV", 1, MOV, "M C"},
    {0x72, "MOV", 1, MOV, "M D"},
    {0x73, "MOV", 1, MOV, "M E"},
    {0x74, "MOV", 1, MOV, "M H"},
    {0x75, "MOV", 1, MOV, "M L"},
    {0x76, "HLT", 1, HLT, ""},
    {0x77, "MOV", 1, MOV, "M A"},
    {0x78, "MOV", 1, MOV, "A B"},
    {0x79, "MOV", 1, MOV, "A C"},
    {0x7a, "MOV", 1, MOV, "A D"},
    {0x7b, "MOV", 1, MOV, "A E"},
    {0x7c, "MOV", 1, MOV, "A H"},
    {0x7d, "MOV", 1, MOV, "A L"},
    {0x7e, "MOV", 1, MOV, "A M"},
    {0x7f, "MOV", 1, MOV, "A A"},
    {0x80, "ADD", 1, ADD, "B"},
    {0x81, "ADD", 1, ADD, "C"},
    {0x82, "ADD", 1, ADD, "D"},
    {0x83, "ADD", 1, ADD, "E"},
    {0x84, "ADD", 1, ADD, "H"},
    {0x85, "ADD", 1, ADD, "L"},
    {0x86, "ADD", 1, ADD, "M"},
    {0x87, "ADD", 1, ADD, "A"},
    {0x88, "ADC", 1, ADC, "B"},
    {0x89, "ADC", 1, ADC, "C"},
    {0x8a, "ADC", 1, ADC, "D"},
    {0x8b, "ADC", 1, ADC, "E"},
    {0x8c, "ADC", 1, ADC, "H"},
    {0x8d, "ADC", 1, ADC, "L"},
    {0x8e, "ADC", 1, ADC, "M"},
    {0x8f, "ADC", 1, ADC, "A"},
    {0x90, "SUB", 1, SUB, "B"},
    {0x91, "SUB", 1, SUB, "C"},
    {0x92, "SUB", 1, SUB, "D"},
    {0x93, "SUB", 1, SUB, "E"},
    {0x94, "SUB", 1, SUB, "H"},
    {0x95, "SUB", 1, SUB, "L"},
    {0x96, "SUB", 1, SUB, "M"},
    {0x97, "SUB", 1, SUB, "A"},
    {0x98, "SBB", 1, SBB, "B"},
    {0x99, "SBB", 1, SBB, "C"},
    {0x9a, "SBB", 1, SBB, "D"},
    {0x9b, "SBB", 1, SBB, "E"},
    {0x9c, "SBB", 1, SBB, "H"},
    {0x9d, "SBB", 1, SBB, "L"},
    {0x9e, "SBB", 1, SBB, "M"},
    {0x9f, "SBB", 1, SBB, "A"},
    {0xa0, "ANA", 1, ANA, "B"},
    {0xa1, "ANA", 1, ANA, "C"},
    {0xa2, "ANA", 1, ANA, "D"},
    {0xa3, "ANA", 1, ANA, "E"},
    {0xa4, "ANA", 1, ANA, "H"},
    {0xa5, "ANA", 1, ANA, "L"},
    {0xa6, "ANA", 1, ANA, "M"},
    {0xa7, "ANA", 1, ANA, "A"},
    {0xa8, "XRA", 1, XRA, "B"},
    {0xa9, "XRA", 1, XRA, "C"},
    {0xaa, "XRA", 1, XRA, "D"},
    {0xab, "XRA", 1, XRA, "E"},
    {0xac, "XRA", 1, XRA, "H"},
    {0xad, "XRA", 1, XRA, "L"},
    {0xae, "XRA", 1, XRA, "M"},
    {0xaf, "XRA", 1, XRA, "A"},
    {0xb0, "ORA", 1, ORA, "B"},
    {0xb1, "ORA", 1, ORA, "C"},
    {0xb2, "ORA", 1, ORA, "D"},
    {0xb3, "ORA", 1, ORA, "E"},
    {0xb4, "ORA", 1, ORA, "H"},
    {0xb5, "ORA", 1, ORA, "L"},
    {0xb6, "ORA", 1, ORA, "M"},
    {0xb7, "ORA", 1, ORA, "A"},
    {0xb8, "CMP", 1, CMP, "B"},
    {0xb9, "CMP", 1, CMP, "C"},
    {0xba, "CMP", 1, CMP, "D"},
    {0xbb, "CMP", 1, CMP, "E"},
    {0xbc, "CMP", 1, CMP, "H"},
    {0xbd, "CMP", 1, CMP, "M"},
    {},
    {0xbf, "CMP", 1, CMP, "A"},
    {0xc0, "RNZ", 1, RNZ, ""},
    {0xc1, "POP", 1, POP, "B"},
    {0xc2, "JNZ", 3, JNZ, "La"},
    {0xc3, "JMP", 3, JMP, "La"},
    {0xc4, "CNZ", 3, CNZ, "La"},
    {0xc5, "PUSH", 1, PUSH, "B"},
    {0xc6, "ADI", 2, ADI, "N"},
    {0xc7, "RST", 1, RST, "0"},
    {0xc8, "RZ", 1, RZ, ""},
    {0xc9, "RET", 1, RET, ""},
    {0xca, "JZ", 3, JZ, "La"},
    {},
    {0xcc, "CZ", 3, CZ, "La"},
    {0xcd, "CALL", 3, CALL, "La"},
    {0xce, "ACI", 2, ACI, "N"},
    {0xcf, "RST", 1, RST, "1"},
    {0xd0, "RNC", 1, RNC, ""},
    {0xd1, "POP", 1, POP, "D"},
    {0xd2, "JNC", 3, JNC, "La"},
    {0xd3, "OUT", 2, OUT, "P"},
    {0xd4, "CNC", 3, CNC, "La"},
    {0xd5, "PUSH", 1, PUSH, "D"},
    {0xd6, "SUI", 2, SUI, "N"},
    {0xd7, "RST", 1, RST, "2"},
    {0xd8, "RC", 1, RC, ""},
    {},
    {0xda, "JC", 3, JC, "La"},
    {0xdb, "IN", 2, IN, "P"},
    {0xdc, "CC", 3, CC, "La"},
    {},
    {0xde, "SBI", 2, SBI, "N"},
    {0xdf, "RST", 1, RST, "3"},
    {0xe0, "RPO", 1, RPO, ""},
    {0xe1, "POP", 1, POP, "H"},
    {0xe2, "JPO", 3, JPO, "La"},
    {0xe3, "XTHL", 1, XTHL, ""},
    {0xe4, "CPO", 3, CPO, "La"},
    {0xe5, "PUSH", 1, PUSH, "H"},
    {0xe6, "ANI", 2, ANI, "N"},
    {0xe7, "RST", 1, RST, "4"},
    {0xe8, "RPE", 1, RPE, ""},
    {0xe9, "PCHL", 1, PCHL, ""},
    {0xea, "JPE", 3, JPE, "La"},
    {0xeb, "XCHG", 1, XCHG, ""},
    {0xec, "CPE", 3, CPE, "La"},
    {},
    {0xee, "XRI", 2, XRI, "N"},
    {0xef, "RST", 1, RST, "5"},
    {0xf0, "RP", 1, RP, ""},
    {0xf1, "POP", 1, POP, "PSW"},
    {0xf2, "JP", 3, JP, "La"},
    {0xf3, "DI", 1, DI, ""},
    {0xf4, "CP", 3, CP, "La"},
    {0xf5, "PUSH", 1, PUSH, "PSW"},
    {0xf6, "ORI", 2, ORI, "N"},
    {0xf7, "RST", 1, RST, "6"},
    {0xf8, "RM", 1, RM, ""},
    {0xf9, "SPHL", 1, SPHL, ""},
    {0xfa, "JM", 3, JM, "La"},
    {0xfb, "EI", 1, EI, ""},
    {0xfc, "CM", 3, CM, "La"},
    {},
    {0xfe, "CPI", 2, CPI, "N"},
    {0xff, "RST", 1, RST, "7"}
};