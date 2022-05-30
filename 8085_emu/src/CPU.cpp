#include "cpu.h"

#include <stdio.h>

#include "CPUinstructions.h"

CPU::CPU(Memory* memory)
{
	cpu = this;

	_Running = true;
	_Halted = false;
	_HangingCycles = 0;
	_Memory = memory;
	_Stack = new Stack(16);
	_Stack->SetDataPointer(_Memory->GetData());

	A = new Register8();
	B = new Register8();
	C = new Register8();
	D = new Register8();
	E = new Register8();
	H = new Register8();
	L = new Register8();

	Flags = new Register8();

	PC = new Register();
	PC->Set(0x0800);
	SP = new Register();

	SP->SetRef(_Stack->GetSPPointer());
}

CPU::CPU(uint8_t* memory, size_t size)
{
	cpu = this;

	_Running = true;
	_Halted = false;
	_HangingCycles = 0;
	
	_Memory = new Memory(memory, size);

	_Stack = new Stack(16);
	_Stack->SetDataPointer(_Memory->GetData());

	A = new Register8();
	B = new Register8();
	C = new Register8();
	D = new Register8();
	E = new Register8();
	H = new Register8();
	L = new Register8();

	Flags = new Register8();

	PC = new Register();
	PC->Set(0x0800);
	SP = new Register();

	SP->SetRef(_Stack->GetSPPointer());
}

CPU::~CPU()
{
	delete A;
	delete B;
	delete C;
	delete D;
	delete E;
	delete H;
	delete L;
	delete PC;
	delete SP;
	delete _Stack;
	delete _Memory;

	cpu = nullptr;
}


std::thread CPU::Run()
{
	std::thread f(&CPU::Loop, this);

	return f;
}

auto _ClockCyclesPerLoop = ((double)CLOCK_SPEED) / ((double)CLOCK_ACCURACY);
long long _CurrentCycles = 0;

void CPU::Loop()
{
	_Running = true;
	_Halted = false;

	while (_Running && !_Halted && _CurrentCycles <= _ClockCyclesPerLoop)
	{
		_CurrentCycles += _HangingCycles;

		_HangingCycles = 0;

		Clock();

		_CurrentCycles++;
	}

	_CurrentCycles = 0;
}

void CPU::Clock()
{
	uint8_t op = _Memory->GetDataAtAddr(PC->Get());

	CPUInstruction instr = CPUInstructions[op];

	if (instr.OPCODE != 0)
	{
		_HangingCycles = instr.ACTION(instr.bytes);
	}
	else
	{
		printf("Corrupted data OR bug in the code. . .\n");
		_Running = false;
		_Halted = true;
		return;
	}

	
	PC->Increment();
}

void CPU::SetFlags(uint8_t sign, uint8_t zero, uint8_t aux_c, uint8_t parity, uint8_t carry)
{
	Flags->SetUnsigned(
		((sign != -1) ? (sign & 1) << 7 : Flags->GetBit(SIGN_FLAG)) |
		((zero != -1) ? (zero & 1) << 6 : Flags->GetBit(ZERO_FLAG)) |
		((aux_c != -1) ? (aux_c & 1) << 4 : Flags->GetBit(AUX_CARRY_FLAG)) |
		((parity != -1) ? (parity & 1) << 2 : Flags->GetBit(PARITY_FLAG)) |
		((carry != -1) ? (carry & 1) << 0 : Flags->GetBit(CARRY_FLAG))
	);
}
