#pragma once

#include "Windows/Window.h"

#include <cstdint>
#include <thread>

class Beep8085 : public Window
{
	//Beep duration in MS will be IO 0x60-0x61, for 16 bits of duration (max 0xffff)
	//Beep Frequency will be IO 0x62-0x63, for a range of 0x0000-0xffff, but effectively max freq is 20khz
	//You can just "CALL BEEP" and it'll beep for 1023 ms.
	//OR you can "CALL BEEPFD", where BC will be frequency, and DE will be duration.

	// I need to find a better solution because right now I'm not able to stop this when stopping simulation.

public:
	static Beep8085* Instance;

	uint16_t Duration = 0;
	uint16_t Frequency = 0;

	bool set0, set1, set2, set3;

	bool threadActive = false;
	bool threadDone = false;

	std::thread t;

public:
	void Init() override { Instance = this; }

	void SimulationStart() override;
	void Render() override;
};