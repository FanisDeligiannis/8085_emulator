#pragma once

#include "imgui.h"

#include "Backend/GUI_backend.h"
#include "Simulation.h"
#include "ConfigIni.h"

namespace SegmentDisplay
{
	char chars[6];
	int show = 0;
	
	bool _Open = true;
	bool _Saved = true;

	void Init()
	{
		_Open = ConfigIni::GetInt("7SegmentDisplay", "Open", 1);
		_Saved = _Open;
	}

	void Open()
	{
		_Open = true;
		ConfigIni::SetInt("7SegmentDisplay", "Open", 1);
	}

	void Close()
	{
		if (!_Open && _Open == _Saved)
			return;

		_Open = false;
		_Saved = false;
		ConfigIni::SetInt("7SegmentDisplay", "Open", 0);
	}

	bool IsValidCharacter(uint8_t val)
	{
		if ((val >= 'a' && val <= 'z') || (val >= 'A' && val <= 'Z') || (val >= '0' && val <= '9'))
		{
			return true;
		}

		return false;
	}


	void Char1(uint8_t val)
	{
		chars[0] = val;
	}
	void Char2(uint8_t val)
	{
		chars[1] = val;
	}
	void Char3(uint8_t val)
	{
		chars[2] = val;
	}
	void Char4(uint8_t val)
	{
		chars[3] = val;
	}
	void Char5(uint8_t val)
	{
		chars[4] = val;
	}
	void Char6(uint8_t val)
	{
		chars[5] = val;
	}
	void Show(uint8_t val)
	{
		show = val;
	}

	void SimulationStart()
	{
		Simulation::cpu->AddIOInterface(0x50, Char1, nullptr);
		Simulation::cpu->AddIOInterface(0x51, Char2, nullptr);
		Simulation::cpu->AddIOInterface(0x52, Char3, nullptr);
		Simulation::cpu->AddIOInterface(0x53, Char4, nullptr);
		Simulation::cpu->AddIOInterface(0x54, Char5, nullptr);
		Simulation::cpu->AddIOInterface(0x55, Char6, nullptr);
		Simulation::cpu->AddIOInterface(0x56, Show, nullptr);

		chars[0] = '\0';
		chars[1] = '\0';
		chars[2] = '\0';
		chars[3] = '\0';
		chars[4] = '\0';
		chars[5] = '\0';
	}

	void Render()
	{
		if (!_Open)
		{
			Close();
			return;
		}
		
		ImGui::Begin("7 Segment Display", &_Open, ImGuiWindowFlags_MenuBar);
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
				if (show > 0)
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

					show--;

					ImGui::PopFont();
				}
			}

		}
		ImGui::End();
	}
}