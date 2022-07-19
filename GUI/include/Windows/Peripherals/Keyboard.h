#pragma once

#include "Windows/Window.h"

#include "imgui.h"
#include "Simulation.h"
#include "ConfigIni.h"

class Keyboard : public Window
{
public:
	static Keyboard* Instance;

	uint8_t Scan;
	
	int8_t lastButton = -1;

	bool _Saved = true;

public:
	void Init() override;
	void Open() override;
	void Close() override;
	void SimulationStart() override;
	void Render() override;
};