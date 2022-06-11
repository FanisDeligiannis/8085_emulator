#pragma once

#include <iostream>

#include <imgui_internal.h>
#include "imgui.h"

#include "Backend/GUI_backend.h"

#include "Simulation.h"
#include "CodeEditor.h"

namespace Controls
{
	//Pretty straight forward.
	//Assemble, Step, INTR, Run, Stop, Pause buttons.

	ImFont* _Font;

	void Init()
	{
		_Font = LoadFont(35);
	}

	bool Button(std::string text, bool condition = true, ImVec2 size = ImVec2(0,0))
	{
		if (!condition)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		bool ret = ImGui::Button(text.c_str(), size);

		if (!condition)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		return ret;
	}

	void Render()
	{
		ImGui::Begin("Controls");
		{
			float width = ImGui::GetWindowWidth() / 3 - (3.5 * 3);

			if (Button("Assemble", 
				!Simulation::GetRunning(), 
				ImVec2(width, 40)))
			{
				Simulation::Assemble(CodeEditor::editor.GetText());
			}

			ImGui::SameLine();

			if (Button("Run",
				!Simulation::GetRunning() || Simulation::GetPaused() || Simulation::cpu->GetHalted() || Simulation::_Stepping,
				ImVec2(width, 40)))
			{
				Simulation::Run();
			}

			ImGui::SameLine();


			if (Button("Step", 
				!Simulation::GetRunning() || Simulation::GetPaused() || Simulation::cpu->GetHalted() || Simulation::_Stepping,
				ImVec2(width, 40)))
			{
				Simulation::Step();
			}

			ImGui::Separator();
			
			if (Button("INTR",
				Simulation::GetRunning(),
				ImVec2(width, 40)))
			{
				Simulation::cpu->_IPINTR = true;
			}

			ImGui::SameLine();

			if (Button("Stop", 
				Simulation::GetRunning(),
				ImVec2(width, 40)))
			{
				if (Simulation::GetRunning())
				{
					Simulation::Stop();
				}
			}

			ImGui::SameLine();

			if (Button("Pause", 
				Simulation::GetRunning() && !Simulation::GetPaused() && !Simulation::cpu->GetHalted(),
				ImVec2(width, 40)))
			{
				Simulation::Pause();

			}

			ImGui::Separator();

			ImGui::PushFont(_Font);

			std::string text = "";

			if (Simulation::cpu != nullptr)
			{
				if (Simulation::_Stepping)
					text = "Stepping";
				else if (Simulation::GetPaused())
					text = "Paused";
				else if (Simulation::cpu->GetHalted())
					text = "Halted";
				else if (Simulation::GetRunning())
					text = "Running";
				else
					text = "Not Running";
			}
			else
			{
				text = "Not Running";
			}

			auto windowWidth = ImGui::GetWindowSize().x;

			auto textSize = ImGui::CalcTextSize(text.c_str());
			auto textWidth = textSize.x;
			
			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

			ImGui::Text(text.c_str());

			ImGui::PopFont();
			ImGui::SetWindowSize(ImVec2(ImGui::GetWindowWidth(), ImGui::GetCursorPosY() + 10));
		}

		ImGui::End();
	}
}