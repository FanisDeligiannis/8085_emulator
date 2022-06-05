#pragma once

#include <memory>
#include <cmath>
#include <cstring>

//Very simple.
//0xff size.

class IOchip
{
private:
	size_t _Size;
	uint8_t* _Data;
	volatile bool* _WaitResponse;
	volatile bool* _DoneResponse;

public:
	IOchip()
	{
		_Size = 0xff;
		_Data = (uint8_t*)calloc(0xff + 1, sizeof(uint8_t));
		_WaitResponse = (bool*)calloc(0xff + 1, sizeof(uint8_t));
		_DoneResponse = (bool*) calloc(0xff+1, sizeof(uint8_t));

		if (_Data == nullptr)
		{
			printf("Failed to initialise IO chip!\n");
			exit(1);
		}

		if (_WaitResponse == nullptr)
		{
			printf("Failed to initialise IO chip!\n");
			exit(1);
		}

		if (_DoneResponse == nullptr)
		{
			printf("Failed to initialise IO chip!\n");
			exit(1);
		}
	}

	~IOchip()
	{
		free(_Data);
		free((void*)_WaitResponse);
		free((void*)_DoneResponse);
	}

	volatile void WaitResponse(uint8_t addr)
	{
		if (_WaitResponse[addr])
		{
			while (!_DoneResponse[addr]);
			_DoneResponse[addr] = false;
		}
	}

	volatile void ShouldWaitResponse(uint8_t addr, bool b)
	{
		_WaitResponse[addr] = b;
	}
	
	volatile void DoneResponse(uint8_t addr, bool b = true)
	{
		_DoneResponse[addr] = b;
	}

	void _SetData(uint8_t* data, size_t size)
	{
		_Size = size;
		_Data = data;
	}

	void SetDataAtAddr(uint8_t addr, uint8_t val)
	{
		_Data[addr] = val;
	}

	volatile void StopWaiting()
	{
		memcpy((void*)_DoneResponse, (void*)_WaitResponse, 0xff);
	}

	uint8_t GetDataAtAddr(uint8_t addr)
	{
		return _Data[addr];
	}

	void CopyToMemory(uint8_t addr, uint8_t* values, uint8_t size)
	{
		memcpy(_Data + addr, values, size);
	}

	uint8_t* GetData()
	{
		return _Data;
	}

	uint8_t* GetDataAtAddrPointer(uint8_t addr)
	{
		return _Data + addr;
	}

	size_t GetSize()
	{
		return _Size;
	}
};