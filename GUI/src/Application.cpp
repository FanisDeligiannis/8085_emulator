#include "Application.h"

#include <memory>
#include <string>
#include <fstream>
#include <filesystem>

#include "imgui_internal.h"
#include "imgui.h"

#include "ConfigIni.h"
#include "Texture.h"
#include "Simulation.h"

#include "Windows/Window.h"

#include "Windows/Core/CodeEditor.h"
#include "Windows/Core/Controls.h"
#include "Windows/Core/RegistersWindow.h"
#include "Windows/Core/HexEditor.h"
#include "Windows/Core/Popup.h"

#include "Windows/Peripherals/Leds.h"
#include "Windows/Peripherals/Switches.h"
#include "Windows/Peripherals/7SegmentDisplay.h"
#include "Windows/Peripherals/Beep.h"
#include "Windows/Peripherals/Keyboard.h"

namespace Application
{
	std::vector<std::shared_ptr<Window>> Windows = {
		std::make_shared<CodeEditor>(),
		std::make_shared<Controls>(),
		std::make_shared<Popup>(),
		std::make_shared<HexEditor>(),
		std::make_shared<RegistersWindow>(),
		std::make_shared<SegmentDisplay>(),
		std::make_shared<Beep8085>(),
		std::make_shared<Keyboard>(),
		std::make_shared<Leds>(),
		std::make_shared<Switches>()
	};

	std::string DefaultFile = "";

	float cpu_speed;
	int cpu_accuracy;

	void Init()
	{
		Simulation::Init();

		for (int i = 0; i < Windows.size(); i++)
		{
			Windows.at(i)->Init();
		}

		if (!DefaultFile.empty())
		{
			CodeEditor::Instance->FilePath = DefaultFile;
			CodeEditor::Instance->TextEditorLoadFile(DefaultFile);
		}

		cpu_speed = (Simulation::GetClock() / 1000000.0f);
		cpu_accuracy = Simulation::GetAccuracy();
	}

	void ImGuiRender()
	{
		ImGui::DockSpaceOverViewport();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows"))
			{
				for (int i = 0; i < Windows.size(); i++)
				{
					if (Windows.at(i)->IncludeInWindows)
					{
						if (ImGui::MenuItem(Windows.at(i)->Name.c_str(), 0, Windows.at(i)->IsOpen()))
						{
							if (Windows.at(i)->IsOpen())
							{
								Windows.at(i)->Close();
							}
							else
							{
								Windows.at(i)->Open();
							}
						}
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				ImGui::MenuItem("Only affects the UI. For example,", 0, false, false);
				ImGui::MenuItem("how often the Registers", 0, false, false);
				ImGui::MenuItem("Window updates values etc.", 0, false, false);
				if (ImGui::DragInt("FPS", &_TargetFPS, 1, 5, 60))
				{
					SetFPS(_TargetFPS);
				}


				ImGui::MenuItem("Default is 3.2mhz.", 0, false, false);
				ImGui::MenuItem("Beware: It will affect DELA/DELB.", 0, false, false);
				if (ImGui::DragFloat("CPU Clock", &cpu_speed, 0.1f, 0.1f, 10.0f, "%.1fmhz"))
				{
					if (cpu_speed < 0.1f)
						cpu_speed = 0.1f;
					if (cpu_speed > 10.0f)
						cpu_speed = 10.0f;

					Simulation::SetClock((int)(cpu_speed*1000000), cpu_accuracy);
				}

				ImGui::MenuItem("CPU cycles are divided into steps.", 0, false, false);
				ImGui::MenuItem("Warning: Too many slows the clock speed.", 0, false, false);
				if (ImGui::DragInt("Clock Accuracy", &cpu_accuracy, 1, 10, 1000))
				{
					if (cpu_accuracy < 1)
						cpu_accuracy = 1;

					Simulation::SetClock((int)(cpu_speed * 1000000), cpu_accuracy);
				}

				

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		for (int i = 0; i < Windows.size(); i++)
		{
			Windows.at(i)->Render();
		}
	}

	void Update() // Never fired rn.
	{
		for (int i = 0; i < Windows.size(); i++)
		{
			Windows.at(i)->Update();
		}
	}

	void SimulationStart()
	{
		for (int i = 0; i < Windows.size(); i++)
		{
			Windows.at(i)->SimulationStart();
		}
	}

	void SimulationEnd()
	{
		for (int i = 0; i < Windows.size(); i++)
		{
			Windows.at(i)->SimulationEnd();
		}
	}

	void PreDestroy()
	{
		for (int i = 0; i < Windows.size(); i++)
		{
			Windows.at(i)->PreDestroy();
		}
	}

	void Destroy()
	{
		Simulation::Stop();
	}
}