#pragma once

#include "imgui.h"

namespace Leds
{

	void Render()
	{
		float radius = 15.5;
		
		ImGui::Begin("LEDs", 0, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Info"))
				{
					if (ImGui::MenuItem("Is connected to memory location 3000H.", 0, false, false))
					{

					}
				
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			const ImVec2 p = ImGui::GetCursorScreenPos();

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImU32 col = ImColor(ImVec4(1,0,0,1));

			if (Simulation::GetRunning())
			{
				uint8_t ledValues = Simulation::cpu->GetMemory()->GetDataAtAddr(0x3000);

				for (int i = 0; i < 8; i++)
				{
					if ((ledValues & (1 << i)) > 0)
					{
						draw_list->AddCircleFilled(ImVec2(p.x + 20.0f + ((radius * 2 + 5) * (7 - i)), p.y + 20.0f), radius, IM_COL32(0, 255, 0, 255), 0);
					}
					else
					{
						draw_list->AddCircleFilled(ImVec2(p.x + 20.0f + ((radius * 2 + 5) * (7 - i)), p.y + 20.0f), radius, IM_COL32(255, 0, 0, 255), 0);
					}
				}

			}
			else
			{
				for (int i = 0; i < 8; i++)
				{
					draw_list->AddCircleFilled(ImVec2(p.x + 20.0f + ((radius * 2 + 5) * i), p.y + 20.0f), radius, IM_COL32(255, 0, 0, 255), 0);
				}
			}
		}
		ImGui::End();
	}
}