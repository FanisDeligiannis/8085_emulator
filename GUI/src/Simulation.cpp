#include "Simulation.h"

#include <iostream>

#include "Application.h"
#include "ConfigIni.h"
#include "Windows/CodeEditor.h"
#include "Windows/RegistersWindow.h"

namespace Simulation {
	CPU* cpu;
	std::thread t;

	Assembler::Assembly program;

	bool Paused = false;
	bool _ScheduledStep = false;
	bool _Stepping = false;

	int CPU_Speed;
	int CPU_Accuracy;

	void Assemble(std::string text)
	{
		Assembler::Assembly result;
		Assembler::GetAssembledMemory(text, result);
		program = result;
	}

	void Run(bool stepping)
	{
		if (program.Errors.size() > 0)
		{
			//TODO: Have a popup or something.
			return;
		}

		//If it's already running but pause/halted or if it's in stepping mode, start it again normally.
		if (cpu != nullptr && cpu->GetRunning() && (Paused || cpu->GetHalted() || _Stepping))
		{
			cpu->SetRunning(true);
			cpu->SetHalted(false);
			_Stepping = false;
			Paused = false;
		}
		else if (cpu == nullptr || !cpu->GetRunning())
		{
			//If it's not running, create a new thread and run it there.
			if (cpu != nullptr)
			{
				delete cpu;
				cpu = nullptr;
			}

			if (t.joinable()) //Make sure that if there's an old thread hanging around, it exits.
			{
				t.join();
				//t.detach();
			}

			_Stepping = stepping;

			t = std::thread(&thread);
		}
	}

	void SetClock(int clock_speed, int accuracy)
	{
		CPU_Speed = clock_speed;
		CPU_Accuracy = accuracy;

		ConfigIni::SetInt("Simulation", "CPU_Speed", clock_speed);
		ConfigIni::SetInt("Simulation", "CPU_Accuracy", accuracy);

		if (GetRunning() && cpu != nullptr)
		{
			cpu->SetClock(CPU_Speed, CPU_Accuracy);
		}
	}

	int GetClock() { return CPU_Speed; }
	int GetAccuracy() { return CPU_Accuracy; }

	void Stop()
	{
		if (cpu != nullptr)
		{
			cpu->SetRunning(false);
			cpu->SetHalted(false);
			Paused = false;
			_Stepping = false;
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
			Run(true);
		}
		else
		{
			_ScheduledStep = true; // Step isn't instant. It's buffered.
		}
	}

	void Init()
	{
		program.Memory = (uint8_t*)calloc(0xffff, sizeof(uint8_t));
		CPU_Speed = ConfigIni::GetInt("Simulation", "CPU_Speed", 3200000);
		CPU_Accuracy = ConfigIni::GetInt("Simulation", "CPU_Accuracy", 500);
	}

	bool HasSymbols(Assembler::Assembly program, uint16_t addr)
	{
		for (int i = 0; i < program.Symbols.size(); i++)
		{
			if (program.Symbols.at(i).first == addr)
			{
				return true;
			}
		}

		return false;
	}

	void thread()
	{
		//Create CPU.
		cpu = new CPU(program.Memory, 0xffff, CodeEditor::editor._Breakpoints, program.Symbols);

		cpu->SetClock(CPU_Speed, CPU_Accuracy);

		Application::SimulationStart();

		auto _StartOfFrame = std::chrono::system_clock::now();

		//----- Set the INTR_ADDR to the address of the label "INTR_ROUTINE"
		//maybe find a better way?
		auto &labels = program.Labels;

		for (int i = 0; i < labels.size(); i++)
		{
			if (labels.at(i).first == "INTR_ROUTINE")
			{
				cpu->INTR_ADDR = labels.at(i).second;
			}
		}
		//-----

		cpu->SetRunning(true);
		cpu->SetHalted(false);
		Paused = false;

		while (cpu->GetRunning())
		{
			if (cpu->GetRunning() && !cpu->GetHalted() && !Paused && !_Stepping)
			{
				try // Loop inside try / catch in cases of errors, crashes.
				{
					cpu->Loop();
				}
				catch(...)
				{
#ifdef _DEBUG
					printf("CPU simulation crashed!\n");
#endif
					cpu->SetRunning(false);
					break;
				}
			}
			
			if (GetRunning() && _Stepping && _ScheduledStep) // If a step is scheduled, run a clock cycle.
			{
				try
				{
					// But if we're at PC < 0x0800, we're at a predefined routine.
					// So we keep clocking until we're out of there.
					// Probably should add an option to enable/disable that. TODO

					// BUT. If we have symbols on that address, it means it's user code, added using "ORG". So we don't skip it. 
					while (_ScheduledStep || (_Stepping && GetRunning() && !HasSymbols(program, cpu->PC->Get())))
					{
						cpu->Step(program.Symbols);
						_ScheduledStep = false;

						_StartOfFrame += std::chrono::microseconds(1000000 / CPU_Accuracy);
						std::this_thread::sleep_until(_StartOfFrame);
					}
				}
				catch (...)
				{
#ifdef _DEBUG
					printf("CPU simulation crashed!\n");
#endif
					cpu->SetRunning(false);
					break;
				}
			}

			
			// Check for interrupts, even it we're halted but not when paused.

			if (!Paused && cpu->Interrupts())
			{
				if (!_Stepping)
				{
					cpu->SetHalted(false); //Get out of halt state.
				}
			}

			//Sleep until appropriate times has passed since START OF FRAME. 
			//Not from now. This accounts for the time it takes for the clock/loop to run.
			_StartOfFrame += std::chrono::microseconds(1000000 / CPU_Accuracy);
			std::this_thread::sleep_until(_StartOfFrame);
		}
		
		//Update buffers before deleting CPU.
		RegistersWindow::UpdateBuffers(true);

		delete cpu;
		cpu = nullptr;
		Paused = false;
	}
}