#pragma once

#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

namespace HexEditor
{
	MemoryEditor _HexEditor;
	uint16_t prevPC = 0;

	uint16_t start = 0x0800;
	uint16_t end = 0xffff;

	void Init()
	{
		_HexEditor.HighlightColor = 0xff707000; // IM_COL32(255, 0, 0, 255);

	}

	void Render()
	{
		ImGui::Begin("Hex");
		if (Simulation::memory_data != nullptr)
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

			_HexEditor.DrawContents(Simulation::memory_data + start, end + 1 - start, start);
		}
	}
}