#pragma once

#pragma once

#include <memory>
#include <cmath>

class Memory
{
private:
	int _Size;
	uint8_t* _Data;
public:
	Memory(int bits)
	{
		_Size = pow(2, bits);
		_Data = (uint8_t*)calloc(_Size, sizeof(uint8_t));
	}

	~Memory()
	{
		free(_Data);
	}

	void SetMemory(uint16_t addr, uint8_t val)
	{
		_Data[addr] = val;
	}

	uint8_t GetMemory(uint16_t addr)
	{
		return _Data[addr];
	}

	void CopyToMemory(uint16_t addr, uint8_t* values, uint16_t size)
	{
		memcpy(_Data + addr, values, size);
	}

	uint8_t* GetData()
	{
		return _Data;
	}
};