#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include "source_file.h"
#include "assembler.h"

int main()
{
	std::string file;
	std::string line;

	std::ifstream inFile("filename.txt");

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

	parse(source);

	return 0;
}