#pragma once

#include <cstdint>

class RegistersBuffer
{
public:
	static uint8_t A;
	static uint8_t B;
	static uint8_t C;
	static uint8_t D;
	static uint8_t E;
	static uint8_t H;
	static uint8_t L;
	static uint8_t M;

	static uint16_t PC;
	static uint16_t SP;

	static uint8_t Sign_flag;
	static uint8_t Zero_flag;
	static uint8_t Parity_flag;
	static uint8_t Carry_flag;

private:
	static bool updating;

public:
	static void UpdateBuffers(bool force = false);
};