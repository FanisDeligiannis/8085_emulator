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

	uint16_t Duration = 0;
	uint16_t Frequency = 0;

	bool set0, set1, set2, set3;

	bool threadActive = false;
	bool threadDone = false;

	std::thread t;

	void Duration0(uint8_t val)
	{
		set0 = true;
		Duration = (Duration & 0xff00) | val;
	}

	void Duration1(uint8_t val)
	{
		set1 = true;
		Duration = (Duration & 0x00ff) | (val << 8);
	}

	uint8_t DurationOut0()
	{
		return (uint8_t)Duration;
	}

	uint8_t DurationOut1()
	{
		return (uint8_t)(Duration >> 8);
	}

	void Frequency0(uint8_t val)
	{
		set2 = true;
		Frequency = (Frequency & 0xff00) | val;
	}
	void Frequency1(uint8_t val)
	{
		set3 = true;
		Frequency = (Frequency & 0x00ff) | (val << 8);
	}


	void SimulationStart()
	{
		Duration = 0;

		set0 = false;
		set1 = false;
		set2 = false;
		set3 = false;

		Simulation::cpu->AddIOInterface(0x60, Duration0, DurationOut0);
		Simulation::cpu->AddIOInterface(0x61, Duration1, DurationOut1);
		Simulation::cpu->AddIOInterface(0x62, Frequency0, nullptr);
		Simulation::cpu->AddIOInterface(0x63, Frequency1, nullptr);

	}

	void thread()
	{
		threadActive = true;
		toot(Frequency, Duration);
		Duration = 0;
		threadActive = false;
		threadDone = true;
	}

	void Update()
	{
		if (Simulation::GetRunning() && set0 && set1 && set2 && set3)
		{
			set0 = false;
			set1 = false;
			set2 = false;
			set3 = false;

			if (Duration > 0 && !threadActive)
			{
				if (t.joinable())
				{
					t.join();
				}

				t = std::thread(thread);
			}
		}

		if (threadDone && t.joinable())
		{
			t.join();
			threadDone = false;
		}
	}
}