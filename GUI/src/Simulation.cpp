#include "Simulation.h"

#include "assembler.h"

namespace Simulation {
	CPU* cpu;
	uint8_t* memory_data;
	std::thread t;

	bool Running = false;
	bool Paused = false;
	
	std::vector<std::pair<int, std::string>> Errors;


	void Assemble(std::string text)
	{
		if (memory_data != nullptr)
			free(memory_data);
		memory_data = nullptr;
		memory_data = Assembler::GetAssembledMemory(text);
		Errors = Assembler::GetErrors();
	}

	void Run()
	{
		if (Errors.size() > 0)
		{
			return;
		}

		if (Running && Paused)
		{
			if (cpu != nullptr)
			{
				cpu->SetRunning(true);
				return;
			}

			Running = false;
			Paused = false;
		}
		
		if (!Running)
		{
			if (t.joinable())
			{
				t.join();
				//t.detach();
			}
			t = std::thread(&thread);
		}
	}

	void Stop()
	{
		if (Running)
		{
			Running = false;
			Paused = false;

			if (cpu != nullptr)
			{
				cpu->SetRunning(false);
				cpu->SetHalted(false);
			}

			if (t.joinable())
			{
				t.join();
				//t.detach();
			}
		}
	}

	void Pause()
	{
		if (cpu != nullptr && Running)
		{
			cpu->SetRunning(false);
			Paused = true;
		}
	}

	void Init()
	{
		memory_data = (uint8_t*)calloc(0xffff, sizeof(uint8_t));
	}

	void thread()
	{
		cpu = new CPU(memory_data, 0xffff);

		Running = true;
		Paused = false;

		while (Running)
		{
			cpu->Loop();
			
			while (!cpu->GetRunning() && cpu->GetHalted() && Running)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		delete cpu;
	}
}