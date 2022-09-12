#include "assembler.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "instructions.h"
#include "Bootloader.h"
#include "macro.h"

namespace InternalAssembler
{

	uint16_t startingAddr = 0x0800;
	uint16_t currentAddr = 0x0800;

	Assembler::Assembly* currentAssembler;

	void Error(std::string err, std::shared_ptr<SourceFile> source)
	{
		//Print and add each error to a list
		printf("Error: Line %d, Character %d\n%s\n", source->GetLine(), source->GetCharCount(), err.c_str());
		currentAssembler->Errors.push_back({ source->GetLine(), err });
	}

	void Error(std::string err, int line)
	{
		//Print and add each error to a list
#ifdef _DEBUG
		printf("Error: Line %d, Character %d\n%s\n", line, 0, err.c_str());
#endif
		currentAssembler->Errors.push_back({ line, err });
	}

	void ParseIfDirective(std::shared_ptr<SourceFile> source, std::vector<IfExpr>& IfBuffer)
	{
		std::string word = source->GetLastWord();
		int ifLine = source->GetLine();

		if (word == "IF")
		{
			std::string first = source->Next();
			if (first == "")
			{
				Error("Expected expression after IF", ifLine);
				return;
			}
			std::string _operator = source->Next();
			if (_operator == "")
			{
				Error("Expected expression after IF", ifLine);
				return;
			}
			std::string other = source->Next();
			if (other == "")
			{
				Error("Expected expression after IF", ifLine);
				return;
			}

			if (isNumber(first) && isNumber(other))
			{
				uint16_t first_numeric = StringToUInt16(first, source);
				uint16_t other_numeric = StringToUInt16(other, source);
				if (_operator == "EQ")
				{
					IfBuffer.push_back({ first_numeric == other_numeric, false });
				}
				else if (_operator == "NEQ")
				{
					IfBuffer.push_back({ first_numeric != other_numeric, false });
				}
				else if (_operator == "LT")
				{
					IfBuffer.push_back({ first_numeric < other_numeric, false });
				}
				else if (_operator == "LTE")
				{
					IfBuffer.push_back({ first_numeric <= other_numeric, false });
				}
				else if (_operator == "GT")
				{
					IfBuffer.push_back({ first_numeric > other_numeric, false });
				}
				else if (_operator == "GTE")
				{
					IfBuffer.push_back({ first_numeric >= other_numeric, false });
				}
				else
				{
					Error("Unknown operator " + _operator + " between numbers", source);
				}
			}
			else if (!isNumber(first) && !isNumber(other))
			{
				if (_operator == "EQ")
				{
					IfBuffer.push_back({ first == other, false });
				}
				else if (_operator == "NEQ")
				{
					IfBuffer.push_back({ first != other, false });
				}
				else
				{
					Error("Unknown operator " + _operator + " between literals", source);
				}
			}
			else
			{
				if (_operator == "EQ")
				{
					IfBuffer.push_back({ false, false });
				}
				else if (_operator == "NEQ")
				{
					IfBuffer.push_back({ true, false });
				}
				//Error("Unable to compare literal and number.", source);
			}
		}
		else if (word == "ELSE")
		{
			if (IfBuffer.size() == 0)
			{
				Error("Unexpected ELSE; Expecting IF before ELSE", source);
			}
			else if (IfBuffer.at(IfBuffer.size() - 1).isElse)
			{
				Error("Unexpected ELSE; Expecting IF before ELSE", source);
			}
			else
			{
				bool res = true;
				for (int i = 0; i < IfBuffer.size(); i++)
				{
					if (IfBuffer.at(i).expression && !IfBuffer.at(i).isElse)
					{
						res = false;
						break;
					}
				}
				/*IfBuffer.at(IfBuffer.size() - 1).expression = res;
				IfBuffer.at(IfBuffer.size() - 1).isElse = true;*/
				IfBuffer.push_back({ res, true });
			}
		}
		else if (word == "ENDIF")
		{
			if (IfBuffer.size() == 0)
			{
				Error("Unexpected ENDIF; Expecting IF before ENDIF", source);
			}
			else
			{
				if (IfBuffer.at(IfBuffer.size() - 1).isElse)
					IfBuffer.pop_back();
				IfBuffer.pop_back();
			}
		}
	}

	bool isNumber(std::string str)
	{
		if (str.size() == 0)
			return false;

		if (str[0] == '-')
		{
			str = str.substr(1, str.length() - 1);
		}

		if (str[str.size() - 1] == 'H')
		{
			if (str.size() == 1) return false;
			for (int i = 0; i < str.size() - 1; i++)
			{
				if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'F')) continue;

				return false;
			}
		}
		else if (str[str.size() - 1] == 'B')
		{
			if (str.size() == 1) return false;
			for (int i = 0; i < str.size() - 1; i++)
			{
				if (str[i] == '1' || str[i] == '0') continue;

				return false;
			}
		}
		else
		{
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] >= '0' && str[i] <= '9') continue;

				return false;
			}
		}

		return true;
	}

	uint8_t StringToUInt8(std::string str, std::shared_ptr<SourceFile> source)
	{
		if (str.length() == 0)
		{
			return 0;
		}

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

			uint8_t result = *(uint8_t*)&num; // Convert last 8 bits to uint8_t using pointer casting

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
	uint16_t StringToUInt16(std::string str, std::shared_ptr<SourceFile> source, bool noerrors, bool* NaN)
	{
		if (str.length() == 0)
		{
			return 0;
		}

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
				if (!noerrors)
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
			if (!noerrors)
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

	//Parse the file first, looking for labels and calculating their location in memory. This is my "scanning" mode.
	//We do this so we can use a label before we declare it.
	//Otherwise, we wouldn't be able to use a label that's BELOW the code we're currently writing in a file.
	// Same for EQU and MACROs
	//And then actually parse the file.
	std::shared_ptr<uint8_t> parse(std::shared_ptr<SourceFile> source, Assembler::Assembly& result, bool scanning, bool bootloader)
	{
		if (!scanning && !bootloader)
		{

			result.Memory = std::shared_ptr<uint8_t>((uint8_t*)calloc(0xffff + 1, sizeof(uint8_t)), free);

			//If we failed to allocate memory.
			//TODO Probably should throw error instead of crashing...
			if (result.Memory == nullptr)
			{
				perror("Unable to allocate memory");
				exit(1);
			}

			currentAddr = startingAddr;

			std::shared_ptr<SourceFile> bl = std::make_shared<SourceFile>(Bootloader);
			parse(bl, result, false, true);

			for (int i = 0; i < bl->_Equ.size(); i++)
			{
				source->_Equ.push_back(bl->_Equ.at(i));
			}

			result.Symbols.clear();
		}

		currentAssembler = &result;

		//currentAddr = startingAddr;
		uint16_t addr = currentAddr;

		//Scan for labels
		if (!scanning)
		{
			parse(source, result, true, bootloader);
		}

		source->ResetFile();


		//Reset currentAddr after scanning for labels.
		currentAddr = addr;

		bool ended = false;

		while (source->HasMore() && !ended)
		{
			std::string word = source->Next(true);

			//-----------------------------------------------------------

			if (word == "") { continue; }

			else if (word == "IF" || word == "ELSE" || word == "ENDIF")
			{
				ParseIfDirective(source, result.IfBuffer);
				continue;
			}

			if (result.IfBuffer.size() == 0)
			{

			}
			else if (!result.IfBuffer.at(result.IfBuffer.size() - 1).expression)
			{
				continue;
			}

			//-----------------------------------------------------------

			bool found = false;
			int i = 0;
			while (i < 0xff && !found)
			{
				if (Instructions[i].OPERAND == word)
				{
					found = true;
					if (!scanning)
					{
						result.Symbols.push_back({ currentAddr, source->GetLine() }); //So we know which instruction corresponds to which line
						bool ret = Instructions[i].ACTION(Instructions[i].bytes, source, result.Memory.get() + currentAddr); // Not really using ret. . .
					}
					currentAddr += Instructions[i].bytes;
				}

				i++;
			}

			if (found) {}
			else if (word == "ORG")
			{
				//Read address
				std::string addrStr = source->Next();

				//Convert to uint16_t
				uint16_t addr = StringToUInt16(addrStr, source);

				//Make it the currentAddr
				currentAddr = addr;
			}
			else if (source->NextNoCursor() == "EQU")  //If the NEXT word is "EQU", but don't increment the cursor on SourceFile.
			{
				if (scanning)
				{
					std::string label = word; //Label of EQU is our current word.
					source->Next(); // We ignore next word, we know it's "EQU"
					std::string val = source->Next(); //Value of EQU is the 3rd word.

					if (val.empty())
					{
						Error("Expected a value after EQU", source);
					}

					source->_Equ.push_back({ label, val });
				}
				else
				{
					source->Next();
					source->Next();
				}
			}
			else if (source->NextNoCursor() == "MACRO")
			{
				if (scanning)
				{
					Macro* macro = new Macro(word, source);
					result.Macros.push_back(macro);
				}
				else
				{
					source->ReadRawUntil("ENDM");
				}
			}
			else if (word[word.length() - 1] == ':')
			{
				if (scanning)
				{
					if (word.length() > 1)
					{
						std::string label = word.substr(0, word.length() - 1);

						for (int i = 0; i < result.Labels.size(); i++)
						{
							if (result.Labels.at(i).first == label)
							{
								Error("Label " + label + " already exists", source);
							}
						}

						result.Labels.push_back({ label, currentAddr - 1 });
					}
					else
					{
						Error("Expected a name for the label", source);
					}
				}
			}
			else if (word == "END") // Not needed, but want to be fully compatible with microlab.
			{
				ended = true;
			}
			else if (word == "DB") //DB saves one or more bytes in current memory address and forward
			{
				if (scanning) continue; // Potential problems if someone has DB/DW randomly inside their code.

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

					result.Memory.get()[currentAddr++] = numStr[0];
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
						result.Memory.get()[currentAddr++] = numStr[i];
					}
				}
				else //Otherwise, we expect an 8bit number.
				{
					result.Memory.get()[currentAddr++] = StringToUInt8(nextWord, source);
				}
			}
			else if (word == "DW") //DW get's a 16 bit number.
			{
				if (scanning) continue;

				uint16_t addr = GetImmediate16(source);

				uint8_t HIGH = (addr >> 8) & 0xff;
				uint8_t LOW = addr & 0xff;

				//TODO: LITTLE/BIG endian??

				result.Memory.get()[currentAddr++] = LOW;
				result.Memory.get()[currentAddr++] = HIGH;
			}
			else
			{
				found = false;

				for (int i = 0; i < result.Macros.size(); i++)
				{
					if (result.Macros.at(i)->Name == word)
					{
						found = true;
						if (scanning)
						{
							currentAddr = result.Macros.at(i)->Parse(source, currentAddr, result, true);
						}
						else
						{
							currentAddr = result.Macros.at(i)->Parse(source, currentAddr, result);

							for (int j = 0; j < result.Macros.at(i)->GetSymbols().size(); j++)
							{
								result.Symbols.push_back(result.Macros.at(i)->GetSymbols().at(j)); // Get the Symbols from inside the MACRO.
							}
						}
					}
				}

				if (!found && !scanning)
				{
					Error("Unexpected " + word, source);
				}
			}
		}

		if (!scanning && !bootloader)
		{
			while (result.Macros.size() > 0)
			{
				delete result.Macros.at(0);
				result.Macros.erase(result.Macros.begin());
			}
		}

		return nullptr;
	}

}

std::shared_ptr<InternalAssembler::SourceFile> Assembler::ReadSourceFile(std::string fileName) // Helper function to convert FileName to SourceFile*
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

	std::shared_ptr<InternalAssembler::SourceFile> source = std::make_shared<InternalAssembler::SourceFile>(file);

	return source;
}

std::shared_ptr<uint8_t> Assembler::GetAssembledMemory(std::shared_ptr<InternalAssembler::SourceFile> source, Assembler::Assembly& result) //Helper function to convert SourceFile* to assembled memory
{
	return parse(source, result);
}

std::shared_ptr<uint8_t> Assembler::GetAssembledMemory(std::string code, Assembler::Assembly& result) //Helper function to convert the code to assembled memory
{
	std::shared_ptr<InternalAssembler::SourceFile> source = std::make_shared<InternalAssembler::SourceFile>(code);

	return InternalAssembler::parse(source, result);
}