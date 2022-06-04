#pragma once

#include "imgui.h"

#include "GUI_backend.h"

namespace SegmentDisplay
{
	bool IsValidCharacter(uint8_t val)
	{
		if ((val >= 'a' && val <= 'z') || (val >= 'A' && val <= 'Z') || (val >= '0' && val <= '9'))
		{
			return true;
		}

		return false;
	}

	uint8_t* chars;

	void SimulationStart()
	{
		chars = Simulation::cpu->GetIO()->GetDataAtAddrPointer(0x50);

		chars[0] = 0;
		chars[1] = 0;
		chars[2] = 0;
		chars[3] = 0;
		chars[4] = 0;
		chars[5] = 0;
		chars[6] = 0;
	}

	void Render()
	{
		float radius = 15.5;

		ImGui::Begin("7 Segment Display", 0, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Info"))
				{
					if (ImGui::MenuItem("Is connected to IO chip, addr 50H - 55H. (OUT 50H - OUT 55H)", 0, false, false)) {}
					if (ImGui::MenuItem("This is automatically done by STDM/STDC", 0, false, false)) {}
					if (ImGui::MenuItem("Just point double register DE to the first of the", 0, false, false)) {}
					if (ImGui::MenuItem("6 consecutive data you want to load", 0, false, false)) {}
					if (ImGui::MenuItem("and CALL STDM if your data is a number 0-15,", 0, false, false)) {}
					if (ImGui::MenuItem("or CALL STDC if your data is an ASCII character.", 0, false, false)) {}
					if (ImGui::MenuItem("This will load the data to the display but not show it.", 0, false, false)) {}
					if (ImGui::MenuItem("In order to show it for a frame, CALL DCD", 0, false, false)) {}
					if (ImGui::MenuItem("or CALL DCD continuously to show it all the time.", 0, false, false)) {}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Show last"))
				{
					if (chars != nullptr)
					{
						if (ImGui::MenuItem(" ", 0, false, false)) {}
						chars[6] = 2;
					}
					else
					{
						if (ImGui::MenuItem("No simulation has started yet", 0, false, false)) {}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			if (chars != nullptr)
			{
				if (chars[6] > 0)
				{
					ImGui::PushFont(_SevenSegmentFont);

					for (int i = 5; i >= 0; i--)
					{
						if (IsValidCharacter(chars[i]))
						{
							ImGui::Text("%c", chars[i]);
						}
						else
						{
							ImGui::Text("    ");
						}

						if (i != 0)
						{
							ImGui::SameLine();
						}
					}

					chars[6]--;

					ImGui::PopFont();
				}
			}

		}
		ImGui::End();
	}
}