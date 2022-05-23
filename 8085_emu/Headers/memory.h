#pragma once

#include <memory>
#include <cmath>

class Memory
{
private:
	int _Size;
	uint8_t* _Data;
public:
	Memory()
	{
		//_Size = 0xffff;
		//_Data = (uint8_t*)calloc(_Size, sizeof(uint8_t));
		_Size = 0;
		_Data = nullptr;
	}

	~Memory()
	{
		if(_Data != nullptr)
			free(_Data);
	}

	void _SetData(uint8_t* data, int size)
	{
		_Size = size;
		_Data = data;
	}

	void SetDataAtAddr(uint16_t addr, uint8_t val)
	{
		_Data[addr] = val;
	}

	uint8_t GetDataAtAddr(uint16_t addr)
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

	int GetSize()
	{
		return _Size;
	}
};