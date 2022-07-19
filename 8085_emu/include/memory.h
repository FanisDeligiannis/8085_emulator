#pragma once

#include <memory>
#include <cmath>
#include <cstring>

namespace Emulator
{

	//Simple memory class
	//Size should be 0xffff
	//Data size is 8 bits

	class Memory
	{
	private:
		size_t _Size;
		std::shared_ptr<uint8_t> _Data;
	public:
		Memory()
		{
			_Size = 0;
			_Data = nullptr;
		}

		Memory(std::shared_ptr<uint8_t> data, size_t size)
		{
			_Size = size;
			_Data = data;
		}

		void _SetData(std::shared_ptr<uint8_t> data, size_t size)
		{
			_Size = size;
			_Data = data;
		}

		void SetDataAtAddr(uint16_t addr, uint8_t val)
		{
			_Data.get()[addr] = val;
		}

		uint8_t GetDataAtAddr(uint16_t addr)
		{
			return _Data.get()[addr];
		}

		void CopyToMemory(uint16_t addr, uint8_t* values, uint16_t size)
		{
			memcpy(_Data.get() + addr, values, size);
		}

		std::shared_ptr<uint8_t> GetData()
		{
			return _Data;
		}

		size_t GetSize()
		{
			return _Size;
		}
	};
}