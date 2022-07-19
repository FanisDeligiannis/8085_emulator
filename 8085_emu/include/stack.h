#pragma once

#include <memory>
#include <cmath>
#include <cstdio>
#include <cstdint>

namespace InternalEmulator
{
	//Simple stack.
	//uint8_t* _Data SHOULD point to the _Data of Memory.

	class Stack
	{
	private:
		int _Size;
		std::shared_ptr<uint8_t> _Data;
		std::shared_ptr<uint16_t> _SP;
	public:
		Stack(int bits)
		{
			_Size = (int)pow(2, bits);
			_Data = nullptr;
			_SP = std::shared_ptr<uint16_t>((uint16_t*)calloc(1, sizeof(uint16_t)), free);
			*(_SP.get()) = _Size - 1;
		}

		~Stack() {}

		void SetDataPointer(std::shared_ptr<uint8_t> data)
		{
			_Data = data;
		}

		void Push(uint8_t data)
		{
			_Data.get()[*(_SP.get())] = data;
			(*(_SP.get()))--;
		}

		uint8_t Pop()
		{
			(*(_SP.get()))++;
			uint8_t ret = _Data.get()[*(_SP.get())];
			_Data.get()[*(_SP.get())] = 0;

			return ret;
		}

		uint16_t GetSP()
		{
			return *(_SP.get());
		}

		std::shared_ptr<uint16_t> GetSPPointer()
		{
			return _SP;
		}

		std::shared_ptr<uint8_t> GetData()
		{
			return _Data;
		}
	};
}