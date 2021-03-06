#pragma once

#include <chrono>
#include <thread>
#include <vector>
#include <memory>

#include "memory.h"
#include "stack.h"
#include "register.h"
#include "IO_cb.h"

//Flag bits

#define SIGN_FLAG 7
#define ZERO_FLAG 6
#define AUX_CARRY_FLAG 4
#define PARITY_FLAG 2
#define CARRY_FLAG 0

namespace Emulator
{
	enum ErrorCodes
	{
		None = 0,
		EmptyStackPop = 1
	};

	//Clock speed it 3.2mhz
	//We divide that in CLOCK_ACCURACY steps.
	//More explanation in .cpp file.

	class CPU
	{
	private:
		//Each instruction takes multiple cycles in the real 8085.
		//Here, we execute them all in one or in a few lines of code.
		//So, for us it's only "one cycle". So we will do absolutely nothing for the next few cycles.
		//how many, depending on the instruction.
		int _HangingCycles;
		bool _Running;
		bool _Halted;

		std::vector<IOCallback> IOInterface;
	public:
		static CPU* cpu;

		std::vector<int>& _Breakpoints;
		std::shared_ptr<uint16_t> _BreakpointsArr;
		size_t _BreakpointsArrSize;
		std::shared_ptr<uint16_t> _Symbols;
		size_t _SymbolSize;

		ErrorCodes ErrorCode = None;


		std::shared_ptr<Memory> _Memory;
		std::shared_ptr<InternalEmulator::Stack> _Stack;

		std::shared_ptr <Register8> A;
		std::shared_ptr <Register8> B;
		std::shared_ptr <Register8> C;
		std::shared_ptr <Register8> D;
		std::shared_ptr <Register8> E;
		std::shared_ptr <Register8> H;
		std::shared_ptr <Register8> L;

		std::shared_ptr <Register8> Flags;

		std::shared_ptr <Register> PC;
		std::shared_ptr <Register> SP;

	public:

		bool _InterruptsEnabled = false;

		bool _M55 = true;
		bool _M65 = true;
		bool _M75 = true;

		bool _IP55 = false;
		bool _IP65 = false;
		bool _IP75 = false;

		bool _IPINTR = false;
		uint16_t INTR_ADDR = 0;

		double _ClockCyclesPerLoop = 0;
		long long _CurrentCycles = 0;

	public:

		CPU(std::shared_ptr<Memory> memory, std::vector<int>& breakpoints, std::vector<std::pair<uint16_t, int>>& symbols);
		CPU(std::shared_ptr<uint8_t> memory, size_t size, std::vector<int>& breakpoints, std::vector<std::pair<uint16_t, int>> symbols);
		
		void SetClock(int clock_speed, int accuracy);

		bool Interrupts();

		void Loop();

		void Clock();

		void Step(std::vector<std::pair<uint16_t, int>> Symbols);

		void SetFlags(uint8_t sign, uint8_t zero, uint8_t aux_c, uint8_t parity, uint8_t carry);

		void UpdateBreakpoints();

		int GetInstructionBytes();

		inline void SetRunning(bool running)
		{
			_Running = running;
		}

		inline bool GetRunning()
		{
			return _Running;
		}

		inline void SetHalted(bool halted)
		{
			_Halted = halted;
		}

		inline bool GetHalted()
		{
			return _Halted;
		}

		inline std::shared_ptr<Memory> GetMemory()
		{
			return _Memory;
		}


		//Read memory at location pointed by H,L. Return unsigned.
		inline uint8_t GetUnsignedM()
		{
			uint8_t H = cpu->H->GetUnsigned();
			uint8_t L = cpu->L->GetUnsigned();

			uint16_t addr = (H << 8) | L;
			uint8_t result = cpu->GetMemory()->GetDataAtAddr(addr);

			return result;
		}

		//Read memory at location pointed by H,L. Return signed
		inline int8_t GetSignedM()
		{
			uint8_t H = cpu->H->GetSigned();
			uint8_t L = cpu->L->GetSigned();

			uint16_t addr = (H << 8) | L;
			uint8_t result = cpu->GetMemory()->GetDataAtAddr(addr);

			return *(int8_t*)&result;
		}

		//Increment PC and then read the data at that address.
		inline uint8_t NextPC()
		{
			PC->Increment();
			uint8_t ret = _Memory->GetDataAtAddr(PC->Get());

			return ret;
		}

		//Read current PC without incrementing.
		inline uint8_t ReadPC()
		{
			return _Memory->GetDataAtAddr(PC->Get());
		}

		inline void AddIOInterface(uint16_t addr, void(*OUTPUT)(uint8_t out), uint8_t(*INPUT)())
		{
			IOInterface.push_back({ addr, OUTPUT, INPUT });
		}

		inline std::vector<IOCallback> GetIOInterface() { return IOInterface; }
	};

}