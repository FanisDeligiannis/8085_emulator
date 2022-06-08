#pragma once

#include "imgui.h"

#include "ConfigIni.h"

namespace Leds
{
	uint8_t* ledValues = nullptr;

	bool _Open = true;

	void Init()
	{
		_Open = ConfigIni::GetInt("Leds", "Open", 1);
	}

	void Open()
	{
		_Open = true;
		ConfigIni::SetInt("Leds", "Open", 1);
	}

	void Close()
	{
		if (!_Open)
			return;

		_Open = false;
		ConfigIni::SetInt("Leds", "Open", 0);
	}

	void SimulationStart()
	{
		ledValues = Simulation::cpu->GetIO()->GetDataAtAddrPointer(0x30);
		*ledValues = 0;
	}

	void Render()
	{
		if (!_Open)
		{
			Close();
			return;
		}

		float radius = 15.5;
		
		ImGui::Begin("LEDs", &_Open, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Info"))
				{
					if (ImGui::MenuItem("Is connected to IO chip, addr 30H. (OUT 30H)", 0, false, false))
					{

					}
				
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			const ImVec2 p = ImGui::GetCursorScreenPos();

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImU32 col = ImColor(ImVec4(1,0,0,1));
			
			//Draw green / red circles.
			if (ledValues != nullptr)
			{
				for (int i = 0; i < 8; i++)
				{
					if ((*ledValues & (1 << i)) > 0)
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
					draw_list->AddCircleFilled(ImVec2(p.x + 20.0f + ((radius * 2 + 5) * (7 - i)), p.y + 20.0f), radius, IM_COL32(255, 0, 0, 255), 0);
				}
			}
		}
		ImGui::End();
	}
}