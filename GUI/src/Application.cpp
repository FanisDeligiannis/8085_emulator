#include "Application.h"

#include <string>

#include "imgui_internal.h"
#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

#include "Simulation.h"
#include "CodeEditor.h"
#include "Texture.h"

std::string IntToHex(int num, std::string prefix = "", int size = -1)
{
	std::string result;

	int temp = num;

	bool done = false;

	while (temp > 0)
	{
		int remainder = temp % 16;
		temp = temp / 16;

		switch (remainder)
		{
		case 0:
			result = "0" + result;
			break;
		case 1:
			result = "1" + result;
			break;
		case 2:
			result = "2" + result;
			break;
		case 3:
			result = "3" + result;
			break;
		case 4:
			result = "4" + result;
			break;
		case 5:
			result = "5" + result;
			break;
		case 6:
			result = "6" + result;
			break;
		case 7:
			result = "7" + result;
			break;
		case 8:
			result = "8" + result;
			break;
		case 9:
			result = "9" + result;
			break;
		case 10:
			result = "A" + result;
			break;
		case 11:
			result = "B" + result;
			break;
		case 12:
			result = "C" + result;
			break;
		case 13:
			result = "D" + result;
			break;
		case 14:
			result = "E" + result;
			break;
		case 15:
			result = "F" + result;
			break;
		}
	}

	if (size >= 0)
	{
		if (result.size() < size)
		{
			while (result.size() < size)
			{
				result = "0" + result;
			}
		}
		else if (result.size() > size)
		{
			result = result.substr(result.size() - size, size);
		}
	}

	return (prefix + result);
}

namespace Application
{
	MemoryEditor mem_edit_1;
	std::string DefaultFile = "";

	void Init()
	{
		Simulation::Init();
		CodeEditor::Init();

		if (!DefaultFile.empty())
		{
			CodeEditor::FilePath = DefaultFile;
			CodeEditor::TextEditorLoadFile(DefaultFile);
		}
	
		mem_edit_1.GotoAddr = 0x0800;
	}


	void ImGuiRender()
	{
		ImGui::DockSpaceOverViewport();




		ImGui::Begin("Controls");
		{
			bool pushed = false;

			if (Simulation::Running)
			{
				pushed = true;

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			if (ImGui::Button("Assemble"))
			{
				Simulation::Assemble(CodeEditor::editor.GetText());
			}

			if (Simulation::Running && (Simulation::Paused || Simulation::cpu->GetHalted()))
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

			if (!Simulation::Running)
			{
				pushed = true;

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				if (Simulation::Running)
				{
					Simulation::Stop();
				}
			}

			if (Simulation::Running && Simulation::cpu->GetHalted())
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
				if (Simulation::Running)
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

			if (Simulation::Running)
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


		ImGui::Begin("Registers");
		if(Simulation::Running)
		{
			std::string result = IntToHex(Simulation::cpu->A->GetUnsigned(), "A: 0x", 2);
			ImGui::Text(result.c_str());
			result = IntToHex(Simulation::cpu->B->GetUnsigned(), "B: 0x", 2);
			ImGui::Text(result.c_str());
			result = IntToHex(Simulation::cpu->C->GetUnsigned(), "C: 0x", 2);
			ImGui::Text(result.c_str());
			result = IntToHex(Simulation::cpu->D->GetUnsigned(), "D: 0x", 2);
			ImGui::Text(result.c_str());
			result = IntToHex(Simulation::cpu->E->GetUnsigned(), "E: 0x", 2);
			ImGui::Text(result.c_str());
			result = IntToHex(Simulation::cpu->H->GetUnsigned(), "H: 0x", 2);
			ImGui::Text(result.c_str());
			result = IntToHex(Simulation::cpu->L->GetUnsigned(), "L: 0x", 2);
			ImGui::Text(result.c_str());
			result = IntToHex(Simulation::cpu->GetUnsignedM(), "M: 0x", 2);
			ImGui::Text(result.c_str());
			ImGui::Spacing();
			result = IntToHex(Simulation::cpu->PC->Get(), "PC: 0x", 4);
			ImGui::Text(result.c_str());
			result = IntToHex(Simulation::cpu->SP->Get(), "SP: 0x", 4);
			ImGui::Text(result.c_str());

			std::string str = (Simulation::cpu->Flags->GetBit(SIGN_FLAG)) == 1 ? "1" : "0";
			ImGui::Text(("Sign Flag: " + str).c_str());
			
			str = (Simulation::cpu->Flags->GetBit(ZERO_FLAG)) == 1 ? "1" : "0";
			ImGui::Text(("Zero Flag: " + str).c_str());

			str = (Simulation::cpu->Flags->GetBit(PARITY_FLAG)) == 1 ? "1" : "0";
			ImGui::Text(("Parity Flag: " + str).c_str());

			str = (Simulation::cpu->Flags->GetBit(CARRY_FLAG)) == 1 ? "1" : "0";
			ImGui::Text(("Carry Flag: " + str).c_str());

		}
		ImGui::End();


		CodeEditor::Render();


		ImGui::Begin("Hex");
		if (Simulation::memory_data != nullptr)
			mem_edit_1.DrawContents(Simulation::memory_data, 0xffff+1);
		ImGui::End();
	}

	void Destroy()
	{
		Simulation::Stop();
	}
}