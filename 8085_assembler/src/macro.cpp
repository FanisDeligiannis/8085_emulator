#include "macro.h"

#include "assembler.h"

#include "instructions.h"

Macro::Macro(std::string name, SourceFile* source)
	: Name(name)
{
	std::string word = source->Next(); // Should be "MACRO"
	_StartLine = source->GetLine();

	word = source->Next();
	while (source->GetLine() == _StartLine && word != "")
	{
		_Arguments.push_back(word);
		word = source->Next();
	}

	std::string r = source->GetLastWord();
	if (r == "ENDM")
	{
		_Source = new SourceFile("", "");
		return;
	}

	r += source->ReadRawUntil("ENDM");
	r += "\n";

	if (source->GetLastWord() != "ENDM")
	{
		_Source = new SourceFile("", "");
		Error("Expected ENDM to end the MACRO.", source);
		return;
	}

	_Source = new SourceFile("", r);
}

uint16_t Macro::Parse(SourceFile* source, uint8_t* memory, uint16_t currentAddr)
{
	_Source->_Equ.clear();
	Symbols.clear();
	labels.clear();

	for (int i = 0; i < source->_Equ.size(); i++)
	{
		_Source->_Equ.push_back(source->_Equ.at(i));
	}

	for (int i = 0; i < Assembler::GetLabels().size(); i++)
	{
		labels.push_back(Assembler::GetLabels().at(i));
	}

	_Source->SetLine(_StartLine);

	std::vector<std::string> passedArguments;

	int startLine = source->GetLine();
	std::string word = source->Next();

	while (source->GetLine() == startLine && word != "")
	{
		passedArguments.push_back(word);
		word = source->Next();
	}

	if (_Arguments.size() > passedArguments.size())
	{
		Error("Expected more arguments", _Source);
		return 0;
	}
	else if (_Arguments.size() < passedArguments.size())
	{
		Error("Expected fewer arguments", _Source);
		return 0;
	}
	else
	{
		for (int i = 0; i < _Arguments.size(); i++)
		{
			_Source->_Equ.push_back({ _Arguments.at(i), passedArguments.at(i)});
		}
	}

	return Assemble(_Source, memory, currentAddr, source);
}

uint16_t Macro::CalculateNewAddr(SourceFile* source, uint16_t currentAddr)
{
	_Source->ResetFile();
	_Source->_Equ.clear();
	Symbols.clear();
	labels.clear();

	for (int i = 0; i < source->_Equ.size(); i++)
	{
		_Source->_Equ.push_back(source->_Equ.at(i));
	}

	for (int i = 0; i < Assembler::GetLabels().size(); i++)
	{
		labels.push_back(Assembler::GetLabels().at(i));
	}

	_Source->SetLine(_StartLine);

	std::vector<std::string> passedArguments;

	int startLine = source->GetLine();
	std::string word = source->Next();

	while (source->GetLine() == startLine && word != "")
	{
		passedArguments.push_back(word);
		word = source->Next();
	}

	if (_Arguments.size() > passedArguments.size())
	{
		return currentAddr;
	}
	else if (_Arguments.size() < passedArguments.size())
	{
		return currentAddr;
	}
	else
	{
		for (int i = 0; i < _Arguments.size(); i++)
		{
			_Source->_Equ.push_back({ _Arguments.at(i), passedArguments.at(i) });
		}
	}

	return ScanForLabels(_Source, currentAddr, source);
}


uint16_t Macro::ScanForLabels(SourceFile* source, uint16_t currentAddr, SourceFile* ogSource)
{
	bool ended = false;

	source->SetLine(_StartLine);

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
						Error("Error in MACRO", ogSource);
					}
				}

				labels.push_back({ label, currentAddr - 1 });
			}
			else
			{
				Error("Expected a name for the label", source);
				Error("Error in MACRO", ogSource);
			}
		}
		else
		{
		}
	}

	return currentAddr;
}

uint16_t Macro::Assemble(SourceFile* source, uint8_t* _Memory, uint16_t currentAddr, SourceFile* ogSource)
{
	//Scan for labels
	source->ResetFile();
	ScanForLabels(source, currentAddr, ogSource);
	source->ResetFile();

	source->SetLine(_StartLine);

	//swap our assembler's labels temporarily.
	auto tempLabels = Assembler::GetLabels();
	Assembler::SetLabels(labels);

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
				if(_Memory != nullptr)
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
		else if (word[word.length() - 1] == ':') {} // Ignore labels, we have already scanned and added them.
		else if (word == "DB") //DB saves one or more bytes in current memory address and forward
		{
			std::string nextWord = source->Next(); //Read next word

			if (nextWord[0] == '\'') //If it starts with '  , it is a character.
			{
				if (nextWord.length() != 3)
				{
					Error("Expected ONE character and closing apostrophe", source);
					Error("Error in MACRO", ogSource);
				}

				if (nextWord[nextWord.length() - 1] != '\'') //And it also ends with '
				{
					Error("Expected closing apostrophe", source);
					Error("Error in MACRO", ogSource);
				}

				std::string numStr = nextWord.substr(1, nextWord.length() - 2);

				if(_Memory != nullptr)
					_Memory[currentAddr] = numStr[0];
				currentAddr++;
			}
			else if (nextWord[0] == '\"') // If it starts with ", it is a string and has to also end with "
			{
				if (nextWord.length() < 2) //Length could be anything
				{
					Error("Expected at least one character and closing double apostrophe", source);
					Error("Error in MACRO", ogSource);
				}

				if (nextWord[nextWord.length() - 1] != '\"')
				{
					Error("Expected closing double apostrophe", source);
					Error("Error in MACRO", ogSource);
				}

				std::string numStr = nextWord.substr(1, nextWord.length() - 2); //Whatever the DB contained, without the "

				for (int i = 0; i < numStr.length(); i++) //Add it all to memory.
				{
					if(_Memory != nullptr)
						_Memory[currentAddr] = numStr[i];
					currentAddr++;
				}
			}
			else //Otherwise, we expect an 8bit number.
			{
				if(_Memory != nullptr)
					_Memory[currentAddr] = StringToUInt8(nextWord, source);
				currentAddr++;
			}
		}
		else if (word == "DW") //DW get's a 16 bit number.
		{
			uint16_t addr = GetImmediate16(source);

			uint8_t HIGH = (addr >> 8) & 0xff;
			uint8_t LOW = addr & 0xff;

			//TODO: LITTLE/BIG endian??

			if(_Memory != nullptr)
				_Memory[currentAddr++] = LOW;
			currentAddr++;
			if(_Memory != nullptr)
				_Memory[currentAddr++] = HIGH;
			currentAddr++;
		}
		else
		{
			if (!found)
			{
				Error("Unexpected " + word, source);
				Error("Error in MACRO", ogSource);
			}
		}
	}

	Assembler::SetLabels(tempLabels);

	return currentAddr;
}