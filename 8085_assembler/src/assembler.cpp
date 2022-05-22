#include <stdio.h>
#include <string>
#include <format>

#include "assembler.h"
#include "instructions.h"

uint8_t* _Memory;

uint16_t startingAddr = 0x8000;
uint16_t currentAddr = 0x8000;

std::vector < std::pair<std::string, uint16_t> > labels;

std::vector<std::pair<std::string, uint16_t>> GetLabels()
{
	return labels;
}

uint8_t StringToUInt8(std::string str)
{
	if (str[str.length() - 1] == 'H')
	{
		std::string numStr = str.substr(0, str.length() - 1);

		unsigned int num = std::stoul(numStr, nullptr, 16);

		uint8_t result = *(uint8_t*) &num;
	
		return result;
	}

	if (str[str.length() - 1] == 'B')
	{
		std::string numStr = str.substr(0, str.length() - 1);

		unsigned int num = std::stoul(numStr, nullptr, 2);

		uint8_t result = *(uint8_t*)&num;

		return result;
	}

	unsigned int num = std::stoul(str, nullptr, 10);

	uint8_t result = *(uint8_t*)&num;

	return result;

}

uint16_t StringToUInt16(std::string str)
{
	if (str[str.length() - 1] == 'H')
	{
		std::string numStr = str.substr(0, str.length() - 1);

		unsigned int num = std::stoul(numStr, nullptr, 16);

		uint16_t result = *(uint16_t*)&num;

		return result;
	}

	if (str[str.length() - 1] == 'B')
	{
		std::string numStr = str.substr(0, str.length() - 1);

		unsigned int num = std::stoul(numStr, nullptr, 2);

		uint16_t result = *(uint16_t*)&num;

		return result;
	}

	unsigned int num = std::stoul(str, nullptr, 10);

	uint16_t result = *(uint16_t*)&num;

	return result;
}

std::string IntToHex(int num, int c)
{
	if (c==2)
		return std::format("{:#04X}", num);
	else if(c==4)
		return std::format("{:#06X}", num);
	return std::format("{:#06X}", num);
}

uint8_t* parse(SourceFile* source)
{
	_Memory = (uint8_t*)calloc(0xffff, sizeof(uint8_t));

	if (_Memory == nullptr)
	{
		perror("Unable to allocate memory");
		exit(1);
	}

	bool firstLine = true;

	while (source->HasMore())
	{
		std::string word = source->Next(true);

		if (word == ".ORG")
		{
			if (!firstLine)
			{
				Error(".ORG should be the first line.", source);
				exit(1);
			}

			firstLine = false;

			std::string addrStr = source->Next();

			//TODO: ERROR CHECK

			uint16_t addr = StringToUInt16(addrStr);

			startingAddr = addr;
			currentAddr = addr;
		}
		else if (word[word.length() - 1] == ':')
		{
			firstLine = false;

			if (word.length() > 1)
			{
				std::string label = word.substr(0, word.length() - 1);
				labels.push_back({ label, currentAddr });
			}
			else
			{
				Error("Expected a name for the label", source);
				exit(1);
			}
		}
		else
		{
			bool found = false;
			for (int i = 0; i < 0xff; i++)
			{
				if (Instructions[i].OPERAND == word)
				{
					found = true;
					bool ret = Instructions[i].ACTION(Instructions[i].bytes, source, _Memory+currentAddr);
					currentAddr += Instructions[i].bytes;
				}
			}

			if (!found)
			{
				Error("Instruction " + word + " not found!", source);
				exit(1);
			}
		}
	}

	//int i = startingAddr;
	//int j = 0;

	//while (_Memory[i] != 0x76 && j < 20)
	//{
	//	printf("%s: %s\n", IntToHex(i).c_str(), IntToHex(_Memory[i], 2).c_str());
	//	i++;
	//	j++;
	//}

	//for (int i = 0; i < labels.size(); i++)
	//{
	//	printf("%s: %d\n", labels.at(i).first.c_str(), labels.at(i).second);
	//}

	return _Memory;
}