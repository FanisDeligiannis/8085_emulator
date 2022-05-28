#pragma once

#include <cstdint>
#include <thread>
#include <vector>

#include "cpu.h"

namespace Simulation {
	extern CPU* cpu;
	extern uint8_t* memory_data;
	extern std::thread t;

	extern bool Running;
	extern bool Paused;
	
	extern std::vector<std::pair<int, std::string>> Errors;


	void Assemble(std::string text);

	void Run();
	void Stop();
	void Pause();

	void Init();
	void thread();
}