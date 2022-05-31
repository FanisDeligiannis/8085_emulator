#include "Application.h"

#include <string>

#include "imgui_internal.h"
#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

#include "ConfigIni.h"
#include "Texture.h"
#include "Simulation.h"

#include "CodeEditor.h"
#include "Controls.h"
#include "RegistersWindow.h"
#include "Peripherals/Leds.h"
#include "Peripherals/Switches.h"


namespace Application
{
	MemoryEditor mem_edit_1;
	std::string DefaultFile = "";

	void Init()
	{
		Simulation::Init();
		Controls::Init();
		CodeEditor::Init();

		if (!DefaultFile.empty())
		{
			CodeEditor::FilePath = DefaultFile;
			CodeEditor::TextEditorLoadFile(DefaultFile);
		}
	}


	void ImGuiRender()
	{
		ImGui::DockSpaceOverViewport();

		Switches::Render();

		Leds::Render();

		Controls::Render();

		RegistersWindow::Render();

		CodeEditor::Render();

		ImGui::Begin("Hex");
		if (Simulation::memory_data != nullptr)
			mem_edit_1.DrawContents(Simulation::memory_data + 0x0800 , 0xffff+1 - 0x0800, 0x0800);
		ImGui::End();
	}

	void Destroy()
	{
		Simulation::Stop();
	}
}