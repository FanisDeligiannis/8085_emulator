#pragma once

#include <cstdint>

class Register8
{
private:
	union Data
	{
		uint8_t _DataUnsigned;
		int8_t _DataSigned;
	};

	Data _Data;

public:
	void SetUnsigned(uint8_t value = 0xff)
	{
		_Data._DataUnsigned = value;
	}

	void SetSigned(int8_t value = 0b01111111)
	{
		_Data._DataSigned = value;
	}

	void SetBit(uint8_t bit, uint8_t value = 1)
	{
		value &= 0x01;
		_Data._DataUnsigned = _Data._DataUnsigned | (1 << bit);
	}

	void Clear()
	{
		_Data._DataSigned = 0;
	}

	void ClearBit(uint8_t bit)
	{
		_Data._DataUnsigned = _Data._DataUnsigned & (~(1 << bit));
	}

	void Increment()
	{
		_Data._DataSigned++;
	}

	void Decrement()
	{
		_Data._DataSigned--;
	}

	uint8_t GetUnsigned()
	{
		return _Data._DataUnsigned;
	}

	int8_t GetSigned()
	{
		return _Data._DataSigned;
	}

	uint8_t GetBit(uint8_t bit)
	{
		return (_Data._DataUnsigned >> bit) & 0x01;
	}
};

class Register
{
private:
	uint16_t* _Data;

public:
	Register()
	{
		_Data = new uint16_t();
	}

	~Register()
	{
		delete _Data;
	}

	void SetRef(uint16_t* ref)
	{
		_Data = ref;
	}

	void Set(uint16_t value = 0xffff)
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

	void Increment()
	{
		(*_Data)++;
	}

	void Decrement()
	{
		(*_Data)--;
	}

	uint16_t Get()
	{
		return *_Data;
	}

	uint8_t GetHigh()
	{
		return (*_Data)>>8;
	}

	uint8_t GetLow()
	{
		return (*_Data) & 0xff;
	}

	uint8_t GetBit(uint8_t bit)
	{
		return (*_Data >> bit) & 0x01;
	}
};