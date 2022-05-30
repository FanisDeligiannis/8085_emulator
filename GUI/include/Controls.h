#pragma once

#include "imgui.h"
#include "Simulation.h"
#include "CodeEditor.h"
#include <imgui_internal.h>

namespace Controls
{
	void Render()
	{
		ImGui::Begin("Controls");
		{
			bool pushed = false;

			if (Simulation::GetRunning())
			{
				pushed = true;

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			if (ImGui::Button("Assemble"))
			{
				Simulation::Assemble(CodeEditor::editor.GetText());
			}

			if (Simulation::GetRunning() && (Simulation::GetPaused() || Simulation::cpu->GetHalted()))
			{
				if (pushed)
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();

					pushed = false;
				}
			}


			if (ImGui::Button("Run"))
			{
				Simulation::Run();

			}

			if (pushed)
			{
				pushed = false;

				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			if (!Simulation::GetRunning())
			{
				pushed = true;

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				if (Simulation::GetRunning())
				{
					Simulation::Stop();
				}
			}

			if (Simulation::GetRunning() && Simulation::cpu->GetHalted())
			{
				if (pushed)
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}

				pushed = true;

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			ImGui::SameLine();
			if (ImGui::Button("Pause"))
			{
				if (Simulation::GetRunning())
				{
					Simulation::Pause();
				}

			}

			if (pushed)
			{
				pushed = false;

				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			if (Simulation::GetRunning())
			{
				if (Simulation::cpu->GetHalted())
					ImGui::Text("Halted");
				else
					ImGui::Text("Running");
			}
			else if (Simulation::Paused)
			{
				ImGui::Text("Paused");
			}
			else
			{
				ImGui::Text("Not running");
			}
		}

		ImGui::End();
	}
}