#pragma once

#include <string>

class SourceFile
{
private:
	std::string _Source;
	std::string _LastWord;
	int _Pointer = 0;

	bool _HasMore = true;
public:
	SourceFile(std::string source)
		: _Source(source), _LastWord("") {}

	bool HasMore()
	{
		return _HasMore;
	}

	std::string Next()
	{
		std::string word = "";

		int i = _Pointer;

		bool done = false;

		while (i < _Source.length() && !done)
		{
			if (word.length() == 0)
			{
				if (_Source[i] == ' ' || _Source[i] == '\t' || _Source[i] == '\n' || _Source[i] == '\t' || _Source[i] == ',') // ignores
				{
				}
				else
				{
					word += _Source[i];
				}
			}
			else
			{
				if (_Source[i] == ' ' || _Source[i] == '\t' || _Source[i] == '\n' || _Source[i] == '\t' || _Source[i] == ',') // end del
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

		_Pointer = i;
		if (_Pointer == _Source.length())
		{
			_HasMore = false;
		}

		_LastWord = word;

		return word;
	}
};