#pragma once

#include "Windows/Window.h"

#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

class HexEditor : public Window
{
private:
	MemoryEditor _HexEditor;
	uint16_t prevPC = 0;

	uint16_t start = 0x0800;
	uint16_t end = 0xffff;

public:
	bool _Saved = true;

	void Init() override
	{
		IncludeInWindows = true;
		Name = "Hex Editor";

		_HexEditor.HighlightColor = 0xff707000; // IM_COL32(255, 0, 0, 255);

		_Open = ConfigIni::GetInt("HexEditor", "Open", 1);
		_Saved = _Open;
	}

	void Open() override
	{
		_Open = true;
		ConfigIni::SetInt("HexEditor", "Open", 1);
	}

	void Close() override
	{
		if (!_Open && _Open == _Saved)
			return;

		_Open = false;
		_Saved = false;
		ConfigIni::SetInt("HexEditor", "Open", 0);
	}

	void Render() override
	{
		if (!_Open)
		{
			Close();
			return;
		}

		ImGui::Begin("Hex", &_Open);
		if (Simulation::program.Memory.get() != nullptr)
		{
			if (Simulation::GetRunning() && (Simulation::_Stepping || Simulation::GetPaused()))
			{
				if (prevPC != Simulation::cpu->PC->Get() && Simulation::cpu->PC->Get() >= start)
				{
					_HexEditor.HighlightMin = Simulation::cpu->PC->Get() - start;
					_HexEditor.HighlightMax = Simulation::cpu->PC->Get() + Simulation::cpu->GetInstructionBytes() - start;
					prevPC = Simulation::cpu->PC->Get();
				}
				else if (Simulation::cpu->PC->Get() < start)
				{
					_HexEditor.HighlightMin = 0;
					_HexEditor.HighlightMax = 0;
				}
			}
			else
			{
				_HexEditor.HighlightMin = 0;
				_HexEditor.HighlightMax = 0;
			}

			_HexEditor.DrawContents(Simulation::program.Memory.get() + start, end + 1 - start, start);
		}
		ImGui::End();
	}
};