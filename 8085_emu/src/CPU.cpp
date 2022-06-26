#include "cpu.h"

#include <cstdio>

#include "CPUinstructions.h"

CPU* CPU::cpu;


CPU::CPU(Memory* memory, std::vector<int> &breakpoints, std::vector<std::pair<uint16_t, int>> &symbols)
	: _Breakpoints(breakpoints)
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
	PC->Clear();
	SP = new Register();


	//SP is literally a pointer to the address of the stack.
	SP->SetRef(_Stack->GetSPPointer());

	//The following code is done because it's much faster to use a pointer array than a vector
	int maxLine = 0;

	for (int i = 0; i < symbols.size(); i++)
	{
		if (symbols.at(i).second > maxLine)
		{
			maxLine = symbols.at(i).second;
		}
	}

	_Symbols = (uint16_t*)calloc(maxLine + 1, sizeof(uint16_t));
	if (_Symbols != nullptr)
	{
		_SymbolSize = maxLine + 1;

		for (int i = 0; i < symbols.size(); i++)
		{
			_Symbols[symbols.at(i).second] = symbols.at(i).first;
		}
	}
	//Same with breakpoints
	UpdateBreakpoints();
}

CPU::CPU(uint8_t* memory, size_t size, std::vector<int>& breakpoints, std::vector<std::pair<uint16_t, int>> symbols)
	: CPU(new Memory(memory, size), breakpoints, symbols) {}

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

	free(_Symbols);

	cpu = nullptr;
}

void CPU::SetClock(int clock_speed, int accuracy)
{
	_ClockCyclesPerLoop = ((double)clock_speed) / ((double)accuracy);
}

bool CPU::Interrupts()
{
	//TODO: SHOULD INTERRUPTS BE DISABLED DURING OTHER INTERRUPT HANDLING?

	if (!_InterruptsEnabled)
	{
		return false;
	}

	//M = mask
	//IP = Interrupt Pending

	//If interrupts enabled, and if it's NOT masked, and if it's pending . . .

	if (!_M75 && _IP75) // Interrupt 7.5, highest priority
	{
		_IP75 = false;

		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

		CPU::cpu->PC->Set(0x003C);

		_InterruptsEnabled = false;

		return true;
	}
	
	if (!_M65 && _IP65) // Interrupt 6.5
	{
		_IP65 = false;

		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

		CPU::cpu->PC->Set(0x0034);
		
		_InterruptsEnabled = false;

		return true;
	}

	if (!_M55 && _IP55) // Interrupt 5.5
	{
		_IP55 = false;

		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

		CPU::cpu->PC->Set(0x002C);

		_InterruptsEnabled = false;

		return true;
	}


	if (_IPINTR) // Interrupt INTR, lowest priority
	{
		if (INTR_ADDR != 0)
		{
			_IPINTR = false;

			CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
			CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

			CPU::cpu->PC->Set(INTR_ADDR+1);

			_InterruptsEnabled = false;

			return true;
		}
	}

	return false;
}

//In my program, a Loop is something that happens as many times as "CLOCK_ACCURACY" describes per second.
//"CLOCK" is something that happens as many times as "CLOCK_SPEED" says per second. _HangingCycles are clock cycles that are skipped.

// Since we are dividing our clock cycles into loops, we will have CLOCK_SPEED/CLOCK_ACURACY clocks per loop. Thus, _ClockCyclesPerLoop

//We leave the "Sleeping" to the simulator, probably should be done here though?

void CPU::Loop()
{
	_Running = true;
	_Halted = false;

	while (_Running && !_Halted && _CurrentCycles <= _ClockCyclesPerLoop) //If we're running and if we STILL have cycles in our loop.
	{
		_CurrentCycles += _HangingCycles; //We completely skip _HangingCycles

		_HangingCycles = 0; // And then make them 0.

		Interrupts(); // Check for interrupts.

		Clock(); //Clock.

		_CurrentCycles++; //Increment clock cycles.
	}

	_CurrentCycles = 0; //Reset _CurrentCycles too 0.
	//"_CurrentCycles" counts cycles per loop.
	//Max is equal to _ClockCyclesPerLoop
}

bool alreadyHalted = false;

void CPU::Clock()
{
	if (!_Halted && !alreadyHalted) //We don't want to halt on the same line twice. We want to continue.
	{
		int currentLine = 0; //Search for the line that corresponds to the current opcode

		uint16_t currentAddr = PC->Get();

		for (int i = 0; i < _BreakpointsArrSize; i++) // Search the Breakpoint Array.
		{
			if (_BreakpointsArr[i] == currentAddr) // If the currentAddr is in there, break.
			{
				_Halted = true;
				alreadyHalted = true;
				return;
			}
		}
	}

	alreadyHalted = false;

	uint8_t op = _Memory->GetDataAtAddr(PC->Get()); //Get opcode.

	CPUInstruction instr = CPUInstructions[op]; //CPUInstructions is sorted with OPCODE, so we just get it using [op]

	_HangingCycles = instr.ACTION(instr.bytes);
	
	PC->Increment();
}



void CPU::Step(std::vector<std::pair<uint16_t, int>> Symbols)
{	
	_CurrentCycles = 0;

	bool first = true;
	bool hasSymbol = false;

	while (((!hasSymbol && _CurrentCycles < _ClockCyclesPerLoop) || first) && GetRunning())
	{
		first = false;
		hasSymbol = false;

		Interrupts();
		Clock();
		
		_CurrentCycles += _HangingCycles + 1;
	
		for (int i = 0; i < Symbols.size(); i++)
		{
			if (Symbols.at(i).first == PC->Get())
			{
				hasSymbol = true;
				break;
			}
		}

	}
}


//Set all flags accordingly. If it's -1, don't affect it.
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

void CPU::UpdateBreakpoints()
{
	if (_BreakpointsArr == nullptr)
	{
		free(_BreakpointsArr);
	}

	_BreakpointsArr = (uint16_t*)calloc(_Breakpoints.size(), sizeof(uint16_t));
	_BreakpointsArrSize = _Breakpoints.size();

	if (_BreakpointsArr == nullptr)
	{
		//exit(1); //TODO: Better error handling
		return;
	}

	for (int i = 0; i < _Breakpoints.size(); i++) // We convert from Breakpoint Line to Memory Address.
	{
		for (int j = 0; j < _SymbolSize; j++)
		{
			if (_Breakpoints.at(i) == j)
			{
				_BreakpointsArr[i] = _Symbols[j];
			}
		}
	}
}

int CPU::GetInstructionBytes()
{
	CPUInstruction inst = CPUInstructions[ReadPC()];
	return inst.bytes;
}
