#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "source_file.h"

class Macro
{
private:
	SourceFile* _Source;
	std::vector<std::string> _Arguments;

	std::vector < std::pair<std::string, uint16_t> > labels;
	std::vector<std::pair<uint16_t, int>> Symbols;


	int _StartLine = 0;

public:
	std::string Name;


	Macro(std::string name, SourceFile* source);

	uint16_t Parse(SourceFile* source, uint8_t* memory, uint16_t currentAddr);
	uint16_t CalculateNewAddr(SourceFile* source, uint16_t currentAddr);

	inline std::vector<std::pair<uint16_t, int>> GetSymbols() { return Symbols; }

private:
	uint16_t Assemble(SourceFile* source, uint8_t* _Memory, uint16_t currentAddr, SourceFile* ogSource);
	uint16_t ScanForLabels(SourceFile* source, uint16_t currentAddr, SourceFile* ogSource);
};