#pragma once

#include "Windows/Window.h"

#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"
#include <ConfigIni.h>
#include <Simulation.h>

class RegistersBinary : public Window
{
private:
	
public:
	bool _Saved = true;

	void Init() override
	{
		IncludeInWindows = true;
		Name = "Registers(Binary)";

		_Open = ConfigIni::GetInt("Registers(Binary)", "Open", 0);
		_Saved = _Open;
	}

	void Open() override
	{
		_Open = true;
		ConfigIni::SetInt("Registers(Binary)", "Open", 1);
	}

	void Close() override
	{
		if (!_Open && _Open == _Saved)
			return;

		_Open = false;
		_Saved = false;
		ConfigIni::SetInt("Registers(Binary)", "Open", 0);
	}

	void Render() override;
};