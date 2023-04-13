#pragma once

#include <memory>
#include <cmath>
#include <cstdio>
#include <cstdint>

namespace InternalEmulator
{
	//Simple stack.
	//uint8_t* _Data SHOULD point to the _Data of Memory.

#define STACK_START 0xFFFE

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
			//*(_SP.get()) = _Size - 1;
			*(_SP.get()) = STACK_START;
		}

		~Stack() {}

		inline void SetDataPointer(std::shared_ptr<uint8_t> data)
		{
			_Data = data;
		}

		inline void Push(uint8_t data)
		{
			(*(_SP.get()))--;
			_Data.get()[*(_SP.get())] = data;
		}

		inline uint8_t Pop()
		{
			uint8_t ret = _Data.get()[*(_SP.get())];
			_Data.get()[*(_SP.get())] = 0;
			(*(_SP.get()))++;

			if (*(_SP.get()) > STACK_START)
			{
				//ERROR!
#ifndef _DIST //temporary. Visible in the console, if you're on release/debug mode.
				printf("%s\n", "ERROR: POP on empty stack occured!");
#endif 
			}

			return ret;
		}

		inline uint16_t GetSP()
		{
			return *(_SP.get());
		}

		inline std::shared_ptr<uint16_t> GetSPPointer()
		{
			return _SP;
		}

		inline std::shared_ptr<uint8_t> GetData()
		{
			return _Data;
		}
	};
}