#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <format>

#include "cpu.h"

std::string IntToHex(int num, int c = 4)
{
	if (c == 2)
		return std::format("{:#04X}", num);
	else if (c == 4)
		return std::format("{:#06X}", num);
	return std::format("{:#06X}", num);
}

int main(int argc, char** argv) {
	std::string binFileName = "";
	if (argc < 2)
	{
		binFileName = "out.bin";
		//printf("Usage: %s source [out]\n", argv[0]);
		//std::cin.get();
		//return 1;
	}
	else
	{
		binFileName = argv[1];
	}

	std::string outFileName = "dump.bin";

	if (argc > 2)
	{
		outFileName = argv[2];
	}

	std::ifstream inFile(binFileName, std::ios::binary);

	uint8_t* data = (uint8_t*)calloc(0xffff+1, sizeof(uint8_t));

	if (data == nullptr)
	{
		perror("Error allocating memory");
		exit(1);
	}

	char c;
	int i = 0;

	while (inFile.get(c) && i <= 0xffff)
	{
		data[i] = *(uint8_t*)&c;
		i++;
	}

	inFile.close();

	Memory* memory = new Memory();
	memory->_SetData(data, 0xffff);

	CPU* cpu = new CPU(memory);
	
	for (int i = 0; i < 0x0100; i++)
	{
		printf("%s: %s\n", IntToHex(i).c_str(), IntToHex(memory->GetDataAtAddr(i), 2).c_str());
	}

	return 0;
}