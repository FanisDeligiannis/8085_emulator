#pragma once

#include <memory>
#include <cmath>
#include <cstdint>

//Simple stack.
//uint8_t* _Data SHOULD point to the _Data of memory.

class Stack 
{
private:
	int _Size;
	uint8_t* _Data;
	uint16_t* _SP;
public:
	Stack(int bits)
	{
		_Size = (int) pow(2, bits);
		_Data = nullptr;
		_SP = new uint16_t(_Size - 1);
	}

	~Stack() {}

	void SetDataPointer(uint8_t* data)
	{
		_Data = data;
	}

	void Push(uint8_t data) 
	{
		_Data[*_SP] = data;
		(* _SP)--;

		//TODO error checking
	}

	uint8_t Pop()
	{
		(* _SP)++;
		uint8_t ret = _Data[*_SP];
		_Data[*_SP] = 0;

		return ret;
	}

	uint16_t GetSP()
	{
		return *_SP;
	}

	uint16_t* GetSPPointer()
	{
		return _SP;
	}

	uint8_t* GetData()
	{
		return _Data;
	}
};