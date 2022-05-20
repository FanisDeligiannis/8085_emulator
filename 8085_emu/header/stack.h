#pragma once

#include <memory>
#include <cmath>

class Stack 
{
private:
	int _Size;
	uint8_t* _Data;
	int _SP;
public:
	Stack(int bits)
	{
		_Size = pow(2, bits);
		_Data = nullptr;
		_SP = int(_Size - 1);
	}

	~Stack() {}

	void SetDataPointer(uint8_t* data)
	{
		_Data = data;
	}

	void Push(uint8_t data) 
	{
		_Data[_SP] = data;
		_SP--;

		//TODO error checking
	}

	int Pop()
	{
		_SP--;
		uint8_t ret = _Data[_SP];
		_Data[_SP] = 0;

		return ret;
	}

	int GetSP()
	{
		return _SP;
	}

	int* GetSPPointer()
	{
		return &_SP;
	}

	uint8_t* GetData()
	{
		return _Data;
	}
};