#include "Windows/Peripherals/Beep.h"

#include "Simulation.h"

#include "toot.h"
#include "toot.c"

Beep8085* Beep8085::Instance;

void Duration0(uint8_t val)
{
	Beep8085* inst = Beep8085::Instance;
	inst->set0 = true;
	inst->Duration = (inst->Duration & 0xff00) | val;
}

void Duration1(uint8_t val)
{
	Beep8085* inst = Beep8085::Instance;
	inst->set1 = true;
	inst->Duration = (inst->Duration & 0x00ff) | (val << 8);
}

uint8_t DurationOut0()
{
	Beep8085* inst = Beep8085::Instance;
	return (uint8_t)inst->Duration;
}

uint8_t DurationOut1()
{
	Beep8085* inst = Beep8085::Instance;
	return (uint8_t)(inst->Duration >> 8);
}

void Frequency0(uint8_t val)
{
	Beep8085* inst = Beep8085::Instance;
	inst->set2 = true;
	inst->Frequency = (inst->Frequency & 0xff00) | val;
}
void Frequency1(uint8_t val)
{
	Beep8085* inst = Beep8085::Instance;
	inst->set3 = true;
	inst->Frequency = (inst->Frequency & 0x00ff) | (val << 8);
}

void thread()
{
	Beep8085* inst = Beep8085::Instance;

	inst->threadActive = true;
	toot(inst->Frequency, inst->Duration);
	inst->Duration = 0;
	inst->threadActive = false;
	inst->threadDone = true;
}

//Beep duration in MS will be IO 0x60-0x61, for 16 bits of duration (max 0xffff)
//Beep Frequency will be IO 0x62-0x63, for a range of 0x0000-0xffff, but effectively max freq is 20khz
//You can just "CALL BEEP" and it'll beep for 1023 ms.
//OR you can "CALL BEEPFD", where BC will be frequency, and DE will be duration.

// I need to find a better solution because right now I'm not able to stop this when stopping simulation.


void Beep8085::SimulationStart() 
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

void Beep8085::Update() 
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