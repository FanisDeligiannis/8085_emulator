#pragma once

#include <csignal>

#include "Simulation.h"

#include "toot.h"
#include "toot.c"

namespace Beep8085
{
	//Beep duration in MS will be IO 0x60-0x61, for 16 bits of duration (max 0xffff)
	//Beep Frequency will be IO 0x62-0x63, for a range of 0x0000-0xffff, but effectively max freq is 20khz
	//You can just "CALL BEEP" and it'll beep for 1023 ms.
	//OR you can "CALL BEEPFD", where BC will be frequency, and DE will be duration.

	// I need to find a better solution because right now I'm not able to stop this when stopping simulation.

	uint16_t* Duration = nullptr;
	uint16_t* Frequency = nullptr;

	bool threadActive = false;
	bool threadDone = false;

	std::thread t;

	void SimulationStart()
	{
		Duration = (uint16_t*)Simulation::cpu->GetIO()->GetDataAtAddrPointer(0x60);
		Frequency = (uint16_t*) Simulation::cpu->GetIO()->GetDataAtAddrPointer(0x62);
	}

	void thread()
	{
		threadActive = true;
		toot(*Frequency, *Duration);
		*Duration = 0;
		threadActive = false;
		threadDone = true;
	}

	void Render()
	{
		if (Simulation::GetRunning() && Duration != nullptr)
		{
			if (*Duration > 0 && !threadActive)
			{
				if (t.joinable())
				{
					t.join();
				}

				t = std::thread(thread);
			}
		}
		else if(Duration != nullptr)
		{
			
			*Duration = 0;
		}

		if (threadDone && t.joinable())
		{
			t.join();
			threadDone = false;
		}
	}
}