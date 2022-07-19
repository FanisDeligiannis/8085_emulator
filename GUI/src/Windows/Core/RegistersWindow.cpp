#include "Windows/Core/RegistersWindow.h"

#include "Backend/GUI_backend.h"

//Basic Registers Window with buffers, using ImGui tables.

RegistersWindow* RegistersWindow::Instance;

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

	ImGui::Begin("Dec");
	{
		if (ImGui::BeginTable("RegistersDec", 7, ImGuiTableFlags_Borders))
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

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%03hhu", A);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%03hhu", B);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%03hhu", C);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%03hhu", D);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%03hhu", E);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%03hhu", H);

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("%03hhu", L);

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("Flags-PC-SPDecUnsigned", 6, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();


			ImGui::TableSetColumnIndex(0);
			ImGui::Text("M");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("S");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("Z");

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("P");

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("Cy");

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("PC");

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%03hhu", M);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", Sign_flag);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", Zero_flag);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", Parity_flag);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", Carry_flag);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%04X", PC);

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("RPDecUnsigned", 4, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("BC");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("DE");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("HL");

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("SP");

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%05hu", (B << 8) | (C));

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%05hu", (D << 8) | (E));

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%05hu", (H << 8) | (L));

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%04X", SP);


			ImGui::EndTable();
		}
	}
	ImGui::End();

	ImGui::Begin("Dec Signed");
	{
		if (ImGui::BeginTable("RegistersDec", 7, ImGuiTableFlags_Borders))
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

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%03hhd", A);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%03hhd", B);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%03hhd", C);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%03hhd", D);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%03hhd", E);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%03hhd", H);

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("%03hhd", L);

			ImGui::EndTable();
		}
		if (ImGui::BeginTable("Flags-PC-SPDecSigned", 6, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("M");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("S");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("Z");

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("P");

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("Cy");

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("PC");

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%03hhd", M);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", Sign_flag);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", Zero_flag);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", Parity_flag);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", Carry_flag);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%04X", PC);

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("RPDecSigned", 4, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("BC");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("DE");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("HL");

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("SP");

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%05hd", (B << 8) | (C));

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%05hd", (D << 8) | (E));

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%05hd", (H << 8) | (L));

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%04X", SP);

			ImGui::EndTable();
		}
	}
	ImGui::End();


	ImGui::Begin("Registers");
	{
		if (ImGui::BeginTable("RegistersHex", 7, ImGuiTableFlags_Borders))
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

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("Flags-PC-SPHex", 6, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("M");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("S");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("Z");

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("P");

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("Cy");

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("PC");

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%02X", M);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", Sign_flag);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", Zero_flag);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", Parity_flag);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", Carry_flag);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%04X", PC);

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("RPHex", 4, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("BC");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("DE");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("HL");

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("SP");

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%04X", (B << 8) | (C));

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%04X", (D << 8) | (E));

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%04X", (H << 8) | (L));

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%04X", SP);

			ImGui::EndTable();
		}
	}
	ImGui::End();
}
