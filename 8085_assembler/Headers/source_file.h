#pragma once

#include <string>

class SourceFile
{
private:
	std::string _Source;
	std::string _LastWord;
	int _Pointer = 0;

	bool _HasMore = true;

	int _LineCount = 1;
	int _CharCount = 1;

	int _PrevLineCount = 1;
	int _PrevCharCount = 1;
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

	inline std::string Next(bool ignore_newline_at_start = false)
	{
		std::string word = "";

		int i = _Pointer;

		bool done = false;

		while (i < _Source.length() && !done)
		{
			if (word.length() == 0)
			{
				if (_Source[i] == ' '  || _Source[i] == '\t' || _Source[i] == ',') // ignores
				{
					_CharCount++;
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

		_Pointer = i;
		if (_Pointer == _Source.length())
		{
			_HasMore = false;
		}

		_LastWord = word;

		return word;
	}

	inline std::string GetLastWord()
	{
		return _LastWord;
	}
};