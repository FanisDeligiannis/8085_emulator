#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <format>

#include "main.h"

#include "assembler.h"

uint8_t* Memory;
std::ofstream outFile;

void Error(std::string err, SourceFile* source)
{
	printf("Error: Line %d, Character %d\n%s", source->GetLine(), source->GetCharCount(), err.c_str());

	outFile.close();
	free(Memory);
	delete source;

	exit(1);
}

int main(int argc, char* argv[])
{
	std::string sourceFileName = "";
	if (argc < 2)
	{
		sourceFileName = "source.8085";
		//printf("Usage: %s source [out]\n", argv[0]);
		//std::cin.get();
		//return 1;
	}
	else
	{
		sourceFileName = argv[1];
	}
	
	std::string outFileName = "out.bin";

	if (argc > 2)
	{
		outFileName = argv[2];
	}

	std::string file;
	std::string line;

	std::ifstream inFile(sourceFileName);

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

	Memory = parse(source);

	outFile.open(outFileName.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	
	for (int i = 0; i < 0xffff; i++)
	{
		outFile << std::hex << Memory[i];
	}

	outFile.close();

	free(Memory);
	delete source;

	return 0;
}