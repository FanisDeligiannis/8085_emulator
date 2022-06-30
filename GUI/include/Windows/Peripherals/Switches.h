#pragma once

#include "imgui.h"
#include "Simulation.h"
#include "ConfigIni.h"

namespace Switches
{
	bool _Open = true;
	bool _Saved = true;

	void Init()
	{
		_Open = ConfigIni::GetInt("Switches", "Open", 1);
		_Saved = _Open;
	}

	void Open()
	{
		_Open = true;
		ConfigIni::SetInt("Switches", "Open", 1);
	}

	void Close()
	{
		if (!_Open && _Open == _Saved)
			return;
		
		_Open = false;
		_Saved = false;
		ConfigIni::SetInt("Switches", "Open", 0);
	}
	
	//Toggle button.
	//Modified from creator of ImGui to switch it to vertical.
	void ToggleButton(const char* str_id, bool* v, float woffset = 0.0f)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float width = ImGui::GetFrameHeight() * 1.38;
		float height = width * 1.55f;
		float radius = width * 0.50f;

		if (ImGui::InvisibleButton(str_id, ImVec2(width + woffset, height)))
			*v = !*v;
		ImU32 col_bg;
		if (ImGui::IsItemHovered())
			col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);
		else
			col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);

		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius, *v ? (p.y + radius) : (p.y + height - radius)), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	}

	//Switches buffer.

	bool switch7;
	bool switch6;
	bool switch5;
	bool switch4;
	bool switch3;
	bool switch2;
	bool switch1;
	bool switch0;

	void SimulationStart()
	{
		Simulation::cpu->AddIOInterface(0x20, nullptr, []() -> uint8_t { return  (switch7 << 7) | (switch6 << 6) | (switch5 << 5) | (switch4 << 4) | (switch3 << 3) | (switch2 << 2) | (switch1 << 1) | (switch0 << 0); });
	}

	void Render()
	{
		if(!_Open)
		{
			Close();
			return;
		}

		ImGui::Begin("Switches", &_Open, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Info"))
				{
					if (ImGui::MenuItem("Is connected to IO chip, addr 20H. (IN 20H)", 0, false, false))
					{

					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			float woffset = 1.2f;

			ToggleButton("switch7", &switch7, woffset);
			ImGui::SameLine();
			ToggleButton("switch6", &switch6, woffset);
			ImGui::SameLine();
			ToggleButton("switch5", &switch5, woffset);
			ImGui::SameLine();
			ToggleButton("switch4", &switch4, woffset);
			ImGui::SameLine();
			ToggleButton("switch3", &switch3, woffset);
			ImGui::SameLine();
			ToggleButton("switch2", &switch2, woffset);
			ImGui::SameLine();
			ToggleButton("switch1", &switch1, woffset);
			ImGui::SameLine();
			ToggleButton("switch0", &switch0, woffset);
		}
		ImGui::End();
	}
}