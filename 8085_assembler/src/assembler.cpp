#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "assembler.h"
#include "instructions.h"
#include "Bootloader.h"
#include "macro.h"

//TODO: MACRO

uint8_t* _Memory;

uint16_t startingAddr = 0x0800;
uint16_t currentAddr = 0x0800;

std::vector < std::pair<std::string, uint16_t> > labels;
std::vector<std::pair<int, std::string>> Errors;
std::vector<std::pair<uint16_t, int>> Symbols;
std::vector<Macro*> Macros;

std::vector<std::pair<std::string, uint16_t>> Assembler::GetLabels()
{
	return labels;
}

void Assembler::SetLabels(std::vector<std::pair<std::string, uint16_t>> newLabels)
{
	labels = newLabels;
}


std::vector<std::pair<int, std::string>> Assembler::GetErrors()
{
	return Errors;
}

std::vector<std::pair<uint16_t, int>> Assembler::GetSymbols()
{
	return Symbols;
}

bool Assembler::HasSymbols(uint16_t addr)
{
	for (int i = 0; i < Symbols.size(); i++)
	{
		if (Symbols.at(i).first == addr)
		{
			return true;
		}
	}

	return false;
}

void Error(std::string err, SourceFile* source)
{
	//Print and add each error to a list
	printf("Error: Line %d, Character %d\n%s\n", source->GetLine(), source->GetCharCount(), err.c_str());
	Errors.push_back({ source->GetLine(), err });
}

uint8_t StringToUInt8(std::string str, SourceFile* source)
{
	if (str[str.length() - 1] == 'H') // if it ends with H, treat it as HEX
	{
		std::string numStr = str.substr(0, str.length() - 1);

		unsigned int num;

		try // So that we don't crash.
		{
			num = std::stoul(numStr, nullptr, 16);
		}
		catch (...)
		{
			Error("Expected hex number, got: " + str, source);
			return 0;
		}

		uint8_t result = *(uint8_t*) &num; // Convert last 8 bits to uint8_t using pointer casting
	
		return result;
	}
	else if (str[str.length() - 1] == 'B') // If it ends with B, treat it as binary
	{
		std::string numStr = str.substr(0, str.length() - 1);
		unsigned int num;

		try
		{
			num = std::stoul(numStr, nullptr, 2);
		}
		catch (...)
		{
			Error("Expected binary number, got: " + str, source);
			return 0;
		}

		uint8_t result = *(uint8_t*)&num;

		return result;
	}
	else if (str[0] == '\'' && str[str.length() - 1] == '\'' && str.length() == 3) // If it starts and ends with ', and its length is 3, treat it as char
	{
		return str[1];
	}

	//Otherwise, treat it as dec number

	unsigned int num;

	try
	{
		num = std::stoul(str, nullptr, 10);
	}
	catch (...)
	{
		Error("Expected dec number, got: " + str, source);
		return 0;
	}

	uint8_t result = *(uint8_t*)&num;
	
	return result;

}

//Same exact thing, but with uint16_t number.
uint16_t StringToUInt16(std::string str, SourceFile* source, bool noerrors, bool *NaN) 
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
			if (!noerrors)
				Error("Expected hex number, got: " + str, source);
			else if (NaN != nullptr)
				*NaN = true;
			return 0;
		}

		if (NaN != nullptr)
			*NaN = false;

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
			if(!noerrors)
				Error("Expected binary number, got: " + str, source);
			else if (NaN != nullptr)
				*NaN = true;
			return 0;
		}

		if (NaN != nullptr)
			*NaN = false;

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
		if(!noerrors)
			Error("Expected dec number, got: " + str, source);
		else if (NaN != nullptr)
			*NaN = true;
		return 0;
	}

	if (NaN != nullptr)
		*NaN = false;

	uint16_t result = *(uint16_t*)&num;

	return result;
}

//Parse the file first, looking for labels and calculating their location in memory.
//We do this so we can use a label before we declare it.
//Otherwise, we wouldn't be able to use a label that's BELOW the code we're currently writing in a file.
void ScanForLabels(SourceFile* source)
{
	bool ended = false;

	while (source->HasMore() && !ended)
	{
		std::string word = source->Next(true);
		
		if (word == "") { continue; }


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

		if (found)
		{

		}
		else if (word == "ORG")
		{
			std::string addrStr = source->Next();

			uint16_t addr = StringToUInt16(addrStr, source);

			currentAddr = addr;
		}
		else if (source->NextNoCursor() == "EQU") //If the NEXT word is "EQU", but don't increment the cursor on SourceFile.
		{

			std::string label = word; //Label of EQU is our current word.
			source->Next(); // We ignore next word, we know it's "EQU"
			std::string val = source->Next(); //Value of EQU is the 3rd word.

			source->_Equ.push_back({ label, val });
		}
		else if (source->NextNoCursor() == "MACRO")
		{
			Macro* macro = new Macro(word, source);
			Macros.push_back(macro);
		}
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
			}
		}
		else
		{
			found = false;

			for (int i = 0; i < Macros.size(); i++)
			{
				if (Macros.at(i)->Name == word)
				{
					found = true;
					currentAddr = Macros.at(i)->CalculateNewAddr(source, currentAddr);
				}
			}
		}
	}
}

//Actually parse the file.
uint8_t* parse(SourceFile* source)
{
	_Memory = (uint8_t*)calloc(0xffff + 1, sizeof(uint8_t));

	//If we failed to allocate memory.
	//Probably should throw error instead of crashing...
	//TODO
	if (_Memory == nullptr)
	{
		perror("Unable to allocate memory");
		exit(1);
	}

	//Clear them all.
	labels.clear();
	Errors.clear();
	Symbols.clear();

	currentAddr = startingAddr;

	//Scan for labels
	ScanForLabels(source);
	source->ResetFile();
	source->ResetBootloader();
	

	//Reset currentAddr after "ScanForLabels" is called.
	currentAddr = startingAddr;

	bool ended = false;

	while (source->HasMore() && !ended)
	{
		std::string word = source->Next(true);

		if (word == "") { continue; }


		bool found = false;
		int i = 0;
		while (i < 0xff && !found)
		{
			if (Instructions[i].OPERAND == word)
			{
				found = true;
				if (source->IsBootloaderDone())
					Symbols.push_back({ currentAddr, source->GetLine() }); //So we know which instruction corresponds to which line
				bool ret = Instructions[i].ACTION(Instructions[i].bytes, source, _Memory + currentAddr); // Not really using ret. . .
				currentAddr += Instructions[i].bytes;
			}

			i++;
		}

		if (found) {}
		else if (word == "ORG") //Could have it as a 0 byte "instruction" instead
		{
			//Read address
			std::string addrStr = source->Next();

			//Convert to uint16_t
			uint16_t addr = StringToUInt16(addrStr, source);

			//Make it the currentAddr
			currentAddr = addr;
		}
		else if (source->NextNoCursor() == "EQU") { source->Next(); source->Next(); }
		else if (source->NextNoCursor() == "MACRO") { source->ReadRawUntil("ENDM"); }
		else if (word[word.length() - 1] == ':') {} // Ignore labels, we have already scanned and added them.
		else if (word == "END") // Not needed, but want to be fully compatible with microlab.
		{
			ended = true;
		}
		else if (word == "DB") //DB saves one or more bytes in current memory address and forward
		{
			std::string nextWord = source->Next(); //Read next word

			if (nextWord[0] == '\'') //If it starts with '  , it is a character.
			{
				if (nextWord.length() != 3)
				{
					Error("Expected ONE character and closing apostrophe", source);
				}

				if (nextWord[nextWord.length() - 1] != '\'') //And it also ends with '
				{
					Error("Expected closing apostrophe", source);
				}

				std::string numStr = nextWord.substr(1, nextWord.length() - 2);

				_Memory[currentAddr++] = numStr[0];
			}
			else if (nextWord[0] == '\"') // If it starts with ", it is a string and has to also end with "
			{
				if (nextWord.length() < 2) //Length could be anything
				{
					Error("Expected at least one character and closing double apostrophe", source);
				}

				if (nextWord[nextWord.length() - 1] != '\"')
				{
					Error("Expected closing double apostrophe", source);
				}

				std::string numStr = nextWord.substr(1, nextWord.length() - 2); //Whatever the DB contained, without the "

				for (int i = 0; i < numStr.length(); i++) //Add it all to memory.
				{
					_Memory[currentAddr++] = numStr[i];
				}
			}
			else //Otherwise, we expect an 8bit number.
			{
				_Memory[currentAddr++] = StringToUInt8(nextWord, source);
			}
		}
		else if (word == "DW") //DW get's a 16 bit number.
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
			found = false;

			for (int i = 0; i < Macros.size(); i++)
			{
				if (Macros.at(i)->Name == word)
				{
					found = true;
					currentAddr = Macros.at(i)->Parse(source, _Memory, currentAddr);

					if (source->IsBootloaderDone())
					{
						for (int j = 0; j < Macros.at(i)->GetSymbols().size(); j++)
						{
							Symbols.push_back(Macros.at(i)->GetSymbols().at(j)); // Get the Symbols from inside the MACRO.
						}
					}
				}
			}

			if (!found)
			{
				Error("Unexpected " + word, source);
			}
		}
	}

	while(Macros.size() > 0)
	{
		delete Macros.at(0);
		Macros.erase(Macros.begin());
	}

	return _Memory;
}

SourceFile* Assembler::ReadSourceFile(std::string fileName) // Helper function to convert FileName to SourceFile*
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

	SourceFile* source = new SourceFile(Bootloader, file);

	return source;
}

uint8_t* Assembler::GetAssembledMemory(SourceFile* source) //Helper function to convert SourceFile* to assembled memory
{
	return parse(source);
}

uint8_t* Assembler::GetAssembledMemory(std::string code) //Helper function to convert the code to assembled memory
{
	SourceFile* source = new SourceFile(Bootloader, code);

	return parse(source);
}

uint8_t* Assembler::GetAssembledMemory(char* file) //Helper function to convert FileName to assembled memory
{
	SourceFile* source = ReadSourceFile(file);
	uint8_t* ret = parse(source);
	
	delete source;
	
	return ret;
}

//Same helper functions but they save it as a .bin file.
//Not really useful, may get some use out of it in the future or will remove it.
//Was used when testing in the very very early stages, probably should be removed.

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

