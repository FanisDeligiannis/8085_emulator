#pragma once

#include "imgui.h"

namespace Switches
{
	//Toggle button.
	//Modified from creator of ImGui to switch it to vertical.
	void ToggleButton(const char* str_id, bool* v)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float width = ImGui::GetFrameHeight() * 1.38;
		float height = width * 1.55f;
		float radius = width * 0.50f;

		if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
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

	uint8_t* switches;

	void SimulationStart()
	{
		switches = Simulation::cpu->GetIO()->GetDataAtAddrPointer(0x20);
	}

	void Render()
	{
		if (Simulation::GetRunning())
		{
			switches = Simulation::cpu->GetIO()->GetDataAtAddrPointer(0x20);
		}

		float radius = 15.5;

		ImGui::Begin("Switches", 0, ImGuiWindowFlags_MenuBar);
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

			ToggleButton("switch7", &switch7);
			ImGui::SameLine();
			ToggleButton("switch6", &switch6);
			ImGui::SameLine();
			ToggleButton("switch5", &switch5);
			ImGui::SameLine();
			ToggleButton("switch4", &switch4);
			ImGui::SameLine();
			ToggleButton("switch3", &switch3);
			ImGui::SameLine();
			ToggleButton("switch2", &switch2);
			ImGui::SameLine();
			ToggleButton("switch1", &switch1);
			ImGui::SameLine();
			ToggleButton("switch0", &switch0);
		}
		ImGui::End();

		if (switches != nullptr)
		{
			uint8_t val = (switch7 << 7) | (switch6 << 6) | (switch5 << 5) | (switch4 << 4) | (switch3 << 3) | (switch2 << 2) | (switch1 << 1) | (switch0 << 0);
			*switches = val;
		}
	}
}