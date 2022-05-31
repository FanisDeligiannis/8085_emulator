#include "Simulation.h"

#include "assembler.h"

#include "RegistersWindow.h"

namespace Simulation {
	CPU* cpu;
	uint8_t* memory_data;
	std::thread t;

	bool Paused = false;
	bool _ScheduledStep = false;
	bool _Stepping = false;

	std::vector<std::pair<int, std::string>> Errors;


	void Assemble(std::string text)
	{
		memory_data = Assembler::GetAssembledMemory(text);
		Errors = Assembler::GetErrors();
	}

	void Run()
	{
		if (Errors.size() > 0)
		{
			//TODO: Fix errors popup
			return;
		}

		if (cpu != nullptr && cpu->GetRunning() && (Paused || cpu->GetHalted() || _Stepping))
		{
			cpu->SetRunning(true);
			cpu->SetHalted(false);
			_Stepping = false;
			Paused = false;
		}
		else if (cpu == nullptr || !cpu->GetRunning())
		{
			if (cpu != nullptr)
			{
				delete cpu;
				cpu = nullptr;
			}

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
		if (cpu != nullptr)
		{
			cpu->SetRunning(false);
			cpu->SetHalted(false);
			Paused = false;
		}

		if (t.joinable())
		{
			t.join();
			//t.detach();
		}
	}

	void Pause()
	{
		if (cpu != nullptr && cpu->GetRunning())
		{
			cpu->SetHalted(true);
			Paused = true;
		}
	}

	void Step()
	{
		_Stepping = true;
		if (cpu == nullptr || !GetRunning())
		{
			Run();
		}
		else
		{
			_ScheduledStep = true;
		}
	}

	void Init()
	{
		memory_data = (uint8_t*)calloc(0xffff, sizeof(uint8_t));
	}

	void thread()
	{
		cpu = new CPU(memory_data, 0xffff);

		typedef std::chrono::milliseconds ms;
		typedef std::chrono::duration<float> fsec;

		auto _StartOfFrame = std::chrono::system_clock::now();

		cpu->SetRunning(true);
		cpu->SetHalted(false);
		Paused = false;

		while (cpu->GetRunning())
		{
			if (cpu->GetRunning() && !cpu->GetHalted() && !Paused && !_Stepping)
			{
				try
				{
					cpu->Loop();
				}
				catch(...)
				{
					printf("CPU simulation crashed!\n");
					cpu->SetRunning(false);
					break;
				}
			}
			
			if (cpu->GetRunning() && _Stepping && _ScheduledStep)
			{
				try
				{
					cpu->Clock();
					_ScheduledStep = false;
				}
				catch (...)
				{
					printf("CPU simulation crashed!\n");
					cpu->SetRunning(false);
					break;
				}
			}

			
			_StartOfFrame += std::chrono::microseconds(1000000 / CLOCK_ACCURACY);
			std::this_thread::sleep_until(_StartOfFrame);
		}

		RegistersWindow::UpdateBuffers(true);

		delete cpu;
		cpu = nullptr;
		Paused = false;
	}
}