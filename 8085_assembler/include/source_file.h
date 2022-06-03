#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

//Parsing of text / file.

class SourceFile
{
private:
	std::string _Bootloader; // Our bootloader
	std::string _Source; // Our program
	std::string _LastWord; // Last word we read.
	int _Cursor = 0; // Current cursor in the text.

	bool _BootloaderDone = false; 

	bool _HasMore = true; // Is there more text?

	int _LineCount = 1;
	int _CharCount = 1;

	int _PrevLineCount = 1;
	int _PrevCharCount = 1;

public:
	std::vector < std::pair<std::string, std::string> > _Equ; // A list of all "EQU" defines of our program.

public:
	SourceFile(std::string bootloader, std::string source)
		: _Bootloader(bootloader), _Source(source), _LastWord("") {}

	inline bool HasMore()
	{
		return _HasMore;
	}

	inline int GetLine()
	{
		return _PrevLineCount;
	}

	inline int GetCharCount()
	{
		return _PrevCharCount;
	}

	inline void ResetFile()
	{
		_Cursor = 0;

		_HasMore = true;

		_LineCount = 1;
		_CharCount = 1;

		_PrevLineCount = 1;
		_PrevCharCount = 1;
	}

	inline void ResetBootloader()
	{
		_BootloaderDone = false;
	}

	inline bool IsBootloaderDone()
	{
		return _BootloaderDone;
	}

	inline void SetFileCursor(int cursor)
	{
		_Cursor = cursor;
	}

	inline std::string NextNoCursor(bool ignore_newline_at_start = false)
	{
		if (!_BootloaderDone) //We start reading from bootloader.
		{
			return NextInternal(_Bootloader, ignore_newline_at_start, true);
		}
		else //If we have nothing else to read on bootloader, we read from user's file.
		{
			return NextInternal(_Source, ignore_newline_at_start, true);
		}
	}


	inline std::string Next(bool ignore_newline_at_start = false)
	{
		if (!_BootloaderDone)
		{
			std::string ret = NextInternal(_Bootloader, ignore_newline_at_start);

			if (!_HasMore) //Means bootloader is done. Reset pointers to start, but not bootloader.
			{
				_HasMore = true;
				_BootloaderDone = true;

				ResetFile();
			}

			return ret;
		}
		else
		{
			return NextInternal(_Source, ignore_newline_at_start);
		}
	}

	inline std::string GetLastWord()
	{
		return _LastWord;
	}

private:

	inline std::string NextInternal(std::string str, bool ignore_newline_at_start, bool no_cursor = false)
	{
		std::string word = "";

		int cc = _CharCount; //Buffers
		int lc = _LineCount;
		int c = _Cursor;
		bool hm = _HasMore;

		int i = _Cursor;

		bool done = false;

		while (i < str.length() && !done)
		{
			if (word.length() == 0)
			{
				if (str[i] == ';') // Means it's a comment. Ignore until we find \n
				{
					while (i < str.length() && str[i] != '\n') //Ignore all until \n but increase counter i.
					{
						i++;
					}
				}

				if (str[i] == ' ' || str[i] == '\r' || str[i] == '\t' || str[i] == ',') // ignore these characters
											//\r needed because CRLF
				{
					_CharCount++;
				}
				else if (str[i] == '\"') // Means it's a string. Mostly used with "DB"
				{
					word += '\"';

					i++;

					//We get ALL characters until \n or \".
					//That means a string may contain spaces, commas and everything else, 
					//But it may only be one line.

					//TODO: (Probably should implement escape character)
					while (i < str.length() && str[i] != '\n' && str[i] != '\"') 
					{
						word += str[i];
						i++;
						_CharCount++;
					}

					if (str[i] == '\n')
					{
						i--;
					}

					if (str[i] == '\"')
						word += '\"';
				}
				else if (str[i] == '\n')
				{
					_LineCount++;
					_CharCount = 0;

					if (!ignore_newline_at_start) // Ignore empty lines when it's at the beginning of a word
					{
						done = true;
					}
				}
				else
				{
					_CharCount++;
					word += str[i];
				}
			}
			else
			{
				//Ending delimeters.
				//A word ends with a space, a tab, a comma, or a newline.
				if (str[i] == ' ' || str[i] == '\t' || str[i] == ',' || str[i] == ';')
				{
					_PrevLineCount = _LineCount;
					_PrevCharCount = _CharCount;

					_CharCount++;

					done = true;
				}
				else if (str[i] == '\r') {}
				else if (str[i] == '\n')
				{
					_PrevLineCount = _LineCount;
					_PrevCharCount = _CharCount;

					//_LineCount++;
					//_CharCount = 0;
					i--;

					done = true;
				}
				else // if none of the above, just add the letter to the word.
				{
					_CharCount++;
					word += str[i];
				}
			}

			i++;
		}

		_Cursor = i;
		if (_Cursor == str.length()) // Cursor at the end.
		{
			_HasMore = false;
		}

		std::transform(word.begin(), word.end(), word.begin(), ::toupper); //Convert all letters to upper

		for (int i = 0; i < _Equ.size(); i++) //If word is found in defines, replace it.
		{
			if (_Equ.at(i).first == word)
			{
				word = _Equ.at(i).second;
				break;
			}
		}

		if(!no_cursor)
			_LastWord = word;

		if (no_cursor) //If no_cursor, bring back the valeus from the buffers.
		{
			_CharCount = cc;
			_LineCount = lc;
			_Cursor = c;
			_HasMore = hm;
		}

		return word;
	}
};