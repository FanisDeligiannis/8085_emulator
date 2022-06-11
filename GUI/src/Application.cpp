#include "Application.h"

#include <string>

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
	}

	void ImGuiRender()
	{
		ImGui::DockSpaceOverViewport();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("7 Segment Display"))
				{
					SegmentDisplay::Open();
				}

				if (ImGui::MenuItem("Keyboard"))
				{
					Keyboard::Open();
				}

				if (ImGui::MenuItem("Leds"))
				{
					Leds::Open();
				}

				if (ImGui::MenuItem("Switches"))
				{
					Switches::Open();
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

		Beep8085::Render();

		Keyboard::Render();

		CodeEditor::Render();

		HexEditor::Render();
		
		ImGui::End();
	}

	void SimulationStart()
	{
		Leds::SimulationStart();
		Switches::SimulationStart();
		SegmentDisplay::SimulationStart();
		Beep8085::SimulationStart();
		Keyboard::SimulationStart();
	}

	void Destroy()
	{
		Simulation::Stop();
	}
}