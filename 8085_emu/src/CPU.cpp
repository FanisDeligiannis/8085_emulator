#include "cpu.h"

#include <stdio.h>
#include <chrono>


CPU::CPU()
{
	cpu = this;

	_Running = true;
	_HangingCycles = 0;
	_Memory = new Memory(16);
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
	SP = new Register();

	SP->SetRef(_Stack->GetSPPointer());

	Loop();
}

void CPU::Loop()
{
	_PrevClockTime = std::chrono::high_resolution_clock::now();
	while (_Running)
	{
		auto now = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> t = std::chrono::duration_cast<std::chrono::duration<double>>(now - _PrevClockTime);

		if (t.count() >= _TimeBetweenClockCycles)
		{
			_PrevClockTime = now;
			if (_HangingCycles)
			{
				_HangingCycles--;
				return;
			}
			Clock();
		}
	}
}

void CPU::Clock()
{

}