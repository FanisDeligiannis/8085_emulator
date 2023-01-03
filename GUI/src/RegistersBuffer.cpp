#include "RegistersBuffer.h"

#include "Simulation.h"

uint8_t RegistersBuffer::A = 0;
uint8_t RegistersBuffer::B = 0;
uint8_t RegistersBuffer::C = 0;
uint8_t RegistersBuffer::D = 0;
uint8_t RegistersBuffer::E = 0;
uint8_t RegistersBuffer::H = 0;
uint8_t RegistersBuffer::L = 0;
uint8_t RegistersBuffer::M = 0;

uint16_t RegistersBuffer::PC = 0;
uint16_t RegistersBuffer::SP = 0;

uint8_t RegistersBuffer::Sign_flag = 0;
uint8_t RegistersBuffer::Zero_flag = 0;
uint8_t RegistersBuffer::Parity_flag = 0;
uint8_t RegistersBuffer::Carry_flag = 0;

bool RegistersBuffer::updating = false;

void RegistersBuffer::UpdateBuffers(bool force)
{
	if (!force && updating) // For thread safety reasons
		return;

	//If one thread is already updating buffers, just give up.

	//But if force=true, don't give up, wait for it to be done updating.
	while (force && updating);

	updating = true;

	if (force) {}
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
