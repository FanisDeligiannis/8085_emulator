#include "macro.h"

#include "assembler.h"

#include "instructions.h"

namespace InternalAssembler
{
	Macro::Macro(std::string name, std::shared_ptr<SourceFile> source)
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
			_Source = std::make_shared<SourceFile>("");
			return;
		}

		r += source->ReadRawUntil("ENDM");
		r += "\n";

		if (source->GetLastWord() != "ENDM")
		{
			_Source = std::make_shared<SourceFile>("");

			Error("Expected ENDM to end the MACRO.", source);
			return;
		}

		_Source = std::make_shared<SourceFile>(r);
	}

	uint16_t Macro::Parse(std::shared_ptr<SourceFile> source, uint16_t currentAddr, Assembler::Assembly& result, bool scanning)
	{
		_Source->ResetFile();
		_Source->_Equ.clear();
		Symbols.clear();
		labels.clear();

		for (int i = 0; i < source->_Equ.size(); i++)
		{
			_Source->_Equ.push_back(source->_Equ.at(i));
		}

		for (int i = 0; i < result.Labels.size(); i++)
		{
			labels.push_back(result.Labels.at(i));
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
			if (!scanning)
			{
				//Error("Expected more arguments", _Source);
				Error("Expected more arguments", source);
			}
			return currentAddr;
		}
		else if (_Arguments.size() < passedArguments.size())
		{
			if (!scanning)
				//Error("Expected fewer arguments", _Source);
				Error("Expected fewer arguments", source);
			return currentAddr;
		}
		else
		{
			for (int i = 0; i < _Arguments.size(); i++)
			{
				_Source->_Equ.push_back({ _Arguments.at(i), passedArguments.at(i) });
			}
		}

		return Assemble(_Source, result, currentAddr, source, scanning);
	}

	uint16_t Macro::Assemble(std::shared_ptr<SourceFile> source, Assembler::Assembly& result, uint16_t currentAddr, std::shared_ptr<SourceFile> ogSource, bool scanning)
	{
		//Scan for labels
		source->ResetFile();
		if (!scanning)
			Assemble(source, result, currentAddr, ogSource, true);
		source->ResetFile();

		source->SetLine(_StartLine);

		//swap our assembler's labels temporarily.
		auto tempLabels = result.Labels;
		if (!scanning)
		{
			result.Labels = labels;
		}

		_IfBuffer.clear();

		while (source->HasMore())
		{
			std::string word = source->Next(true);

			if (word == "") { continue; }

			else if (word == "IF" || word == "ELSE" || word == "ENDIF")
			{
				ParseIfDirective(source, _IfBuffer);
				continue;
			}

			if (_IfBuffer.size() == 0)
			{

			}
			else if (!_IfBuffer.at(_IfBuffer.size() - 1).expression)
			{
				continue;
			}

			bool found = false;
			int i = 0;
			while (i < 0xff && !found)
			{
				if (Instructions[i].OPERAND == word)
				{
					found = true;
					if (!scanning)
					{
						Symbols.push_back({ currentAddr, source->GetLine() }); //So we know which instruction corresponds to which line

						if (result.Memory != nullptr)
							bool ret = Instructions[i].ACTION(Instructions[i].bytes, source, result.Memory.get() + currentAddr); // Not really using ret. . .
					}
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
			else if (source->NextNoCursor() == "EQU")  //If the NEXT word is "EQU", but don't increment the cursor on SourceFile.
			{
				if (scanning)
				{
					std::string label = word; //Label of EQU is our current word.
					source->Next(); // We ignore next word, we know it's "EQU"
					std::string val = source->Next(); //Value of EQU is the 3rd word.

					source->_Equ.push_back({ label, val });
				}
				else
				{
					source->Next();
					source->Next();
				}
			}
			else if (word[word.length() - 1] == ':')
			{
				// Ignore labels if we're not scanning, add them if we are.
				if (scanning)
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
			}
			else if (word == "DB") //DB saves one or more bytes in current memory address and forward
			{
				if (scanning) continue;

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

					result.Memory.get()[currentAddr++] = numStr[0];
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
								Symbols.push_back(result.Macros.at(i)->GetSymbols().at(j)); // Get the Symbols from inside the MACRO.
							}
						}
					}
				}

				if (!found && !scanning)
				{
					Error("Unexpected " + word, source);
					Error("Error in MACRO", ogSource);
				}
			}
		}

		if (!scanning)
			result.Labels = tempLabels;

		return currentAddr;
	}
}