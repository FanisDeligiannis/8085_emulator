#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <format>

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

	uint8_t* Memory = parse(source);

	std::ofstream outFile;
	outFile.open("out.bin", std::ios::out | std::ios::trunc | std::ios::binary);
	
	for (int i = 0; i < 0xffff; i++)
	{
		outFile << std::hex << Memory[i];
	}

	outFile.close();

	free(Memory);
	delete source;

	return 0;
}