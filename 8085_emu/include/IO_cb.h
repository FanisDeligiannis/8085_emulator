#pragma once

#include <cstdint>

struct IOCallback
{
	uint16_t addr;

	void(*OUTPUT)(uint8_t out) = nullptr;
	uint8_t(*INPUT)() = nullptr;
};