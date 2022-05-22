#include <stdio.h>

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

std::string parse(SourceFile* source)
{
	_Memory = (uint8_t*)calloc(0xffff, sizeof(uint8_t));

	if (_Memory == nullptr)
	{
		printf("Error allocating memory");
		exit(1);
	}

	bool firstLine = true;

	while (source->HasMore())
	{
		std::string word = source->Next();

		if (word == ".ORG")
		{
			if (!firstLine)
			{
				printf("Error: .ORG can only be in the first line!");
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
				printf("Expected a name for the label!"); //errors
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
					uint8_t* m = Instructions[i].ACTION(Instructions[i].bytes, source);
					if (m != nullptr)
					{
						printf("%d - %d\n", m[0], m[1]);

						for (int i = 0; i < Instructions[i].bytes; i++)
						{
							_Memory[currentAddr] = m[i];
							currentAddr++;
						}
					}
				}
			}

			if (!found)
			{
				printf("Instruction '%s' not found!", word.c_str());
				exit(1);
			}
		}
	}

	for (int i = 0; i < labels.size(); i++)
	{
		printf("%s: %d\n", labels.at(i).first.c_str(), labels.at(i).second);
	}

	return "";
}