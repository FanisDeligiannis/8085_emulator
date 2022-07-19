#pragma once

#include "Windows/Window.h"

#include "imgui.h"
#include "Simulation.h"

class RegistersWindow : public Window
{
private:
	uint8_t A = 0;
	uint8_t B = 0;
	uint8_t C = 0;
	uint8_t D = 0;
	uint8_t E = 0;
	uint8_t H = 0;
	uint8_t L = 0;
	uint8_t M = 0;

	uint16_t PC = 0;
	uint16_t SP = 0;

	uint8_t Sign_flag = 0;
	uint8_t Zero_flag = 0;
	uint8_t Parity_flag = 0;
	uint8_t Carry_flag = 0;

	bool updating = false;

private:
	//Stupid IntToHex function.
	inline std::string IntToHex(int num, const std::string prefix = "", int size = -1)
	{
		std::string result;

		int temp = num;

		bool done = false;

		while (temp > 0)
		{
			int remainder = temp % 16;
			temp = temp / 16;

			switch (remainder)
			{
			case 0:
				result = "0" + result;
				break;
			case 1:
				result = "1" + result;
				break;
			case 2:
				result = "2" + result;
				break;
			case 3:
				result = "3" + result;
				break;
			case 4:
				result = "4" + result;
				break;
			case 5:
				result = "5" + result;
				break;
			case 6:
				result = "6" + result;
				break;
			case 7:
				result = "7" + result;
				break;
			case 8:
				result = "8" + result;
				break;
			case 9:
				result = "9" + result;
				break;
			case 10:
				result = "A" + result;
				break;
			case 11:
				result = "B" + result;
				break;
			case 12:
				result = "C" + result;
				break;
			case 13:
				result = "D" + result;
				break;
			case 14:
				result = "E" + result;
				break;
			case 15:
				result = "F" + result;
				break;
			}
		}

		if (size >= 0)
		{
			if (result.size() < size)
			{
				while (result.size() < size)
				{
					result = "0" + result;
				}
			}
			else if (result.size() > size)
			{
				result = result.substr(result.size() - size, size);
			}
		}

		return (prefix + result);
	}

public:
	static RegistersWindow* Instance;

public:
	void Init() override 
	{ 
		Instance = this; 
	}

	void UpdateBuffers(bool force = false);

	void Render() override;
};