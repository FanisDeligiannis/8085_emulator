#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "assembler.h"
#include "instructions.h"

//TODO: MACRO

uint8_t* _Memory;

uint16_t startingAddr = 0x0800;
uint16_t currentAddr = 0x0800;

std::vector < std::pair<std::string, uint16_t> > labels;

std::vector<std::pair<std::string, uint16_t>> GetLabels()
{
	return labels;
}

void Error(std::string err, SourceFile* source)
{
	printf("Error: Line %d, Character %d\n%s\n", source->GetLine(), source->GetCharCount(), err.c_str());

	free(_Memory);
	delete source;

	exit(1);
}

uint8_t StringToUInt8(std::string str)
{
	if (str[str.length() - 1] == 'H')
	{
		std::string numStr = str.substr(0, str.length() - 1);

		unsigned int num;

		try
		{
			num = std::stoul(numStr, nullptr, 16);
		}
		catch (...)
		{
			printf("Error: Expected number, got: %s", str.c_str());
			exit(1);
		}

		uint8_t result = *(uint8_t*) &num;
	
		return result;
	}
	else if (str[str.length() - 1] == 'B')
	{
		std::string numStr = str.substr(0, str.length() - 1);
		unsigned int num;

		try
		{
			num = std::stoul(numStr, nullptr, 2);
		}
		catch (...)
		{
			printf("Error: Expected number, got: %s", str.c_str());
			exit(1);
		}

		uint8_t result = *(uint8_t*)&num;

		return result;
	}
	
	unsigned int num;

	try
	{
		num = std::stoul(str, nullptr, 10);
	}
	catch (...)
	{
		printf("Error: Expected number, got: %s", str.c_str());
		exit(1);
	}

	uint8_t result = *(uint8_t*)&num;

	return result;

}

uint16_t StringToUInt16(std::string str)
{
	if (str[str.length() - 1] == 'H')
	{
		std::string numStr = str.substr(0, str.length() - 1);
		unsigned int num;

		try
		{
			num = std::stoul(numStr, nullptr, 16);
		}
		catch (...)
		{
			printf("Error: Expected number, got: %s", str.c_str());
			exit(1);
		}

		uint16_t result = *(uint16_t*)&num;

		return result;
	}

	if (str[str.length() - 1] == 'B')
	{
		std::string numStr = str.substr(0, str.length() - 1);
		unsigned int num;

		try
		{
			num = std::stoul(numStr, nullptr, 2);
		}
		catch (...)
		{
			printf("Error: Expected number, got: %s", str.c_str());
			exit(1);
		}

		uint16_t result = *(uint16_t*)&num;

		return result;
	}

	unsigned int num;

	try
	{
		num = std::stoul(str, nullptr, 10);
	}
	catch (...)
	{
		printf("Error: Expected number, got: %s", str.c_str());
		exit(1);
	}

	uint16_t result = *(uint16_t*)&num;

	return result;
}

std::string IntToHex(int num, int c)
{
	//if (c==2)
	//	return std::format("{:#04X}", num);
	//else if(c==4)
	//	return std::format("{:#06X}", num);
	//return std::format("{:#06X}", num);
	return "";
}

void ScanForLabels(SourceFile* source)
{
	bool ended = false;

	while (source->HasMore() && !ended)
	{
		std::string word = source->Next(true);

		if (word == "ORG")
		{
			std::string addrStr = source->Next();

			//TODO: ERROR CHECK

			uint16_t addr = StringToUInt16(addrStr);

			currentAddr = addr;
		}
		else if (word == "EQU") {}
		else if (word == "END")
		{
			ended = true;
		}
		else if (word == "DB") {}
		else if (word == "DW") {}
		else if (word[word.length() - 1] == ':')
		{
			if (word.length() > 1)
			{
				std::string label = word.substr(0, word.length() - 1);

				for (int i = 0; i < labels.size(); i++)
				{
					if (labels.at(i).first == label)
					{
						Error("Label " + label + " already exists", source);
					}
				}

				labels.push_back({ label, currentAddr-1 });
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
			int i = 0;
			
			while (i < 0xff && !found)
			{
				if (Instructions[i].OPERAND == word)
				{
					found = true;
					currentAddr += Instructions[i].bytes;
				}
				i++;
			}
		}
	}
}

uint8_t* parse(SourceFile* source)
{
	_Memory = (uint8_t*)calloc(0xffff, sizeof(uint8_t));

	source->_Equ.push_back({"CODE", "0800H"});


	if (_Memory == nullptr)
	{
		perror("Unable to allocate memory");
		exit(1);
	}

	ScanForLabels(source);
	source->ResetFile();

	currentAddr = startingAddr;

	bool ended = false;

	while (source->HasMore() && !ended)
	{
		std::string word = source->Next(true);

		if (word == "ORG")
		{
			std::string addrStr = source->Next();

			//TODO: ERROR CHECK

			uint16_t addr = StringToUInt16(addrStr);

			currentAddr = addr;
		}
		else if (source->NextNoCursor() == "EQU")
		{

			std::string label = word;
			source->Next();
			std::string val = source->Next();

			source->_Equ.push_back({ label, val });
		}
		else if (word[word.length() - 1] == ':') {}
		else if (word == "END")
		{
			ended = true;
		}
		else if (word == "DB")
		{
			std::string nextWord = source->Next();

			if (nextWord[0] == '\'')
			{
				if (nextWord.length() != 3)
				{
					Error("Expected ONE character and closing apostrophe", source);
				}

				if (nextWord[nextWord.length() - 1] != '\'')
				{
					Error("Expected closing apostrophe", source);
				}

				std::string numStr = nextWord.substr(1, nextWord.length() - 2);

				_Memory[currentAddr++] = numStr[0];
			}
			else if (nextWord[0] == '\"')
			{
				if (nextWord.length() < 2)
				{
					Error("Expected closing double apostrophe", source);
				}

				if (nextWord[nextWord.length() - 1] != '\"')
				{
					Error("Expected closing double apostrophe", source);
				}

				std::string numStr = nextWord.substr(1, nextWord.length() - 2);

				for (int i = 0; i < numStr.length(); i++)
				{
					_Memory[currentAddr++] = numStr[i];
				}
			}
			else
			{
				_Memory[currentAddr++] = StringToUInt8(nextWord);
			}
		}
		else if (word == "DW")
		{
			uint16_t addr = GetImmediate16(source);

			uint8_t HIGH = (addr >> 8) & 0xff;
			uint8_t LOW = addr & 0xff;
			
			//TODO: LITTLE/BIG endian??

			_Memory[currentAddr++] = LOW;
			_Memory[currentAddr++] = HIGH;
		}
		else
		{
			bool found = false;
			int i = 0;
			while (i < 0xff && !found)
			{
				if (Instructions[i].OPERAND == word)
				{
					found = true;
					bool ret = Instructions[i].ACTION(Instructions[i].bytes, source, _Memory + currentAddr);
					currentAddr += Instructions[i].bytes;
				}

				i++;
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
	//	std::cout << labels.at(i).first << ": 0x" << std::hex << labels.at(i).second << std::endl;
	//}

	return _Memory;
}

SourceFile* Assembler::ReadSourceFile(std::string fileName)
{
	std::string file;
	std::string line;

	std::ifstream inFile(fileName);

	while (std::getline(inFile, line)) {
		for (int i = 0; i < line.length(); i++)
		{
			line[i] = std::toupper(line[i]);
		}
		file += line + '\n';
	}

	file = file.substr(0, file.length() - 1);

	inFile.close();

	SourceFile* source = new SourceFile(file);

	return source;
}

uint8_t* Assembler::GetAssembledMemory(SourceFile* source)
{
	return parse(source);
}

uint8_t* Assembler::GetAssembledMemory(std::string file)
{
	SourceFile* source = ReadSourceFile(file);
	uint8_t* ret = parse(source);
	
	delete source;
	
	return ret;
}

void Assembler::SaveAssembledMemory(uint8_t* memory, std::string outFileName)
{
	std::ofstream outFile;

	outFile.open(outFileName.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

	for (int i = 0; i < 0xffff; i++)
	{
		outFile << std::hex << memory[i];
	}

	outFile.close();
}

void Assembler::SaveAssembledMemory(SourceFile* source, std::string outFileName)
{
	uint8_t* Memory;
	Memory = GetAssembledMemory(source);	

	SaveAssembledMemory(Memory, outFileName);

	free(Memory);
}

void Assembler::SaveAssembledMemory(std::string inFileName, std::string outFileName)
{
	SourceFile* source = ReadSourceFile(inFileName);
	SaveAssembledMemory(source, outFileName);
	delete source;
}

