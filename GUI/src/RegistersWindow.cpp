#include "RegistersWindow.h"

#include "GUI_backend.h"

//Basic Registers Window with buffers, using ImGui tables.

uint8_t A = 0;
uint8_t B = 0;
uint8_t C = 0;
uint8_t D = 0;
uint8_t E = 0;
uint8_t H = 0;
uint8_t L = 0;
uint8_t M = 0;

uint16_t PC = 0;
uint16_t SP = 0;

uint8_t Sign_flag = 0;
uint8_t Zero_flag = 0;
uint8_t Parity_flag = 0;
uint8_t Carry_flag = 0;

bool updating = false;

void RegistersWindow::UpdateBuffers(bool force)
{
	if (!force && updating) // For thread safety reasons
		return;

	//If one thread is already updating buffers, just give up.

	//But if force=true, don't give up, wait for it to be done updating.
	while (force && updating);

	updating = true;

	if (force) {  }
	else if (Simulation::cpu == nullptr || !Simulation::GetRunning())
	{
		updating = false;
		return;
	}

	A = Simulation::cpu->A->GetUnsigned();
	B = Simulation::cpu->B->GetUnsigned();
	C = Simulation::cpu->C->GetUnsigned();
	D = Simulation::cpu->D->GetUnsigned();
	E = Simulation::cpu->E->GetUnsigned();
	H = Simulation::cpu->H->GetUnsigned();
	L = Simulation::cpu->L->GetUnsigned();
	M = Simulation::cpu->GetUnsignedM();

	Sign_flag = Simulation::cpu->Flags->GetBit(SIGN_FLAG);
	Zero_flag = Simulation::cpu->Flags->GetBit(ZERO_FLAG);
	Parity_flag = Simulation::cpu->Flags->GetBit(PARITY_FLAG);
	Carry_flag = Simulation::cpu->Flags->GetBit(CARRY_FLAG);

	PC = Simulation::cpu->PC->Get();
	SP = Simulation::cpu->SP->Get();

	updating = false;
}

void RegistersWindow::Render()
{
	UpdateBuffers();	

	ImGui::Begin("Registers");
	{
		if (ImGui::BeginTable("Registers", 8, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("A");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("B");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("C");

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("D");

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("E");

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("H");

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("L");

			ImGui::TableSetColumnIndex(7);
			ImGui::Text("M");

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%02X", A);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%02X", B);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%02X", C);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%02X", D);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%02X", E);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%02X", H);

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("%02X", L);

			ImGui::TableSetColumnIndex(7);
			ImGui::Text("%02X", M);

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("Flags-PC-SP", 6, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("S");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("Z");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("P");

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("Cy");

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("PC");

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("SP");

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d", Sign_flag);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", Zero_flag);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", Parity_flag);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", Carry_flag);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%04X", PC);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%04X", SP);

			ImGui::EndTable();
		}
	}
	ImGui::End();
}
