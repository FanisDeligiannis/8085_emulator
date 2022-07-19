#pragma once

#include "Windows/Window.h"

#include "imgui.h"
#include "Simulation.h"
#include "ConfigIni.h"

class Switches : public Window
{
private:
	bool _Saved = true;

public:
	static Switches* Instance;

	bool SwitchesBuf[8];


public:
	void ToggleButton(const char* str_id, bool* v, float woffset = 0.0f);
	
	void Init() override;
	void Open() override;
	void Close() override;
	
	
	void SimulationStart() override;
	void Render() override;
};