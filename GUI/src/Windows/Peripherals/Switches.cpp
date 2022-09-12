#include "Windows/Peripherals/Switches.h"

#include "imgui.h"
#include "Simulation.h"
#include "ConfigIni.h"

Switches* Switches::Instance;

void Switches::Init()
{
	Instance = this;

	Name = "Switches";
	IncludeInWindows = true;

	_Open = ConfigIni::GetInt("Switches", "Open", 1);
	_Saved = _Open;
}

void Switches::Open()
{
	_Open = true;
	ConfigIni::SetInt("Switches", "Open", 1);
}

void Switches::Close()
{
	if (!_Open && _Open == _Saved)
		return;

	_Open = false;
	_Saved = false;
	ConfigIni::SetInt("Switches", "Open", 0);
}

//Toggle button.
//Modified from creator of ImGui to switch it to vertical.
void Switches::ToggleButton(const char* str_id, bool* v, float woffset)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float width = ImGui::GetFrameHeight() * 1.38;
	float height = width * 1.55f;
	float radius = width * 0.50f;

	if (ImGui::InvisibleButton(str_id, ImVec2(width + woffset, height)))
		*v = !*v;
	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);
	else
		col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius, *v ? (p.y + radius) : (p.y + height - radius)), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

void Switches::SimulationStart()
{
	Simulation::cpu->AddIOInterface(0x20, nullptr, []() -> uint8_t
		{
			Switches* inst = Switches::Instance;
			uint8_t res = 0;
			for (int i = 0; i < 8; i++)
			{
				res |= inst->SwitchesBuf[i] << i;
			}
			return res;
		});
}

void Switches::Render()
{
	if (!_Open)
	{
		Close();
		return;
	}

	ImGui::Begin("Switches", &_Open, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Info"))
			{
				if (ImGui::MenuItem("Is connected to IO chip, addr 20H. (IN 20H)", 0, false, false))
				{

				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		float woffset = 1.2f;

		ToggleButton("switch7", &SwitchesBuf[7], woffset);
		ImGui::SameLine();
		ToggleButton("switch6", &SwitchesBuf[6], woffset);
		ImGui::SameLine();
		ToggleButton("switch5", &SwitchesBuf[5], woffset);
		ImGui::SameLine();
		ToggleButton("switch4", &SwitchesBuf[4], woffset);
		ImGui::SameLine();
		ToggleButton("switch3", &SwitchesBuf[3], woffset);
		ImGui::SameLine();
		ToggleButton("switch2", &SwitchesBuf[2], woffset);
		ImGui::SameLine();
		ToggleButton("switch1", &SwitchesBuf[1], woffset);
		ImGui::SameLine();
		ToggleButton("switch0", &SwitchesBuf[0], woffset);
	}
	ImGui::End();
}
