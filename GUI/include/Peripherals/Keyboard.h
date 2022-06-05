#pragma once

namespace Keyboard
{
	uint8_t* Scan;
	uint8_t* Out;

	int8_t lastButton = -1;

	void SimulationStart()
	{
		Scan = Simulation::_IOchip->GetDataAtAddrPointer(0x28);
		Out = Simulation::_IOchip->GetDataAtAddrPointer(0x18);

		*Scan = 0x7f;
		*Out = 0xff;
		lastButton = -1;

		Simulation::_IOchip->ShouldWaitResponse(0x18, true);
		Simulation::_IOchip->DoneResponse(0x18, false);
	}


	void Render()
	{
		ImGui::Begin("Keyboard", 0, ImGuiWindowFlags_MenuBar);
		{

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Info"))
				{
					if (ImGui::MenuItem("Scan line at IO address 28H (OUT 28H)", 0, false, false)) {}
					if (ImGui::MenuItem("Output at IO address 18H (IN 18H)", 0, false, false)) {}
					if (ImGui::MenuItem("For example, to see if 1 is pressed:", 0, false, false)) {}
					if (ImGui::MenuItem("Set the scanline (28H) to 11111110 (first line)", 0, false, false)) {}
					if (ImGui::MenuItem("And check the output(18H) to see if the 2nd LSB is 0 (2nd button)", 0, false, false)) {}
					if (ImGui::MenuItem("If 1 is actually pressed, it'll be 111111101B", 0, false, false)) {}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Kind"))
				{
					if (ImGui::MenuItem("You can simply \"CALL KIND\", and the program will", 0, false, false)) {}
					if (ImGui::MenuItem("hang until input is given. KIND also calls DCD while hanging.", 0, false, false)) {}
					if (ImGui::MenuItem("KIND will only affect the Accumulator, which will be equal ", 0, false, false)) {}
					if (ImGui::MenuItem("to the button pressed.", 0, false, false)) {}
					if (ImGui::MenuItem("If 1 was pressed, A=1. If B was pressed, A=12 etc.", 0, false, false)) {}

					ImGui::EndMenu();
				}


				if (ImGui::BeginMenu("Interrupt"))
				{
					if (ImGui::MenuItem("Whenever a key is pressed, the keyboard", 0, false, false)) {}
					if (ImGui::MenuItem("triggers an interrupt RST5.5", 0, false, false)) {}
					if (ImGui::MenuItem("By setting the appropriate mask and enabling interrupts", 0, false, false)) {}
					if (ImGui::MenuItem("You can use that to know when a button is pressed", 0, false, false)) {}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			float scale = 0.6;
			ImVec2 size = ImVec2((ImGui::GetWindowWidth() / 4) * scale, (ImGui::GetWindowWidth() / 4) * scale);

			float center = ImGui::GetWindowWidth() / 2;

			float newCursorX = center - ((size.x + 7) * 2);

			ImGui::SetCursorPosX(newCursorX);
			
			if (ImGui::Button("0", size))
			{
				lastButton = 0;
				
				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}

			} ImGui::SameLine();

			if (ImGui::Button("1", size))
			{
				lastButton = 1;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("2", size))
			{
				lastButton = 2;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("3", size))
			{
				lastButton = 3;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} 

			ImGui::SetCursorPosX(newCursorX);
			
			if (ImGui::Button("4", size))
			{
				lastButton = 4;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();


			if (ImGui::Button("5", size))
			{
				lastButton = 5;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("6", size))
			{
				lastButton = 6;
				
				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("7", size))
			{
				lastButton = 7;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			}

			ImGui::SetCursorPosX(newCursorX);
			
			if (ImGui::Button("8", size))
			{
				lastButton = 8;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("9", size))
			{
				lastButton = 9;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("A", size))
			{
				lastButton = 10;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("B", size))
			{
				lastButton = 11;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			}

			ImGui::SetCursorPosX(newCursorX);
			
			if (ImGui::Button("C", size))
			{
				lastButton = 12;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("D", size))
			{
				lastButton = 13;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("E", size))
			{
				lastButton = 14;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			} ImGui::SameLine();

			if (ImGui::Button("F", size))
			{
				lastButton = 15;

				if (Simulation::GetRunning())
				{
					Simulation::cpu->_IP55 = true;
				}
			}

			
			if (Scan != nullptr)
			{
				if (*Scan != 0x7f)
				{
					*Out = 0xff;

					if (!(*Scan & 0b00000001))
					{
						if (lastButton == 0)
						{
							*Out = 0b11111110;
						}
						else if (lastButton == 1)
						{
							*Out = 0b11111101;
						}
						else if (lastButton == 2)
						{
							*Out = 0b11111011;
						}
						else if (lastButton == 3)
						{
							*Out = 0b11110111;
						}
					}
					else if (!(*Scan & 0b00000010))
					{
						if (lastButton == 4)
						{
							*Out = 0b11111110;
						}
						else if (lastButton == 5)
						{
							*Out = 0b11111101;
						}
						else if (lastButton == 6)
						{
							*Out = 0b11111011;
						}
						else if (lastButton == 7)
						{
							*Out = 0b11110111;
						}
					}
					else if (!(*Scan & 0b00000100))
					{
						if (lastButton == 8)
						{
							*Out = 0b11111110;
						}
						else if (lastButton == 9)
						{
							*Out = 0b11111101;
						}
						else if (lastButton == 10)
						{
							*Out = 0b11111011;
						}
						else if (lastButton == 11)
						{
							*Out = 0b11110111;
						}
					}

					else if (!(*Scan & 0b00001000))
					{
						if (lastButton == 12)
						{
							*Out = 0b11111110;
						}
						else if (lastButton == 13)
						{
							*Out = 0b11111101;
						}
						else if (lastButton == 14)
						{
							*Out = 0b11111011;
						}
						else if (lastButton == 15)
						{
							*Out = 0b11110111;
						}
					}
					
					if(*Out != 0xff)
						lastButton = -1;
					*Scan = 0x7f;

					Simulation::_IOchip->DoneResponse(0x18);
				}
			}
		}
		ImGui::End();
	}
}