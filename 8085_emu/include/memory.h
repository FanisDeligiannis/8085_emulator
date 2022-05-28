#pragma once

#include <memory>
#include <cmath>
#include <cstring>

class Memory
{
private:
	size_t _Size;
	uint8_t* _Data;
public:
	Memory()
	{
		_Size = 0;
		_Data = nullptr;
	}

	Memory(uint8_t* data, size_t size)
	{
		_Size = size;
		_Data = data;
	}

	~Memory()
	{
		//leave it up to the rest of the code to clean up
		/*if(_Data != nullptr)
			free(_Data);*/
	}

	void _SetData(uint8_t* data, size_t size)
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

	size_t GetSize()
	{
		return _Size;
	}
};