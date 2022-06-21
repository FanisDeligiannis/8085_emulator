#pragma once

#include <string>
#include <utility>
#include <cstdint>
#include <thread>
#include <vector>

#include "cpu.h"
#include "assembler.h"

namespace Simulation {
	extern CPU* cpu;
	extern std::thread t;

	extern bool Paused;
	extern bool _Stepping;

	extern IOchip* _IOchip;

	extern Assembler::Assembly program;

	void Assemble(std::string text);

	void SetClock(int clock_speed, int accuracy);
	int GetClock();
	int GetAccuracy();
	
	void Run(bool stepping = false);
	void Stop();
	void Pause();
	void Step();

	inline bool GetRunning() {
		if (cpu == nullptr)
			return false;
		return cpu->GetRunning();
	}
	inline bool GetPaused() { return Paused; }

	void Init();
	void thread();
}