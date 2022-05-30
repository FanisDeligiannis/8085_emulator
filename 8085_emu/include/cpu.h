#pragma once

#include <chrono>
#include <thread>

#include "memory.h"
#include "stack.h"
#include "register.h"

#define SIGN_FLAG 7
#define ZERO_FLAG 6
#define AUX_CARRY_FLAG 4
#define PARITY_FLAG 2
#define CARRY_FLAG 0

#define CLOCK_SPEED 3200000
#define CLOCK_ACCURACY 60

class CPU
{
private:
	int _HangingCycles;
	bool _Running;
	bool _Halted;

public:
	static inline CPU* cpu;

	Memory* _Memory;
	Stack* _Stack;

	Register8* A;
	Register8* B;
	Register8* C;
	Register8* D;
	Register8* E;
	Register8* H;
	Register8* L;

	Register8* Flags;

	Register* PC;
	Register* SP;

public:

	CPU(Memory* memory);
	CPU(uint8_t* memory, size_t size);
	~CPU();

	std::thread Run();

	void Loop();

	void Clock();

	void SetFlags(uint8_t sign, uint8_t zero, uint8_t aux_c, uint8_t parity, uint8_t carry);

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

	inline Memory* GetMemory()
	{
		return _Memory;
	}

	inline uint8_t GetUnsignedM()
	{
		uint8_t H = cpu->H->GetUnsigned();
		uint8_t L = cpu->L->GetUnsigned();

		uint16_t addr = (H << 8) | L;
		uint8_t result = cpu->GetMemory()->GetDataAtAddr(addr);

		return result;
	}
	
	inline int8_t GetSignedM()
	{
		uint8_t H = cpu->H->GetSigned();
		uint8_t L = cpu->L->GetSigned();

		uint16_t addr = (H << 8) | L;
		uint8_t result = cpu->GetMemory()->GetDataAtAddr(addr);

		return *(int8_t*)&result;
	}

	inline uint8_t NextPC()
	{
		PC->Increment();
		uint8_t ret = _Memory->GetDataAtAddr(PC->Get());

		return ret;
	}

	inline uint8_t ReadPC()
	{
		return _Memory->GetDataAtAddr(PC->Get());
	}
};