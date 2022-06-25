#include "Application.h"

#include <string>
#include <fstream>
#include <filesystem>

#include "imgui_internal.h"
#include "imgui.h"

#include "ConfigIni.h"
#include "Texture.h"
#include "Simulation.h"

#include "Windows/CodeEditor.h"
#include "Windows/Controls.h"
#include "Windows/RegistersWindow.h"
#include "Windows/HexEditor.h"

#include "Windows/Peripherals/Leds.h"
#include "Windows/Peripherals/Switches.h"
#include "Windows/Peripherals/7SegmentDisplay.h"
#include "Windows/Peripherals/Beep.h"
#include "Windows/Peripherals/Keyboard.h"

namespace Application
{
	std::string DefaultFile = "";

	float cpu_speed;
	int cpu_accuracy;

	void Init()
	{
		Simulation::Init();
		Controls::Init();
		CodeEditor::Init();
		HexEditor::Init();

		SegmentDisplay::Init();
		Keyboard::Init();
		Leds::Init();
		Switches::Init();

		if (!DefaultFile.empty())
		{
			CodeEditor::FilePath = DefaultFile;
			CodeEditor::TextEditorLoadFile(DefaultFile);
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
				if (ImGui::MenuItem("Hex Editor", 0, HexEditor::_Open))
				{
					HexEditor::Open();
				}

				if (ImGui::MenuItem("7 Segment Display", 0, SegmentDisplay::_Open))
				{
					SegmentDisplay::Open();
				}

				if (ImGui::MenuItem("Keyboard", 0, Keyboard::_Open))
				{
					Keyboard::Open();
				}

				if (ImGui::MenuItem("Leds", 0, Leds::_Open))
				{
					Leds::Open();
				}

				if (ImGui::MenuItem("Switches", 0, Switches::_Open))
				{
					Switches::Open();
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


		Switches::Render();
		
		SegmentDisplay::Render();

		Leds::Render();

		Controls::Render();

		RegistersWindow::Render();

		Keyboard::Render();

		CodeEditor::Render();

		HexEditor::Render();
	}

	void Update()
	{
		Beep8085::Update();

		Keyboard::Update();
	}

	void SimulationStart()
	{
		Leds::SimulationStart();
		Switches::SimulationStart();
		SegmentDisplay::SimulationStart();
		Beep8085::SimulationStart();
		Keyboard::SimulationStart();
	}

	void PreDestroy()
	{
		CodeEditor::PreDestroy();
	}

	void Destroy()
	{
		Simulation::Stop();
	}
}