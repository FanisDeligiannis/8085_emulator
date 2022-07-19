#pragma once

#include <cstdint>

namespace Emulator
{
	//8 bit register.

	class Register8
	{
	private:
		union Data //Union of data for use in SIGNED and UNSIGNED operations.
		{
			uint8_t _DataUnsigned;
			int8_t _DataSigned;
		};

		Data _Data;

	public:
		//Set data with an unsigned value
		void SetUnsigned(uint8_t value = 0xff)
		{
			_Data._DataUnsigned = value;
		}

		//Set data with a signed value
		void SetSigned(int8_t value = 0b01111111)
		{
			_Data._DataSigned = value;
		}

		//Set a certain bit
		void SetBit(uint8_t bit, uint8_t value = 1)
		{
			value &= 0x01;
			if (value)
				_Data._DataUnsigned = _Data._DataUnsigned | (1 << bit);
			else
				ClearBit(bit);
		}

		//Clear the register
		void Clear()
		{
			_Data._DataSigned = 0;
		}

		//Clear a certain bit.
		void ClearBit(uint8_t bit)
		{
			_Data._DataUnsigned = _Data._DataUnsigned & (~(1 << bit));
		}

		//Increment signed data.
		void Increment()
		{
			_Data._DataSigned++;
		}

		//Decrement signed data.
		void Decrement()
		{
			_Data._DataSigned--;
		}

		//Get data with type unsigned
		uint8_t GetUnsigned()
		{
			return _Data._DataUnsigned;
		}

		//Get data with type signed
		int8_t GetSigned()
		{
			return _Data._DataSigned;
		}

		//Get a certain bit
		uint8_t GetBit(uint8_t bit)
		{
			return (_Data._DataUnsigned >> bit) & 0x01;
		}
	};

	//16 bit register.
	class Register
	{
	private:
		//This time, _Data is a pointer
		//This does not affect the use of Register at all outside of this class
		//This is needed for SP, so we can point _Data to the ACTUAL stack pointer in _Stack.
		std::shared_ptr<uint16_t> _Data;

	public:
		Register()
		{
			_Data = std::shared_ptr<uint16_t>((uint16_t*)calloc(1, sizeof(uint16_t)), free);
		}

		void SetRef(std::shared_ptr<uint16_t> ref) //Set the reference to point to
		{
			_Data = ref;
		}

		//Everything else is the same as Register8, but with pointer.

		void Set(uint16_t value = 0xffff)
		{
			*(_Data.get()) = value;
		}

		void SetBit(uint8_t bit, int value = 1)
		{
			value &= 0x01;
			if (value)
				*(_Data.get()) = *(_Data.get()) | (1 << bit);
			else
				ClearBit(bit);
		}

		void Clear()
		{
			*(_Data.get()) = 0;
		}

		void ClearBit(uint8_t bit)
		{
			*(_Data.get()) = *(_Data.get()) & (~(1 << bit));
		}

		void Increment()
		{
			(*(_Data.get()))++;
		}

		void Decrement()
		{
			(*(_Data.get()))--;
		}

		uint16_t Get()
		{
			return *_Data;
		}

		uint8_t GetHigh()
		{
			return (*(_Data.get())) >> 8;
		}

		uint8_t GetLow()
		{
			return (*(_Data.get())) & 0xff;
		}

		uint8_t GetBit(uint8_t bit)
		{
			return (*(_Data.get()) >> bit) & 0x01;
		}
	};
}