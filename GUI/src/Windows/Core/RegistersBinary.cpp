#include "Windows/Core/RegistersBinary.h"

#include "RegistersBuffer.h"

#include "Backend/GUI_backend.h"

#include <bitset>

void RegistersBinary::Render()
{
	if (!_Open)
	{
		Close();
		return;
	}

	ImGui::Begin("Registers(Binary)", &_Open);
	{
		if (ImGui::BeginTable("RegistersBin", 7, ImGuiTableFlags_Borders))
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
			ImGui::Text("%s", std::bitset<8>(RegistersBuffer::A).to_string().c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", std::bitset<8>(RegistersBuffer::B).to_string().c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%s", std::bitset<8>(RegistersBuffer::C).to_string().c_str());

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%s", std::bitset<8>(RegistersBuffer::D).to_string().c_str());

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%s", std::bitset<8>(RegistersBuffer::E).to_string().c_str());

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%s", std::bitset<8>(RegistersBuffer::H).to_string().c_str());

			ImGui::TableSetColumnIndex(6);
			ImGui::Text("%s", std::bitset<8>(RegistersBuffer::L).to_string().c_str());

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("Flags-PC-SPBin", 6, ImGuiTableFlags_Borders))
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
			ImGui::Text("%s", std::bitset<8>(RegistersBuffer::M).to_string().c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", RegistersBuffer::Sign_flag);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", RegistersBuffer::Zero_flag);

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", RegistersBuffer::Parity_flag);

			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", RegistersBuffer::Carry_flag);

			ImGui::TableSetColumnIndex(5);
			ImGui::Text("%s", std::bitset<16>(RegistersBuffer::PC).to_string().c_str());

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("RPBin", 4, ImGuiTableFlags_Borders))
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
			ImGui::Text("%s", std::bitset<16>((RegistersBuffer::B << 8) | (RegistersBuffer::C)).to_string().c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", std::bitset<16>((RegistersBuffer::D << 8) | (RegistersBuffer::E)).to_string().c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%s", std::bitset<16>((RegistersBuffer::H << 8) | (RegistersBuffer::L)).to_string().c_str());

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%s", std::bitset<16>(RegistersBuffer::SP).to_string().c_str());

			ImGui::EndTable();
		}
	}
	ImGui::End();
}
