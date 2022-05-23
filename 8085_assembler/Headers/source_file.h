#pragma once

#include <string>
#include <vector>
#include <utility>

class SourceFile
{
private:
	std::string _Source;
	std::string _LastWord;
	int _Cursor = 0;

	bool _HasMore = true;

	int _LineCount = 1;
	int _CharCount = 1;

	int _PrevLineCount = 1;
	int _PrevCharCount = 1;

public:
	std::vector < std::pair<std::string, std::string> > _Equ;

public:
	SourceFile(std::string source)
		: _Source(source), _LastWord("") {}

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

	inline void SetFileCursor(int cursor)
	{
		_Cursor = cursor;
	}

	inline std::string NextNoCursor(bool ignore_newline_at_start = false)
	{
		std::string word = "";

		int i = _Cursor;

		bool done = false;

		while (i < _Source.length() && !done)
		{
			if (word.length() == 0)
			{
				if (_Source[i] == ';')
				{
					while (i < _Source.length() && _Source[i] != '\n')
					{
						i++;
					}
				}

				if (_Source[i] == ' ' || _Source[i] == '\t' || _Source[i] == ',') // ignores
				{
				}
				else if (_Source[i] == '\"')
				{
					word += '\"';

					i++;

					while (i < _Source.length() && _Source[i] != '\n' && _Source[i] != '\"')
					{
						word += _Source[i];
						i++;
					}

					if (_Source[i] == '\"')
						word += '\"';
				}
				else if (_Source[i] == '\n')
				{
					if (!ignore_newline_at_start)
					{
						done = true;
					}
				}
				else
				{
					_CharCount++;
					word += _Source[i];
				}
			}
			else
			{
				if (_Source[i] == ' ' || _Source[i] == '\t' || _Source[i] == ',') // end del
				{
					done = true;
				}
				else if (_Source[i] == '\n')
				{
					done = true;
				}
				else
				{
					word += _Source[i];
				}
			}

			i++;
		}

		for (int i = 0; i < _Equ.size(); i++)
		{
			if (_Equ.at(i).first == word)
			{
				word = _Equ.at(i).second;
				break;
			}
		}

		return word;
	}


	inline std::string Next(bool ignore_newline_at_start = false)
	{
		std::string word = "";

		int i = _Cursor;

		bool done = false;

		while (i < _Source.length() && !done)
		{
			if (word.length() == 0)
			{
				if (_Source[i]  == ';')
				{
					while (i < _Source.length() && _Source[i] != '\n')
					{
						i++;
					}
				}

				if (_Source[i] == ' '  || _Source[i] == '\t' || _Source[i] == ',') // ignores
				{
					_CharCount++;
				}
				else if (_Source[i] == '\"')
				{
					word += '\"';

					i++;

					while (i < _Source.length() && _Source[i] != '\n' && _Source[i] != '\"')
					{
						word += _Source[i];
						i++;
					}
					
					if (_Source[i] == '\"')
						word += '\"';
				}
				else if (_Source[i] == '\n')
				{
					_LineCount++;
					_CharCount = 0;

					if (!ignore_newline_at_start)
					{
						done = true;
					}
				}
				else
				{
					_CharCount++;
					word += _Source[i];
				}
			}
			else
			{
				if (_Source[i] == ' ' || _Source[i] == '\t' || _Source[i] == ',') // end del
				{
					_PrevLineCount = _LineCount;
					_PrevCharCount = _CharCount;

					_CharCount++;

					done = true;
				}
				else if (_Source[i] == '\n')
				{
					_PrevLineCount = _LineCount;
					_PrevCharCount = _CharCount;

					_LineCount++;
					_CharCount = 0;
					
					done = true;
				}
				else
				{
					_CharCount++;
					word += _Source[i];
				}
			}

			i++;
		}

		_Cursor = i;
		if (_Cursor == _Source.length())
		{
			_HasMore = false;
		}

		for (int i = 0; i < _Equ.size(); i++)
		{
			if (_Equ.at(i).first == word)
			{
				word = _Equ.at(i).second;
				break;
			}
		}

		_LastWord = word;

		return word;
	}

	inline std::string GetLastWord()
	{
		return _LastWord;
	}
};