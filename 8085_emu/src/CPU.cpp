#include "cpu.h"

#include <stdio.h>

#include "CPUinstructions.h"

CPU::CPU(Memory* memory, IOchip* io, std::vector<int> &breakpoints, std::vector<std::pair<uint16_t, int>> &symbols)
	: _Breakpoints(breakpoints), _Symbols(symbols)
{
	cpu = this;

	_Running = true;
	_Halted = false;
	_HangingCycles = 0;
	_Memory = memory;
	_IOchip = io;

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
}

CPU::CPU(uint8_t* memory, size_t size, IOchip* io, std::vector<int>& breakpoints, std::vector<std::pair<uint16_t, int>> symbols)
	: _Breakpoints(breakpoints), _Symbols(symbols)
{
	cpu = this;

	_Running = true;
	_Halted = false;
	_HangingCycles = 0;
	
	_Memory = new Memory(memory, size);

	_IOchip = io;

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
	//Start a thread.
	//Deprecated, not really used now.
	std::thread f(&CPU::Loop, this);

	return f;
}

bool CPU::Interrupts()
{
	//SHOULD INTERRUPTS BE DISABLED DURING OTHER INTERRUPT HANDLING?


	//M = mask
	//IP = Interrupt Pending

	//If interrupts enabled, and if it's NOT masked, and if it's pending . . .

	if (_InterruptsEnabled && !_M55 && _IP55) // Interrupt 5.5
	{
		_IP55 = false;

		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

		CPU::cpu->PC->Set(0x002C);
		
		return true;
	}

	if (_InterruptsEnabled && !_M65 && _IP65) // Interrupt 6.5
	{
		_IP65 = false;

		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

		CPU::cpu->PC->Set(0x0034);

		return true;
	}

	if (_InterruptsEnabled && !_M75 && _IP75) // Interrupt 7.5
	{
		_IP75 = false;

		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
		CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

		CPU::cpu->PC->Set(0x003C);

		return true;
	}

	if (_InterruptsEnabled && _IPINTR)
	{
		if (INTR_ADDR != 0)
		{
			_IPINTR = false;

			CPU::cpu->_Stack->Push(CPU::cpu->PC->GetHigh());
			CPU::cpu->_Stack->Push(CPU::cpu->PC->GetLow());

			CPU::cpu->PC->Set(INTR_ADDR+1);

			return true;
		}
	}

	return false;
}

//Probably should not be static and decided upon compile. Maybe an option for slower computers?
auto _ClockCyclesPerLoop = ((double)CLOCK_SPEED) / ((double)CLOCK_ACCURACY);
long long _CurrentCycles = 0;

//In my program, a Loop is something that happens as many times as "CLOCK_ACCURACY" describes. By default 60 times a seocond.
//"CLOCK" is something that happens as many times as "CLOCK_SPEED" says. _HangingCycles are clock cycles that are skipped, though.

// Since we are dividing our clock cycles into loops, we will have CLOCK_SPEED/CLOCK_ACURACY clocks per loop. Thus, _ClockCyclesPerLoop

//We leave the "Sleeping" to the simulator, probably should be done here though.

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

		for (int i = 0; i < _Symbols.size(); i++)
		{
			if (_Symbols.at(i).first == PC->Get())
			{
				currentLine = _Symbols.at(i).second;
			}
		}
		
		//Search the breakpoints to see if that line has a breakpoint set.
		for (int i = 0; i < _Breakpoints.size(); i++)
		{
			if (_Breakpoints.at(i) == currentLine)
			{
				//If yes, halt the CPU and cancel the clock
				_Halted = true;
				alreadyHalted = true;
				return;
			}
		}
	}

	alreadyHalted = false;

	uint8_t op = _Memory->GetDataAtAddr(PC->Get()); //Get opcode.

	CPUInstruction instr = CPUInstructions[op]; //CPUInstructions is sorted with OPCODE, so we just get it using [op]

	////TEMPORARY.
	////0 SHOULDN'T BE AN ERROR, IT SHOULD JUST BE 'NOP'
	////It's easier to find bugs this way. Just don't use 'NOP' yet.
	////Should be removed soon.
	//if (instr.OPCODE != 0)
	//{
		_HangingCycles = instr.ACTION(instr.bytes);
	//}
	//else
	//{
	//	printf("Corrupted data OR bug in the code. . .\n");
	//	_Running = false;
	//	_Halted = true;
	//	return;
	//}

	
	PC->Increment();
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
