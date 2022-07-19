#pragma once

#include "Windows/Window.h"

#include <cstdint>

class Leds : public Window
{
public:
	static Leds* Instance;

	uint8_t ledValues = 0xff;

	bool _Saved = true;

public:
	void Init() override;
	void Open() override;
	void Close() override;
	void SimulationStart() override;
	void Render() override;
};