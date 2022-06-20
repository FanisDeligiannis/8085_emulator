#include "CPUinstructions.h"

#include <stdio.h>
#include <vector>
#include <bitset>

#include "cpu.h"

//Set sign, zero and parity flag, depending on a certain number.
//Not enough info to know aux_c and carry.

int bits_in(std::uint64_t u)
{
    auto bs = std::bitset<64>(u);
    return bs.count();
}

void SetFlagsBasedOn(int8_t n)
{
    CPU::cpu->SetFlags(
        n < 0,
        n == 0,
        -1,
        !(bits_in(*(uint8_t*)&n) % 2),
        -1
    );
}

//Get double register BC unsigned uint16_t
uint16_t GetBCUnsigned()
{
    CPU* cpu = CPU::cpu;

    uint8_t B = cpu->B->GetUnsigned();
    uint8_t C = cpu->C->GetUnsigned();

    uint16_t BC = (B << 8) | C;

    return BC;
}

//Get double regsiter DE unsigned uint16_t
uint16_t GetDEUnsigned()
{
    CPU* cpu = CPU::cpu;

    uint8_t D = cpu->D->GetUnsigned();
    uint8_t E = cpu->E->GetUnsigned();

    uint16_t DE = (D << 8) | E;

    return DE;
}

//Get double regsiter HL unsigned uint16_t
uint16_t GetHLUnsigned()
{
    CPU* cpu = CPU::cpu;

    uint8_t H = cpu->H->GetUnsigned();
    uint8_t L = cpu->L->GetUnsigned();

    uint16_t HL = (H << 8) | L;

    return HL;
}

//Get double regsiter BC signed int16_t
int16_t GetBCSigned()
{
    CPU* cpu = CPU::cpu;

    uint8_t B = cpu->B->GetUnsigned();
    uint8_t C = cpu->C->GetUnsigned();

    uint16_t BC = (B << 8) | C;

    return *(int16_t*)&BC;
}

//Get double regsiter DE signed int16_t
int16_t GetDESigned()
{
    CPU* cpu = CPU::cpu;

    uint8_t D = cpu->D->GetUnsigned();
    uint8_t E = cpu->E->GetUnsigned();

    uint16_t DE = (D << 8) | E;

    return *(int16_t*)&DE;
}

//Get double regsiter HL signed int16_t
int16_t GetHLSigned()
{
    CPU* cpu = CPU::cpu;

    uint8_t H = cpu->H->GetUnsigned();
    uint8_t L = cpu->L->GetUnsigned();

    uint16_t HL = (H << 8) | L;

    return *(int16_t*)&HL;
}


//Get the next 2 bytes in memory, using PC
uint16_t GetNextPC16()
{
    CPU* cpu = CPU::cpu;

    uint8_t LOW = cpu->NextPC();
    uint8_t HIGH = cpu->NextPC();

    uint16_t hl = (HIGH << 8) | LOW;

    return hl;
}

//Compare Register A with another number. Set the flags accordingly.
void Compare(int8_t other)
{
    CPU* cpu = CPU::cpu;
    int8_t A = cpu->A->GetSigned();

    if (A < other)
    {
        cpu->Flags->SetBit(CARRY_FLAG, 1);
        cpu->Flags->ClearBit(ZERO_FLAG);
    }
    else if (A == other)
    {
        cpu->Flags->ClearBit(CARRY_FLAG);
        cpu->Flags->SetBit(ZERO_FLAG, 1);
    }
    else if (A > other)
    {
        cpu->Flags->ClearBit(CARRY_FLAG);
        cpu->Flags->ClearBit(ZERO_FLAG);
    }
}

//Add signed number to Register A
void AddSigned(int8_t data)
{
    CPU* cpu = CPU::cpu;

    int8_t rA = cpu->A->GetSigned();

    int16_t result16 = rA + data;
    int8_t result = rA + data;
    int8_t result4 = rA + (data & 0x0f);

    cpu->SetFlags(
        result < 0,
        result != 0,
        (result4 & 0xf0) > 0,
        !(bits_in(*(uint8_t*)&result) % 2),
        (result16 & 0b100000000) > 0
    );

    cpu->A->SetSigned(result);
}

//Add signed number to Register A with carry
void AddSignedWithCarry(int8_t data)
{
    CPU* cpu = CPU::cpu;

    int8_t rA = cpu->A->GetSigned();
    int8_t rC = cpu->Flags->GetBit(CARRY_FLAG);

    int16_t result16 = rA + data + rC;
    int8_t result = rA + data + rC;
    int8_t result4 = rA + (data & 0x0f) + rC;

    cpu->SetFlags(
        result < 0,
        result != 0,
        (result4 & 0xf0) > 0,
        !(bits_in(*(uint8_t*)&result) % 2),
        (result16 & 0b100000000) > 0
    );

    cpu->A->SetSigned(result);
}

//Add unsigned number to Register A with carry
void AddUnignedWithCarry(uint8_t data)
{
    CPU* cpu = CPU::cpu;

    uint8_t rA = cpu->A->GetUnsigned();
    uint8_t rC = cpu->Flags->GetBit(CARRY_FLAG);

    uint16_t result16 = rA + data + rC;
    uint8_t result = rA + data + rC;
    uint8_t result4 = rA + (data & 0x0f) + rC;

    //TODO CHECK FLAGS HERE
    cpu->SetFlags(
        0,
        result != 0,
        (result4 & 0xf0) > 0,
        !(bits_in(result) % 2),
        (result16 & 0b100000000) > 0
    );

    cpu->A->SetUnsigned(result);
}

//Bitwise And Register A with another number.
void And(uint8_t data)
{
    CPU* cpu = CPU::cpu;

    int8_t rA = cpu->A->GetUnsigned();

    uint8_t result = rA & data;

    cpu->SetFlags(
        (result & 0b10000000) > 0,
        result != 0,
        0,
        !(bits_in(*(uint8_t*)&result) % 2),
        0
    );

    cpu->A->SetSigned(result);
}

//-------------------Instructions--------------------
//It's all pretty straight-forward.
//Only comments on "weird" stuff.
//Questions, pointing out mistakes/bugs etc. is all welcome.

int ACIData(int bytes)
{
    CPU* cpu = CPU::cpu;

    int8_t data = cpu->NextPC();

    AddSignedWithCarry(data);

    return 7;
}

int ADCA(int bytes)
{
    AddSignedWithCarry(CPU::cpu->A->GetSigned());

    return 4;
}

int ADCB(int bytes)
{
    AddSignedWithCarry(CPU::cpu->B->GetSigned());

    return 4;
}

int ADCC(int bytes)
{
    AddSignedWithCarry(CPU::cpu->C->GetSigned());

    return 4;
}

int ADCD(int bytes)
{
    AddSignedWithCarry(CPU::cpu->D->GetSigned());

    return 4;
}

int ADCE(int bytes)
{
    AddSignedWithCarry(CPU::cpu->E->GetSigned());

    return 4;
}

int ADCH(int bytes)
{
    AddSignedWithCarry(CPU::cpu->H->GetSigned());

    return 4;
}

int ADCL(int bytes)
{
    AddSignedWithCarry(CPU::cpu->L->GetSigned());

    return 4;
}

int ADCM(int bytes)
{
    AddSignedWithCarry(CPU::cpu->GetSignedM());

    return 7;
}

int ADDA(int bytes)
{
    AddSigned(CPU::cpu->A->GetSigned());
    
    return 4;
}

int ADDB(int bytes)
{
    AddSigned(CPU::cpu->B->GetSigned());

    return 4;
}

int ADDC(int bytes)
{
    AddSigned(CPU::cpu->C->GetSigned());

    return 4;
}

int ADDD(int bytes)
{
    AddSigned(CPU::cpu->D->GetSigned());

    return 4;
}

int ADDE(int bytes)
{
    AddSigned(CPU::cpu->E->GetSigned());

    return 4;
}

int ADDH(int bytes)
{
    AddSigned(CPU::cpu->H->GetSigned());

    return 4;
}

int ADDL(int bytes)
{
    AddSigned(CPU::cpu->L->GetSigned());

    return 4;
}

int ADDM(int bytes)
{
    AddSigned(CPU::cpu->GetSignedM());

    return 7;
}

int ADIData(int bytes)
{
    AddSigned(CPU::cpu->NextPC());

    return 7;
}

int ANAA(int bytes)
{
    And(CPU::cpu->A->GetUnsigned());

    return 4;
}

int ANAB(int bytes)
{
    And(CPU::cpu->B->GetUnsigned());

    return 4;
}

int ANAC(int bytes)
{
    And(CPU::cpu->C->GetUnsigned());

    return 4;
}

int ANAD(int bytes)
{
    And(CPU::cpu->D->GetUnsigned());

    return 4;
}

int ANAE(int bytes)
{
    And(CPU::cpu->E->GetUnsigned());

    return 4;
}

int ANAH(int bytes)
{
    And(CPU::cpu->H->GetUnsigned());

    return 4;
}

int ANAL(int bytes)
{
    And(CPU::cpu->L->GetUnsigned());

    return 4;
}

int ANAM(int bytes)
{
    And(CPU::cpu->GetUnsignedM());

    return 7;
}

int ANIData(int bytes)
{
    And(CPU::cpu->NextPC());

    return 7;
}

int CALLLabel(int bytes)
{
    uint16_t addr = GetNextPC16();

    CPU* cpu = CPU::cpu;

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int CCLabel(int bytes)
{
    CPU* cpu = CPU::cpu;
    
    if (!cpu->Flags->GetBit(CARRY_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int CMLabel(int bytes)
{
    CPU* cpu = CPU::cpu;

    if (!cpu->Flags->GetBit(SIGN_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int CMA(int bytes)
{
    CPU::cpu->A->SetUnsigned(~CPU::cpu->A->GetUnsigned());

    return 4;
}

int CMC(int bytes)
{
    uint8_t newCy = (~CPU::cpu->Flags->GetBit(CARRY_FLAG) & 1);
    if(newCy)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int CMPA(int bytes)
{
    CPU::cpu->Flags->ClearBit(CARRY_FLAG);
    CPU::cpu->Flags->SetBit(ZERO_FLAG, 1);

    return 4;
}

int CMPB(int bytes)
{
    CPU* cpu = CPU::cpu;
    int8_t B = cpu->B->GetSigned();
    
    Compare(B);

    return 4;
}

int CMPC(int bytes)
{
    CPU* cpu = CPU::cpu;
    int8_t C = cpu->C->GetSigned();

    Compare(C);

    return 4;
}

int CMPD(int bytes)
{
    CPU* cpu = CPU::cpu;
    int8_t D = cpu->D->GetSigned();

    Compare(D);

    return 4;
}

int CMPE(int bytes)
{
    CPU* cpu = CPU::cpu;
    int8_t E = cpu->E->GetSigned();

    Compare(E);

    return 4;
}

int CMPH(int bytes)
{
    CPU* cpu = CPU::cpu;
    int8_t H = cpu->H->GetSigned();

    Compare(H);

    return 4;
}

int CMPL(int bytes)
{
    CPU* cpu = CPU::cpu;
    int8_t L = cpu->L->GetSigned();

    Compare(L);

    return 4;
}

int CMPM(int bytes)
{
    CPU* cpu = CPU::cpu;
    int8_t M= cpu->GetSignedM();

    Compare(M);

    return 7;
}

int CNCLabel(int bytes)
{
    CPU* cpu = CPU::cpu;

    if (cpu->Flags->GetBit(CARRY_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int CNZLabel(int bytes)
{
    CPU* cpu = CPU::cpu;

    if (cpu->Flags->GetBit(ZERO_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int CPLabel(int bytes)
{
    CPU* cpu = CPU::cpu;

    if (cpu->Flags->GetBit(SIGN_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int CPELabel(int bytes)
{
    CPU* cpu = CPU::cpu;

    if (!cpu->Flags->GetBit(PARITY_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int CPIData(int bytes)
{
    CPU* cpu = CPU::cpu;
    int8_t L = cpu->NextPC();

    Compare(L);

    return 7;
}

int CPOLabel(int bytes)
{
    CPU* cpu = CPU::cpu;

    if (cpu->Flags->GetBit(PARITY_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int CZLabel(int bytes)
{
    CPU* cpu = CPU::cpu;

    if (!cpu->Flags->GetBit(ZERO_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    uint16_t PC = cpu->PC->Get();
    PC += (1);

    uint8_t HIGH = (PC >> 8);
    uint8_t LOW = (PC & 0xff);

    cpu->_Stack->Push(HIGH);
    cpu->_Stack->Push(LOW);

    CPU::cpu->PC->Set(addr);

    return 18;
}

int DAA(int bytes)
{
    CPU* cpu = CPU::cpu;

    uint8_t A = cpu->A->GetUnsigned();

    uint8_t tens = A / 10;
    uint8_t ones = A - (tens*10); // A % 10

    uint8_t result = ((tens & 0x0f) << 4) | (ones & 0x0f);

    cpu->A->SetUnsigned(result);

    cpu->SetFlags(
        (result & 0b10000000) > 0,
        result == 0,
        0,
        !(bits_in(result) % 2),
        0 // TODO: CARRY SHOULD BE CALCULATED!
    );

    return 4;
}

int DADB(int bytes)
{
    int16_t BC = GetBCSigned();
    int16_t HL = GetHLSigned();

    int32_t res = BC + HL;

    int16_t newHL = res & (0xffff);

    if ((newHL & 0xffff0000) > 1)
    {
        CPU::cpu->Flags->SetBit(CARRY_FLAG, 1);
    }
    else
    {
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);
    }

    uint8_t high = (newHL >> 8) & 0xff;
    uint8_t low = newHL & 0xff;

    CPU::cpu->H->SetSigned(high);
    CPU::cpu->L->SetSigned(low);

    return 10;
}

int DADD(int bytes)
{
    int16_t DE = GetDESigned();
    int16_t HL = GetHLSigned();

    int32_t res = DE + HL;

    int16_t newHL = res & (0xffff);

    if ((newHL & 0xffff0000) > 1)
    {
        CPU::cpu->Flags->SetBit(CARRY_FLAG, 1);
    }
    else
    {
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);
    }

    uint8_t high = (newHL >> 8) & 0xff;
    uint8_t low = newHL & 0xff;

    CPU::cpu->H->SetSigned(high);
    CPU::cpu->L->SetSigned(low);

    return 10;
}

int DADH(int bytes)
{
    int16_t HL = GetHLSigned();

    int32_t res = HL + HL;

    int16_t newHL = res & (0xffff);

    if ((newHL & 0xffff0000) > 1)
    {
        CPU::cpu->Flags->SetBit(CARRY_FLAG, 1);
    }
    else
    {
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);
    }

    uint8_t high = (newHL >> 8) & 0xff;
    uint8_t low = newHL & 0xff;

    CPU::cpu->H->SetSigned(high);
    CPU::cpu->L->SetSigned(low);

    return 10;
}

int DADSP(int bytes)
{
    uint16_t uSP = CPU::cpu->SP->Get();
    int16_t SP = *(int16_t*)&uSP;
    int16_t HL = GetHLSigned();

    int32_t res = SP + HL;

    int16_t newHL = res & (0xffff);

    if ((newHL & 0xffff0000) > 1)
    {
        CPU::cpu->Flags->SetBit(CARRY_FLAG, 1);
    }
    else
    {
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);
    }

    uint8_t high = (newHL >> 8) & 0xff;
    uint8_t low = newHL & 0xff;

    CPU::cpu->H->SetSigned(high);
    CPU::cpu->L->SetSigned(low);

    return 10;
}

int DCRA(int bytes)
{
    CPU::cpu->A->Decrement();
    
    SetFlagsBasedOn(CPU::cpu->A->GetSigned());

    return 4;
}

int DCRB(int bytes)
{
    CPU::cpu->B->Decrement();

    SetFlagsBasedOn(CPU::cpu->B->GetSigned());

    return 4;
}

int DCRC(int bytes)
{
    CPU::cpu->C->Decrement();

    SetFlagsBasedOn(CPU::cpu->C->GetSigned());

    return 4;
}

int DCRD(int bytes)
{
    CPU::cpu->D->Decrement();

    SetFlagsBasedOn(CPU::cpu->D->GetSigned());

    return 4;
}

int DCRE(int bytes)
{
    CPU::cpu->E->Decrement();

    SetFlagsBasedOn(CPU::cpu->E->GetSigned());

    return 4;
}

int DCRH(int bytes)
{
    CPU::cpu->H->Decrement();

    SetFlagsBasedOn(CPU::cpu->H->GetSigned());

    return 4;
}

int DCRL(int bytes)
{
    CPU::cpu->L->Decrement();

    SetFlagsBasedOn(CPU::cpu->L->GetSigned());

    return 4;
}

int DCRM(int bytes)
{
    uint8_t M = CPU::cpu->GetUnsignedM();
    M--;

    uint16_t addr = GetHLUnsigned();
    CPU::cpu->GetMemory()->SetDataAtAddr(addr, M);

    SetFlagsBasedOn(M);

    return 10;
}

int DCXB(int bytes)
{
    int16_t BC = GetBCSigned();
    BC--;

    int8_t HIGH = (BC >> 8) & 0xff;
    int8_t LOW = BC & 0xff;

    CPU::cpu->B->SetSigned(HIGH);
    CPU::cpu->C->SetSigned(LOW);

    return 6;
}

int DCXD(int bytes)
{
    int16_t DE = GetDESigned();
    DE--;

    int8_t HIGH = (DE >> 8) & 0xff;
    int8_t LOW = DE & 0xff;

    CPU::cpu->D->SetSigned(HIGH);
    CPU::cpu->E->SetSigned(LOW);

    return 6;
}

int DCXH(int bytes)
{
    int16_t HL = GetHLSigned();
    HL--;

    int8_t HIGH = (HL >> 8) & 0xff;
    int8_t LOW = HL & 0xff;

    CPU::cpu->H->SetSigned(HIGH);
    CPU::cpu->L->SetSigned(LOW);

    return 6;
}

int DCXSP(int bytes)
{
    CPU::cpu->SP->Decrement();
    return 6;
}

int DI(int bytes) // INTERRUPTS
{
    CPU::cpu->_InterruptsEnabled = false;

    return 4;
}

int EI(int bytes)
{
    CPU::cpu->_InterruptsEnabled = true;

    return 4;
}

int HLT(int bytes)
{
    //CPU::cpu->SetRunning(false);
    CPU::cpu->SetHalted(true);

    return 5;
}

int INPortAddress(int bytes) // PORTS
{
    uint8_t addr = CPU::cpu->NextPC();
    CPU::cpu->_IOchip->WaitResponse(addr);
    uint8_t val = CPU::cpu->_IOchip->GetDataAtAddr(addr);
    CPU::cpu->A->SetUnsigned(val);

    return 10;
}

int INRA(int bytes)
{
    CPU::cpu->A->Increment();

    SetFlagsBasedOn(CPU::cpu->A->GetSigned());

    return 4;
}

int INRB(int bytes)
{
    CPU::cpu->B->Increment();

    SetFlagsBasedOn(CPU::cpu->B->GetSigned());

    return 4;
}

int INRC(int bytes)
{
    CPU::cpu->C->Increment();

    SetFlagsBasedOn(CPU::cpu->C->GetSigned());

    return 4;
}

int INRD(int bytes)
{
    CPU::cpu->D->Increment();

    SetFlagsBasedOn(CPU::cpu->D->GetSigned());

    return 4;
}

int INRE(int bytes)
{
    CPU::cpu->E->Increment();

    SetFlagsBasedOn(CPU::cpu->E->GetSigned());

    return 4;
}

int INRH(int bytes)
{
    CPU::cpu->H->Increment();

    SetFlagsBasedOn(CPU::cpu->H->GetSigned());

    return 4;
}

int INRL(int bytes)
{
    CPU::cpu->L->Increment();

    SetFlagsBasedOn(CPU::cpu->L->GetSigned());

    return 4;
}

int INRM(int bytes)
{
    uint8_t M = CPU::cpu->GetUnsignedM();
    M--;

    uint16_t addr = GetHLUnsigned();
    CPU::cpu->GetMemory()->SetDataAtAddr(addr, M);

    SetFlagsBasedOn(M);

    return 10;
}

int INXB(int bytes)
{
    int16_t BC = GetBCSigned();
    BC++;

    int8_t HIGH = (BC >> 8) & 0xff;
    int8_t LOW = BC & 0xff;

    CPU::cpu->B->SetSigned(HIGH);
    CPU::cpu->C->SetSigned(LOW);

    return 6;
}

int INXD(int bytes)
{
    int16_t DE = GetDESigned();
    DE++;

    int8_t HIGH = (DE >> 8) & 0xff;
    int8_t LOW = DE & 0xff;

    CPU::cpu->D->SetSigned(HIGH);
    CPU::cpu->E->SetSigned(LOW);

    return 6;
}

int INXH(int bytes)
{
    int16_t HL = GetHLSigned();
    HL++;

    int8_t HIGH = (HL >> 8) & 0xff;
    int8_t LOW = HL & 0xff;

    CPU::cpu->H->SetSigned(HIGH);
    CPU::cpu->L->SetSigned(LOW);

    return 6;
}

int INXSP(int bytes)
{
    CPU::cpu->SP->Increment();
    return 6;
}

int JCLabel(int bytes)
{
    if (!CPU::cpu->Flags->GetBit(CARRY_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int JMLabel(int bytes)
{
    if (!CPU::cpu->Flags->GetBit(SIGN_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int JMPLabel(int bytes)
{
    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int JNCLabel(int bytes)
{
    if (CPU::cpu->Flags->GetBit(CARRY_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int JNZLabel(int bytes)
{
    if (CPU::cpu->Flags->GetBit(ZERO_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int JPLabel(int bytes)
{
    if (CPU::cpu->Flags->GetBit(SIGN_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int JPELabel(int bytes)
{
    if (!CPU::cpu->Flags->GetBit(PARITY_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int JPOLabel(int bytes)
{
    if (CPU::cpu->Flags->GetBit(PARITY_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int JZLabel(int bytes)
{
    if (!CPU::cpu->Flags->GetBit(ZERO_FLAG))
    {
        CPU::cpu->PC->Increment();
        CPU::cpu->PC->Increment();
        return 1;
    }

    uint16_t addr = GetNextPC16();

    CPU::cpu->PC->Set(addr);

    return 10;
}

int LDAAddress(int bytes)
{
    uint16_t addr = GetNextPC16();

    CPU::cpu->A->SetUnsigned(CPU::cpu->GetMemory()->GetDataAtAddr(addr));

    return 13;
}

int LDAXB(int bytes)
{
    uint16_t addr = GetBCUnsigned();

    CPU::cpu->A->SetUnsigned(CPU::cpu->GetMemory()->GetDataAtAddr(addr));

    return 7;
}

int LDAXD(int bytes)
{
    uint16_t addr = GetDEUnsigned();

    CPU::cpu->A->SetUnsigned(CPU::cpu->GetMemory()->GetDataAtAddr(addr));

    return 7;
}

int LHLDAddress(int bytes)
{
    uint16_t addr = GetDEUnsigned();

    CPU::cpu->L->SetUnsigned(CPU::cpu->GetMemory()->GetDataAtAddr(addr));
    CPU::cpu->H->SetUnsigned(CPU::cpu->GetMemory()->GetDataAtAddr(addr+1));

    return 16;
}

int LXIB(int bytes)
{
    uint16_t val = GetNextPC16();

    int8_t HIGH = (val >> 8) & 0xff;
    int8_t LOW = val & 0xff;

    CPU::cpu->B->SetSigned(HIGH);
    CPU::cpu->C->SetSigned(LOW);

    return 10;
}

int LXID(int bytes)
{
    uint16_t val = GetNextPC16();

    int8_t HIGH = (val >> 8) & 0xff;
    int8_t LOW = val & 0xff;

    CPU::cpu->D->SetSigned(HIGH);
    CPU::cpu->E->SetSigned(LOW);

    return 10;
}

int LXIH(int bytes)
{
    uint16_t val = GetNextPC16();

    int8_t HIGH = (val >> 8) & 0xff;
    int8_t LOW = val & 0xff;

    CPU::cpu->H->SetSigned(HIGH);
    CPU::cpu->L->SetSigned(LOW);

    return 10;
}

int LXISP(int bytes)
{
    uint16_t val = GetNextPC16();

    CPU::cpu->SP->Set(val);

    return 10;
}

int MOVAA(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned());

    return 4;
}

int MOVAB(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->B->GetUnsigned());

    return 4;
}

int MOVAC(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->C->GetUnsigned());

    return 4;
}

int MOVAD(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->D->GetUnsigned());

    return 4;
}

int MOVAE(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->E->GetUnsigned());

    return 4;
}

int MOVAH(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->H->GetUnsigned());

    return 4;
}

int MOVAL(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->L->GetUnsigned());

    return 4;
}

int MOVAM(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->GetUnsignedM());

    return 7;
}

int MOVBA(int bytes)
{
    CPU::cpu->B->SetUnsigned(CPU::cpu->A->GetUnsigned());

    return 4;
}

int MOVBB(int bytes)
{
    CPU::cpu->B->SetUnsigned(CPU::cpu->B->GetUnsigned());

    return 4;
}

int MOVBC(int bytes)
{
    CPU::cpu->B->SetUnsigned(CPU::cpu->C->GetUnsigned());

    return 4;
}

int MOVBD(int bytes)
{
    CPU::cpu->B->SetUnsigned(CPU::cpu->D->GetUnsigned());

    return 4;
}

int MOVBE(int bytes)
{
    CPU::cpu->B->SetUnsigned(CPU::cpu->E->GetUnsigned());

    return 4;
}

int MOVBH(int bytes)
{
    CPU::cpu->B->SetUnsigned(CPU::cpu->H->GetUnsigned());

    return 4;
}

int MOVBL(int bytes)
{
    CPU::cpu->B->SetUnsigned(CPU::cpu->L->GetUnsigned());

    return 4;
}

int MOVBM(int bytes)
{
    CPU::cpu->B->SetUnsigned(CPU::cpu->GetUnsignedM());

    return 7;
}

int MOVCA(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->A->GetUnsigned());

    return 4;
}

int MOVCB(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->B->GetUnsigned());

    return 4;
}

int MOVCC(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->C->GetUnsigned());

    return 4;
}

int MOVCD(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->D->GetUnsigned());

    return 4;
}

int MOVCE(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->E->GetUnsigned());

    return 4;
}

int MOVCH(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->H->GetUnsigned());

    return 4;
}

int MOVCL(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->L->GetUnsigned());

    return 4;
}

int MOVCM(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->GetUnsignedM());

    return 7;
}

int MOVDA(int bytes)
{
    CPU::cpu->D->SetUnsigned(CPU::cpu->A->GetUnsigned());

    return 4;
}

int MOVDB(int bytes)
{
    CPU::cpu->D->SetUnsigned(CPU::cpu->B->GetUnsigned());

    return 4;
}

int MOVDC(int bytes)
{
    CPU::cpu->D->SetUnsigned(CPU::cpu->C->GetUnsigned());

    return 4;
}

int MOVDD(int bytes)
{
    CPU::cpu->D->SetUnsigned(CPU::cpu->D->GetUnsigned());

    return 4;
}

int MOVDE(int bytes)
{
    CPU::cpu->D->SetUnsigned(CPU::cpu->E->GetUnsigned());

    return 4;
}

int MOVDH(int bytes)
{
    CPU::cpu->D->SetUnsigned(CPU::cpu->H->GetUnsigned());

    return 4;
}

int MOVDL(int bytes)
{
    CPU::cpu->D->SetUnsigned(CPU::cpu->L->GetUnsigned());

    return 4;
}

int MOVDM(int bytes)
{
    CPU::cpu->D->SetUnsigned(CPU::cpu->GetUnsignedM());

    return 7;
}

int MOVEA(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->A->GetUnsigned());

    return 4;
}

int MOVEB(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->B->GetUnsigned());

    return 4;
}

int MOVEC(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->C->GetUnsigned());

    return 4;
}

int MOVED(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->D->GetUnsigned());

    return 4;
}

int MOVEE(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->E->GetUnsigned());

    return 4;
}

int MOVEH(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->H->GetUnsigned());

    return 4;
}

int MOVEL(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->L->GetUnsigned());

    return 4;
}

int MOVEM(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->GetUnsignedM());

    return 7;
}

int MOVHA(int bytes)
{
    CPU::cpu->H->SetUnsigned(CPU::cpu->A->GetUnsigned());

    return 4;
}

int MOVHB(int bytes)
{
    CPU::cpu->H->SetUnsigned(CPU::cpu->B->GetUnsigned());

    return 4;
}

int MOVHC(int bytes)
{
    CPU::cpu->H->SetUnsigned(CPU::cpu->C->GetUnsigned());

    return 4;
}

int MOVHD(int bytes)
{
    CPU::cpu->H->SetUnsigned(CPU::cpu->D->GetUnsigned());

    return 4;
}

int MOVHE(int bytes)
{
    CPU::cpu->H->SetUnsigned(CPU::cpu->E->GetUnsigned());

    return 4;
}

int MOVHH(int bytes)
{
    CPU::cpu->H->SetUnsigned(CPU::cpu->H->GetUnsigned());

    return 4;
}

int MOVHL(int bytes)
{
    CPU::cpu->H->SetUnsigned(CPU::cpu->L->GetUnsigned());

    return 4;
}

int MOVHM(int bytes)
{
    CPU::cpu->H->SetUnsigned(CPU::cpu->GetUnsignedM());

    return 7;
}

int MOVLA(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->A->GetUnsigned());

    return 4;
}

int MOVLB(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->B->GetUnsigned());

    return 4;
}

int MOVLC(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->C->GetUnsigned());

    return 4;
}

int MOVLD(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->D->GetUnsigned());

    return 4;
}

int MOVLE(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->E->GetUnsigned());

    return 4;
}

int MOVLH(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->H->GetUnsigned());

    return 4;
}

int MOVLL(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->L->GetUnsigned());

    return 4;
}

int MOVLM(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->GetUnsignedM());

    return 7;
}

int MOVMA(int bytes)
{
    CPU::cpu->GetMemory()->SetDataAtAddr(GetHLUnsigned(), CPU::cpu->A->GetUnsigned());

    return 7;
}

int MOVMB(int bytes)
{
    CPU::cpu->GetMemory()->SetDataAtAddr(GetHLUnsigned(), CPU::cpu->B->GetUnsigned());

    return 7;
}

int MOVMC(int bytes)
{
    CPU::cpu->GetMemory()->SetDataAtAddr(GetHLUnsigned(), CPU::cpu->C->GetUnsigned());

    return 7;
}

int MOVMD(int bytes)
{
    CPU::cpu->GetMemory()->SetDataAtAddr(GetHLUnsigned(), CPU::cpu->D->GetUnsigned());

    return 7;
}

int MOVME(int bytes)
{
    CPU::cpu->GetMemory()->SetDataAtAddr(GetHLUnsigned(), CPU::cpu->E->GetUnsigned());

    return 7;
}

int MOVMH(int bytes)
{
    CPU::cpu->GetMemory()->SetDataAtAddr(GetHLUnsigned(), CPU::cpu->H->GetUnsigned());

    return 7;
}

int MOVML(int bytes)
{
    CPU::cpu->GetMemory()->SetDataAtAddr(GetHLUnsigned(), CPU::cpu->L->GetUnsigned());

    return 7;
}

int MVIAData(int bytes)
{
    uint8_t val = CPU::cpu->NextPC();
    CPU::cpu->A->SetUnsigned(val);

    return 7;
}

int MVIBData(int bytes)
{
    uint8_t val = CPU::cpu->NextPC();
    CPU::cpu->B->SetUnsigned(val);

    return 7;
}

int MVICData(int bytes)
{
    uint8_t val = CPU::cpu->NextPC();
    CPU::cpu->C->SetUnsigned(val);

    return 7;
}

int MVIDData(int bytes)
{
    uint8_t val = CPU::cpu->NextPC();
    CPU::cpu->D->SetUnsigned(val);

    return 7;
}

int MVIEData(int bytes)
{
    uint8_t val = CPU::cpu->NextPC();
    CPU::cpu->E->SetUnsigned(val);

    return 7;
}

int MVIHData(int bytes)
{
    uint8_t val = CPU::cpu->NextPC();
    CPU::cpu->H->SetUnsigned(val);

    return 7;
}

int MVILData(int bytes)
{
    uint8_t val = CPU::cpu->NextPC();
    CPU::cpu->L->SetUnsigned(val);

    return 7;
}

int MVIMData(int bytes)
{
    uint8_t val = CPU::cpu->NextPC();
    CPU::cpu->GetMemory()->SetDataAtAddr(GetHLUnsigned(), val);

    return 10;
}

int NOP(int bytes)
{
    return 4;
}

int ORAA(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->A->GetUnsigned());

    return 4;
}

int ORAB(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->B->GetUnsigned());

    return 4;
}

int ORAC(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->C->GetUnsigned());

    return 4;
}

int ORAD(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->D->GetUnsigned());

    return 4;
}

int ORAE(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->E->GetUnsigned());

    return 4;
}

int ORAH(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->H->GetUnsigned());

    return 4;
}

int ORAL(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->L->GetUnsigned());

    return 4;
}

int ORAM(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->GetUnsignedM());

    return 7;
}

int ORIData(int bytes)
{
    CPU::cpu->A->SetUnsigned(CPU::cpu->A->GetUnsigned() | CPU::cpu->NextPC());

    return 7;
}

int OUTPortAddress(int bytes) // PORT
{
    uint8_t addr = CPU::cpu->NextPC();
    CPU::cpu->_IOchip->SetDataAtAddr(addr, CPU::cpu->A->GetUnsigned());
    CPU::cpu->_IOchip->WaitResponse(addr);

    return 10;
}

int PCHL(int bytes)
{
    uint16_t addr = GetHLUnsigned();

    CPU::cpu->PC->Set(addr - 1);

    return 6;
}

int POPB(int bytes)
{
    CPU::cpu->C->SetUnsigned(CPU::cpu->_Stack->Pop());
    CPU::cpu->B->SetUnsigned(CPU::cpu->_Stack->Pop());

    return 10;
}

int POPD(int bytes)
{
    CPU::cpu->E->SetUnsigned(CPU::cpu->_Stack->Pop());
    CPU::cpu->D->SetUnsigned(CPU::cpu->_Stack->Pop());

    return 10;
}

int POPH(int bytes)
{
    CPU::cpu->L->SetUnsigned(CPU::cpu->_Stack->Pop());
    CPU::cpu->H->SetUnsigned(CPU::cpu->_Stack->Pop());

    return 10;
}

int POPPSW(int bytes)
{
    CPU::cpu->Flags->SetUnsigned(CPU::cpu->_Stack->Pop());
    CPU::cpu->A->SetUnsigned(CPU::cpu->_Stack->Pop());

    return 10;
}

int PUSHB(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->B->GetUnsigned());
    CPU::cpu->_Stack->Push(CPU::cpu->C->GetUnsigned());

    return 12;
}

int PUSHD(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->D->GetUnsigned());
    CPU::cpu->_Stack->Push(CPU::cpu->E->GetUnsigned());

    return 12;
}

int PUSHH(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->H->GetUnsigned());
    CPU::cpu->_Stack->Push(CPU::cpu->L->GetUnsigned());

    return 12;
}

int PUSHPSW(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->A->GetUnsigned());
    CPU::cpu->_Stack->Push(CPU::cpu->Flags->GetUnsigned());

    return 12;
}

int RAL(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();

    uint8_t newCy = (A & 0b10000000) > 0;

    A = A << 1;
    CPU::cpu->A->SetUnsigned(A);
    CPU::cpu->A->SetBit(0, CPU::cpu->Flags->GetBit(CARRY_FLAG));

    if(newCy)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int RAR(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();

    uint8_t newCy = (A & 0b00000001) > 0;

    A = A >> 1;
    CPU::cpu->A->SetUnsigned(A);
    CPU::cpu->A->SetBit(7, CPU::cpu->Flags->GetBit(CARRY_FLAG));

    if (newCy)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int RC(int bytes)
{
    if (!CPU::cpu->Flags->GetBit(CARRY_FLAG))
    {
        return 1;
    }

    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int RET(int bytes)
{
    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int RIM(int bytes) // INTERRUPT
{

    bool M55 = CPU::cpu->_M55; 
    bool M65 = CPU::cpu->_M65; 
    bool M75 = CPU::cpu->_M75; 
    bool IE = CPU::cpu->_InterruptsEnabled; 
    bool IP55 = CPU::cpu->_IP55;
    bool IP65 = CPU::cpu->_IP65;
    bool IP75 = CPU::cpu->_IP75;

    CPU::cpu->A->SetBit(0, M55);
    CPU::cpu->A->SetBit(1, M65);
    CPU::cpu->A->SetBit(2, M75);
    CPU::cpu->A->SetBit(3, IE);
    CPU::cpu->A->SetBit(4, IP55);
    CPU::cpu->A->SetBit(5, IP65);
    CPU::cpu->A->SetBit(6, IP75);
    CPU::cpu->A->SetBit(7, 0);

    return 4;
}

int RLC(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();

    uint8_t newCy = (A & 0b10000000) > 0;

    A = A << 1;
    CPU::cpu->A->SetUnsigned(A);
    CPU::cpu->A->SetBit(0, newCy);

    if (newCy)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int DSUB(int bytes)
{
    uint16_t HL = GetHLSigned();
    uint16_t BC = GetBCSigned();

    uint16_t result = HL - BC;

    int8_t HIGH = (result >> 8) & 0xff;
    int8_t LOW = result & 0xff;

    CPU::cpu->H->SetSigned(HIGH);
    CPU::cpu->L->SetSigned(LOW);

    return 8;
}

int RM(int bytes)
{
    if (!CPU::cpu->Flags->GetBit(SIGN_FLAG))
    {
        return 1;
    }

    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int RNC(int bytes)
{
    if (CPU::cpu->Flags->GetBit(CARRY_FLAG))
    {
        return 1;
    }

    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int RNZ(int bytes)
{
    if (CPU::cpu->Flags->GetBit(ZERO_FLAG))
    {
        return 1;
    }

    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int RP(int bytes)
{

    if (CPU::cpu->Flags->GetBit(SIGN_FLAG))
    {
        return 1;
    }

    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int RPE(int bytes)
{
    if (!CPU::cpu->Flags->GetBit(PARITY_FLAG))
    {
        return 1;
    }

    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int RPO(int bytes)
{
    if (CPU::cpu->Flags->GetBit(PARITY_FLAG))
    {
        return 1;
    }

    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int RRC(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();

    uint8_t newCy = (A & 0b00000001) > 0;

    A = A >> 1;
    CPU::cpu->A->SetUnsigned(A);
    CPU::cpu->A->SetBit(7, newCy);

    if (newCy)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int RST0(int bytes) 
{
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

    CPU::cpu->PC->Set(0x0000 - 1); //Intentional overflow.

    return 12;
}

int RST1(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

    CPU::cpu->PC->Set(0x0008 - 1);

    return 12;
}

int RST2(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

    CPU::cpu->PC->Set(0x0010 - 1);

    return 12;
}

int RST3(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

    CPU::cpu->PC->Set(0x0018 - 1);

    return 12;
}

int RST4(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

    CPU::cpu->PC->Set(0x0020 - 1);

    return 12;
}

int RST5(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

    CPU::cpu->PC->Set(0x0028 - 1);

    return 12;
}

int RST6(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

    CPU::cpu->PC->Set(0x0030 - 1);

    return 12;
}

int RST7(int bytes)
{
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
    CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

    CPU::cpu->PC->Set(0x0038 - 1);

    return 12;
}

int RZ(int bytes)
{
    if (!CPU::cpu->Flags->GetBit(ZERO_FLAG))
    {
        return 1;
    }

    uint8_t LOW = CPU::cpu->_Stack->Pop();
    uint8_t HIGH = CPU::cpu->_Stack->Pop();

    uint16_t addr = (HIGH << 8) | LOW;

    CPU::cpu->PC->Set(addr - 1);

    return 12;
}

int SBBA(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);
    
    int8_t other = (~(A+Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SBBB(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);

    int8_t other = (~(CPU::cpu->B->GetSigned() + Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SBBC(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);

    int8_t other = (~(CPU::cpu->C->GetSigned() + Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SBBD(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);

    int8_t other = (~(CPU::cpu->D->GetSigned() + Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SBBE(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);

    int8_t other = (~(CPU::cpu->E->GetSigned() + Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SBBH(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);

    int8_t other = (~(CPU::cpu->H->GetSigned() + Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SBBL(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);

    int8_t other = (~(CPU::cpu->L->GetSigned() + Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SBBM(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);

    int8_t other = (~(CPU::cpu->GetSignedM() + Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 7;
}

int SBIData(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t Cy = CPU::cpu->Flags->GetBit(CARRY_FLAG);

    int8_t other = (~(CPU::cpu->NextPC() + Cy)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 7;
}

int SHLDAddress(int bytes)
{
    uint16_t addr = GetNextPC16();

    CPU::cpu->GetMemory()->SetDataAtAddr(addr, CPU::cpu->L->GetUnsigned());
    CPU::cpu->GetMemory()->SetDataAtAddr(addr+1, CPU::cpu->H->GetUnsigned());

    return 16;
}

int SIM(int bytes) // interrupt
{
    bool M55 = CPU::cpu->A->GetBit(0);
    bool M65 = CPU::cpu->A->GetBit(1);
    bool M75 = CPU::cpu->A->GetBit(2);
    bool MSE = CPU::cpu->A->GetBit(3);
    bool R75 = CPU::cpu->A->GetBit(4);

    if (MSE)
    {
        CPU::cpu->_M55 = M55;
        CPU::cpu->_M65 = M65;
        CPU::cpu->_M75 = M75;
    }

    if (R75)
    {
        CPU::cpu->_IP75 = false;
    }

    return 4;
}

int SPHL(int bytes)
{
    CPU::cpu->SP->Set(GetHLUnsigned());

    return 6;
}

int STAAddress(int bytes)
{
    uint16_t addr = GetNextPC16();

    CPU::cpu->GetMemory()->SetDataAtAddr(addr, CPU::cpu->A->GetUnsigned());

    return 13;
}

int STAXB(int bytes)
{
    uint16_t addr = GetBCUnsigned();

    CPU::cpu->GetMemory()->SetDataAtAddr(addr, CPU::cpu->A->GetUnsigned());

    return 7;
}

int STAXD(int bytes)
{
    uint16_t addr = GetDEUnsigned();

    CPU::cpu->GetMemory()->SetDataAtAddr(addr, CPU::cpu->A->GetUnsigned());

    return 7;
}

int STC(int bytes)
{
    CPU::cpu->Flags->SetBit(CARRY_FLAG);

    return 4;
}

int SUBA(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(A)) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SUBB(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(CPU::cpu->B->GetSigned())) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SUBC(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(CPU::cpu->C->GetSigned())) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SUBD(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(CPU::cpu->D->GetSigned())) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SUBE(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(CPU::cpu->E->GetSigned())) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SUBH(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(CPU::cpu->H->GetSigned())) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SUBL(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(CPU::cpu->L->GetSigned())) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 4;
}

int SUBM(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(CPU::cpu->GetSignedM())) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 7;
}

int SUIData(int bytes)
{
    int8_t A = CPU::cpu->A->GetSigned();

    int8_t other = (~(CPU::cpu->NextPC())) + 1;

    int16_t res = A + other;

    CPU::cpu->A->SetSigned(res & 0xff);

    SetFlagsBasedOn(A);

    if ((res & 0xff) != res)
        CPU::cpu->Flags->SetBit(CARRY_FLAG);
    else
        CPU::cpu->Flags->ClearBit(CARRY_FLAG);

    return 7;
}

int XCHG(int bytes)
{
    uint8_t H = CPU::cpu->H->GetUnsigned();
    uint8_t L = CPU::cpu->L->GetUnsigned();

    uint8_t D = CPU::cpu->D->GetUnsigned();
    uint8_t E = CPU::cpu->E->GetUnsigned();

    CPU::cpu->H->SetUnsigned(D);
    CPU::cpu->L->SetUnsigned(E);

    CPU::cpu->D->SetUnsigned(H);
    CPU::cpu->E->SetUnsigned(L);

    return 4;
}

int XRAA(int bytes)
{
    CPU::cpu->A->SetUnsigned(0);

    return 4;
}

int XRAB(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();
    uint8_t other = CPU::cpu->B->GetUnsigned();

    CPU::cpu->A->SetUnsigned(A ^ other);

    return 4;
}

int XRAC(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();
    uint8_t other = CPU::cpu->C->GetUnsigned();

    CPU::cpu->A->SetUnsigned(A ^ other);

    return 4;
}

int XRAD(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();
    uint8_t other = CPU::cpu->D->GetUnsigned();

    CPU::cpu->A->SetUnsigned(A ^ other);

    return 4;
}

int XRAE(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();
    uint8_t other = CPU::cpu->E->GetUnsigned();

    CPU::cpu->A->SetUnsigned(A ^ other);

    return 4;
}

int XRAH(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();
    uint8_t other = CPU::cpu->H->GetUnsigned();

    CPU::cpu->A->SetUnsigned(A ^ other);

    return 4;
}

int XRAL(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();
    uint8_t other = CPU::cpu->L->GetUnsigned();

    CPU::cpu->A->SetUnsigned(A ^ other);

    return 4;
}

int XRAM(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();
    uint8_t other = CPU::cpu->GetUnsignedM();

    CPU::cpu->A->SetUnsigned(A ^ other);

    return 7;
}

int XRIData(int bytes)
{
    uint8_t A = CPU::cpu->A->GetUnsigned();
    uint8_t other = CPU::cpu->NextPC();

    CPU::cpu->A->SetUnsigned(A ^ other);

    return 7;
}

int XTHL(int bytes)
{
    CPU::cpu->_Stack->Pop();
    CPU::cpu->_Stack->Pop();

    CPU::cpu->_Stack->Push(CPU::cpu->H->GetUnsigned());
    CPU::cpu->_Stack->Push(CPU::cpu->L->GetUnsigned());

    return 16;
}

//--------------------Declaring instructions--------------------
//Again, using python script.
CPUInstruction CPUInstructions[] = {
    {0x0, "NOP", 1, NOP},
    {0x1, "LXI", 3, LXIB},
    {0x2, "STAX", 1, STAXB},
    {0x3, "INX", 1, INXB},
    {0x4, "INR", 1, INRB},
    {0x5, "DCR", 1, DCRB},
    {0x6, "MVI", 2, MVIBData},
    {0x7, "RLC", 1, RLC},
    {0x8, "DSUB", 1, DSUB},
    {0x9, "DAD", 1, DADB},
    {0xa, "LDAX", 1, LDAXB},
    {0xb, "DCX", 1, DCXB},
    {0xc, "INR", 1, INRC},
    {0xd, "DCR", 1, DCRC},
    {0xe, "MVI", 2, MVICData},
    {0xf, "RRC", 1, RRC},
    {},
    {0x11, "LXI", 3, LXID},
    {0x12, "STAX", 1, STAXD},
    {0x13, "INX", 1, INXD},
    {0x14, "INR", 1, INRD},
    {0x15, "DCR", 1, DCRD},
    {0x16, "MVI", 2, MVIDData},
    {0x17, "RAL", 1, RAL},
    {},
    {0x19, "DAD", 1, DADD},
    {0x1a, "LDAX", 1, LDAXD},
    {0x1b, "DCX", 1, DCXD},
    {0x1c, "INR", 1, INRE},
    {0x1d, "DCR", 1, DCRE},
    {0x1e, "MVI", 2, MVIEData},
    {0x1f, "RAR", 1, RAR},
    {0x20, "RIM", 1, RIM},
    {0x21, "LXI", 3, LXIH},
    {0x22, "SHLD", 3, SHLDAddress},
    {0x23, "INX", 1, INXH},
    {0x24, "INR", 1, INRH},
    {0x25, "DCR", 1, DCRH},
    {0x26, "MVI", 2, MVIHData},
    {0x27, "DAA", 1, DAA},
    {},
    {0x29, "DAD", 1, DADH},
    {0x2a, "LHLD", 3, LHLDAddress},
    {0x2b, "DCX", 1, DCXH},
    {0x2c, "INR", 1, INRL},
    {0x2d, "DCR", 1, DCRL},
    {0x2e, "MVI", 2, MVILData},
    {0x2f, "CMA", 1, CMA},
    {0x30, "SIM", 1, SIM},
    {0x31, "LXI", 3, LXISP},
    {0x32, "STA", 3, STAAddress},
    {0x33, "INX", 1, INXSP},
    {0x34, "INR", 1, INRM},
    {0x35, "DCR", 1, DCRM},
    {0x36, "MVI", 2, MVIMData},
    {0x37, "STC", 1, STC},
    {},
    {0x39, "DAD", 1, DADSP},
    {0x3a, "LDA", 3, LDAAddress},
    {0x3b, "DCX", 1, DCXSP},
    {0x3c, "INR", 1, INRA},
    {0x3d, "DCR", 1, DCRA},
    {0x3e, "MVI", 2, MVIAData},
    {0x3f, "CMC", 1, CMC},
    {0x40, "MOV", 1, MOVBB},
    {0x41, "MOV", 1, MOVBC},
    {0x42, "MOV", 1, MOVBD},
    {0x43, "MOV", 1, MOVBE},
    {0x44, "MOV", 1, MOVBH},
    {0x45, "MOV", 1, MOVBL},
    {0x46, "MOV", 1, MOVBM},
    {0x47, "MOV", 1, MOVBA},
    {0x48, "MOV", 1, MOVCB},
    {0x49, "MOV", 1, MOVCC},
    {0x4a, "MOV", 1, MOVCD},
    {0x4b, "MOV", 1, MOVCE},
    {0x4c, "MOV", 1, MOVCH},
    {0x4d, "MOV", 1, MOVCL},
    {0x4e, "MOV", 1, MOVCM},
    {0x4f, "MOV", 1, MOVCA},
    {0x50, "MOV", 1, MOVDB},
    {0x51, "MOV", 1, MOVDC},
    {0x52, "MOV", 1, MOVDD},
    {0x53, "MOV", 1, MOVDE},
    {0x54, "MOV", 1, MOVDH},
    {0x55, "MOV", 1, MOVDL},
    {0x56, "MOV", 1, MOVDM},
    {0x57, "MOV", 1, MOVDA},
    {0x58, "MOV", 1, MOVEB},
    {0x59, "MOV", 1, MOVEC},
    {0x5a, "MOV", 1, MOVED},
    {0x5b, "MOV", 1, MOVEE},
    {0x5c, "MOV", 1, MOVEH},
    {0x5d, "MOV", 1, MOVEL},
    {0x5e, "MOV", 1, MOVEM},
    {0x5f, "MOV", 1, MOVEA},
    {0x60, "MOV", 1, MOVHB},
    {0x61, "MOV", 1, MOVHC},
    {0x62, "MOV", 1, MOVHD},
    {0x63, "MOV", 1, MOVHE},
    {0x64, "MOV", 1, MOVHH},
    {0x65, "MOV", 1, MOVHL},
    {0x66, "MOV", 1, MOVHM},
    {0x67, "MOV", 1, MOVHA},
    {0x68, "MOV", 1, MOVLB},
    {0x69, "MOV", 1, MOVLC},
    {0x6a, "MOV", 1, MOVLD},
    {0x6b, "MOV", 1, MOVLE},
    {0x6c, "MOV", 1, MOVLH},
    {0x6d, "MOV", 1, MOVLL},
    {0x6e, "MOV", 1, MOVLM},
    {0x6f, "MOV", 1, MOVLA},
    {0x70, "MOV", 1, MOVMB},
    {0x71, "MOV", 1, MOVMC},
    {0x72, "MOV", 1, MOVMD},
    {0x73, "MOV", 1, MOVME},
    {0x74, "MOV", 1, MOVMH},
    {0x75, "MOV", 1, MOVML},
    {0x76, "HLT", 1, HLT},
    {0x77, "MOV", 1, MOVMA},
    {0x78, "MOV", 1, MOVAB},
    {0x79, "MOV", 1, MOVAC},
    {0x7a, "MOV", 1, MOVAD},
    {0x7b, "MOV", 1, MOVAE},
    {0x7c, "MOV", 1, MOVAH},
    {0x7d, "MOV", 1, MOVAL},
    {0x7e, "MOV", 1, MOVAM},
    {0x7f, "MOV", 1, MOVAA},
    {0x80, "ADD", 1, ADDB},
    {0x81, "ADD", 1, ADDC},
    {0x82, "ADD", 1, ADDD},
    {0x83, "ADD", 1, ADDE},
    {0x84, "ADD", 1, ADDH},
    {0x85, "ADD", 1, ADDL},
    {0x86, "ADD", 1, ADDM},
    {0x87, "ADD", 1, ADDA},
    {0x88, "ADC", 1, ADCB},
    {0x89, "ADC", 1, ADCC},
    {0x8a, "ADC", 1, ADCD},
    {0x8b, "ADC", 1, ADCE},
    {0x8c, "ADC", 1, ADCH},
    {0x8d, "ADC", 1, ADCL},
    {0x8e, "ADC", 1, ADCM},
    {0x8f, "ADC", 1, ADCA},
    {0x90, "SUB", 1, SUBB},
    {0x91, "SUB", 1, SUBC},
    {0x92, "SUB", 1, SUBD},
    {0x93, "SUB", 1, SUBE},
    {0x94, "SUB", 1, SUBH},
    {0x95, "SUB", 1, SUBL},
    {0x96, "SUB", 1, SUBM},
    {0x97, "SUB", 1, SUBA},
    {0x98, "SBB", 1, SBBB},
    {0x99, "SBB", 1, SBBC},
    {0x9a, "SBB", 1, SBBD},
    {0x9b, "SBB", 1, SBBE},
    {0x9c, "SBB", 1, SBBH},
    {0x9d, "SBB", 1, SBBL},
    {0x9e, "SBB", 1, SBBM},
    {0x9f, "SBB", 1, SBBA},
    {0xa0, "ANA", 1, ANAB},
    {0xa1, "ANA", 1, ANAC},
    {0xa2, "ANA", 1, ANAD},
    {0xa3, "ANA", 1, ANAE},
    {0xa4, "ANA", 1, ANAH},
    {0xa5, "ANA", 1, ANAL},
    {0xa6, "ANA", 1, ANAM},
    {0xa7, "ANA", 1, ANAA},
    {0xa8, "XRA", 1, XRAB},
    {0xa9, "XRA", 1, XRAC},
    {0xaa, "XRA", 1, XRAD},
    {0xab, "XRA", 1, XRAE},
    {0xac, "XRA", 1, XRAH},
    {0xad, "XRA", 1, XRAL},
    {0xae, "XRA", 1, XRAM},
    {0xaf, "XRA", 1, XRAA},
    {0xb0, "ORA", 1, ORAB},
    {0xb1, "ORA", 1, ORAC},
    {0xb2, "ORA", 1, ORAD},
    {0xb3, "ORA", 1, ORAE},
    {0xb4, "ORA", 1, ORAH},
    {0xb5, "ORA", 1, ORAL},
    {0xb6, "ORA", 1, ORAM},
    {0xb7, "ORA", 1, ORAA},
    {0xb8, "CMP", 1, CMPB},
    {0xb9, "CMP", 1, CMPC},
    {0xba, "CMP", 1, CMPD},
    {0xbb, "CMP", 1, CMPE},
    {0xbc, "CMP", 1, CMPH},
    {0xbd, "CMP", 1, CMPM},
    {},
    {0xbf, "CMP", 1, CMPA},
    {0xc0, "RNZ", 1, RNZ},
    {0xc1, "POP", 1, POPB},
    {0xc2, "JNZ", 3, JNZLabel},
    {0xc3, "JMP", 3, JMPLabel},
    {0xc4, "CNZ", 3, CNZLabel},
    {0xc5, "PUSH", 1, PUSHB},
    {0xc6, "ADI", 2, ADIData},
    {0xc7, "RST", 1, RST0},
    {0xc8, "RZ", 1, RZ},
    {0xc9, "RET", 1, RET},
    {0xca, "JZ", 3, JZLabel},
    {},
    {0xcc, "CZ", 3, CZLabel},
    {0xcd, "CALL", 3, CALLLabel},
    {0xce, "ACI", 2, ACIData},
    {0xcf, "RST", 1, RST1},
    {0xd0, "RNC", 1, RNC},
    {0xd1, "POP", 1, POPD},
    {0xd2, "JNC", 3, JNCLabel},
    {0xd3, "OUT", 2, OUTPortAddress},
    {0xd4, "CNC", 3, CNCLabel},
    {0xd5, "PUSH", 1, PUSHD},
    {0xd6, "SUI", 2, SUIData},
    {0xd7, "RST", 1, RST2},
    {0xd8, "RC", 1, RC},
    {},
    {0xda, "JC", 3, JCLabel},
    {0xdb, "IN", 2, INPortAddress},
    {0xdc, "CC", 3, CCLabel},
    {},
    {0xde, "SBI", 2, SBIData},
    {0xdf, "RST", 1, RST3},
    {0xe0, "RPO", 1, RPO},
    {0xe1, "POP", 1, POPH},
    {0xe2, "JPO", 3, JPOLabel},
    {0xe3, "XTHL", 1, XTHL},
    {0xe4, "CPO", 3, CPOLabel},
    {0xe5, "PUSH", 1, PUSHH},
    {0xe6, "ANI", 2, ANIData},
    {0xe7, "RST", 1, RST4},
    {0xe8, "RPE", 1, RPE},
    {0xe9, "PCHL", 1, PCHL},
    {0xea, "JPE", 3, JPELabel},
    {0xeb, "XCHG", 1, XCHG},
    {0xec, "CPE", 3, CPELabel},
    {},
    {0xee, "XRI", 2, XRIData},
    {0xef, "RST", 1, RST5},
    {0xf0, "RP", 1, RP},
    {0xf1, "POP", 1, POPPSW},
    {0xf2, "JP", 3, JPLabel},
    {0xf3, "DI", 1, DI},
    {0xf4, "CP", 3, CPLabel},
    {0xf5, "PUSH", 1, PUSHPSW},
    {0xf6, "ORI", 2, ORIData},
    {0xf7, "RST", 1, RST6},
    {0xf8, "RM", 1, RM},
    {0xf9, "SPHL", 1, SPHL},
    {0xfa, "JM", 3, JMLabel},
    {0xfb, "EI", 1, EI},
    {0xfc, "CM", 3, CMLabel},
    {},
    {0xfe, "CPI", 2, CPIData},
    {0xff, "RST", 1, RST7}
};