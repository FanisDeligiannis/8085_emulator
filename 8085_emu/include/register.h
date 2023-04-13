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
		inline void SetUnsigned(uint8_t value = 0xff)
		{
			_Data._DataUnsigned = value;
		}

		//Set data with a signed value
		inline void SetSigned(int8_t value = 0b01111111)
		{
			_Data._DataSigned = value;
		}

		//Set a certain bit
		inline void SetBit(uint8_t bit, uint8_t value = 1)
		{
			value &= 0x01;
			if (value)
				_Data._DataUnsigned = _Data._DataUnsigned | (1 << bit);
			else
				ClearBit(bit);
		}

		//Clear the register
		inline void Clear()
		{
			_Data._DataSigned = 0;
		}

		//Clear a certain bit.
		inline void ClearBit(uint8_t bit)
		{
			_Data._DataUnsigned = _Data._DataUnsigned & (~(1 << bit));
		}

		//Increment signed data.
		inline void Increment()
		{
			_Data._DataSigned++;
		}

		//Decrement signed data.
		inline void Decrement()
		{
			_Data._DataSigned--;
		}

		//Get data with type unsigned
		inline uint8_t GetUnsigned()
		{
			return _Data._DataUnsigned;
		}

		//Get data with type signed
		inline int8_t GetSigned()
		{
			return _Data._DataSigned;
		}

		//Get a certain bit
		inline uint8_t GetBit(uint8_t bit)
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

		inline void SetRef(std::shared_ptr<uint16_t> ref) //Set the reference to point to
		{
			_Data = ref;
		}

		//Everything else is the same as Register8, but with pointer.

		inline void Set(uint16_t value = 0xffff)
		{
			*(_Data.get()) = value;
		}

		inline void SetBit(uint8_t bit, int value = 1)
		{
			value &= 0x01;
			if (value)
				*(_Data.get()) = *(_Data.get()) | (1 << bit);
			else
				ClearBit(bit);
		}

		inline void Clear()
		{
			*(_Data.get()) = 0;
		}

		inline void ClearBit(uint8_t bit)
		{
			*(_Data.get()) = *(_Data.get()) & (~(1 << bit));
		}

		inline void Increment()
		{
			(*(_Data.get()))++;
		}

		inline void Decrement()
		{
			(*(_Data.get()))--;
		}

		inline uint16_t Get()
		{
			return *_Data;
		}

		inline uint8_t GetHigh()
		{
			return (*(_Data.get())) >> 8;
		}

		inline uint8_t GetLow()
		{
			return (*(_Data.get())) & 0xff;
		}

		inline uint8_t GetBit(uint8_t bit)
		{
			return (*(_Data.get()) >> bit) & 0x01;
		}
	};
}