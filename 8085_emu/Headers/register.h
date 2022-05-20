#pragma once

#include <cstdint>

class Register8
{
private:
	uint8_t _Data;

public:
	void Set(uint8_t value = 0xff)
	{
		_Data = value;
	}

	void SetBit(uint8_t bit, uint8_t value = 1)
	{
		value &= 0x01;
		_Data = _Data | (1 << bit);
	}

	void Clear()
	{
		_Data = 0;
	}

	void ClearBit(uint8_t bit)
	{
		_Data = _Data & (~(1 << bit));
	}

	uint8_t Get()
	{
		return _Data;
	}

	uint8_t GetBit(uint8_t bit)
	{
		return (_Data >> bit) & 0x01;
	}
};

class Register
{
private:
	int* _Data;

public:
	Register()
	{
		_Data = new int();
	}

	~Register()
	{
		delete _Data;
	}

	void SetRef(int* ref)
	{
		_Data = ref;
	}

	void Set(int value = 0xffff)
	{
		*_Data = value;
	}

	void SetBit(uint8_t bit, int value = 1)
	{
		value &= 0x01;
		*_Data = *_Data | (1 << bit);
	}

	void Clear()
	{
		*_Data = 0;
	}

	void ClearBit(uint8_t bit)
	{
		*_Data = *_Data & (~(1 << bit));
	}

	int Get()
	{
		return *_Data;
	}

	int GetBit(uint8_t bit)
	{
		return (*_Data >> bit) & 0x01;
	}
};