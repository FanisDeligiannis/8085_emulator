#include "Windows/Core/RegistersWindow.h"

#include "RegistersBuffer.h"

#include "Backend/GUI_backend.h"

//Basic Registers Window with buffers, using ImGui tables.

void RegistersWindow::Render()
{
	RegistersBuffer::UpdateBuffers();

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
			ImGui::Text("%03hhu", RegistersBuffer::A);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%03hhu", RegistersBuffer::B);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%03hhu", RegistersBuffer::C);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%03hhu", RegistersBuffer::D);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%03hhu", RegistersBuffer::E);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%03hhu", RegistersBuffer::H);

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("%03hhu", RegistersBuffer::L);

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
			ImGui::Text("%03hhu", RegistersBuffer::M);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", RegistersBuffer::Sign_flag);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", RegistersBuffer::Zero_flag);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", RegistersBuffer::Parity_flag);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", RegistersBuffer::Carry_flag);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%04X", RegistersBuffer::PC);

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
			ImGui::Text("%05hu", (RegistersBuffer::B << 8) | (RegistersBuffer::C));

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%05hu", (RegistersBuffer::D << 8) | (RegistersBuffer::E));

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%05hu", (RegistersBuffer::H << 8) | (RegistersBuffer::L));

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%04X", RegistersBuffer::SP);


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
			ImGui::Text("%03hhd", RegistersBuffer::A);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%03hhd", RegistersBuffer::B);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%03hhd", RegistersBuffer::C);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%03hhd", RegistersBuffer::D);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%03hhd", RegistersBuffer::E);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%03hhd", RegistersBuffer::H);

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("%03hhd", RegistersBuffer::L);

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
			ImGui::Text("%03hhd", RegistersBuffer::M);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", RegistersBuffer::Sign_flag);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", RegistersBuffer::Zero_flag);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", RegistersBuffer::Parity_flag);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", RegistersBuffer::Carry_flag);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%04X", RegistersBuffer::PC);

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
			ImGui::Text("%05hd", (RegistersBuffer::B << 8) | (RegistersBuffer::C));

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%05hd", (RegistersBuffer::D << 8) | (RegistersBuffer::E));

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%05hd", (RegistersBuffer::H << 8) | (RegistersBuffer::L));

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%04X", RegistersBuffer::SP);

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
			ImGui::Text("%02X", RegistersBuffer::A);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%02X", RegistersBuffer::B);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%02X", RegistersBuffer::C);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%02X", RegistersBuffer::D);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%02X", RegistersBuffer::E);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%02X", RegistersBuffer::H);

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("%02X", RegistersBuffer::L);

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
			ImGui::Text("%02X", RegistersBuffer::M);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", RegistersBuffer::Sign_flag);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", RegistersBuffer::Zero_flag);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", RegistersBuffer::Parity_flag);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", RegistersBuffer::Carry_flag);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%04X", RegistersBuffer::PC);

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
			ImGui::Text("%04X", (RegistersBuffer::B << 8) | (RegistersBuffer::C));

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%04X", (RegistersBuffer::D << 8) | (RegistersBuffer::E));

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%04X", (RegistersBuffer::H << 8) | (RegistersBuffer::L));

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%04X", RegistersBuffer::SP);

			ImGui::EndTable();
		}
	}
	ImGui::End();
}
