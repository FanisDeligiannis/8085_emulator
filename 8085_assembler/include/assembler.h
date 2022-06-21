#pragma once

#include <string>
#include <vector>

#include "source_file.h"

struct IfExpr
{
	bool expression;
	bool isElse;
};


class Macro;

namespace Assembler
{
	struct Assembly
	{
		uint8_t* Memory = nullptr;

		std::vector < std::pair<std::string, uint16_t> > Labels;
		std::vector<std::pair<int, std::string>> Errors;
		std::vector<std::pair<uint16_t, int>> Symbols;
		std::vector<IfExpr> IfBuffer;
		std::vector<Macro*> Macros;
	};

	SourceFile* ReadSourceFile(std::string fileName); // Take in file name, return SourceFile* of it.

	uint8_t* GetAssembledMemory(SourceFile* source, Assembler::Assembly& result); // Assembled memory from SourceFile*
	uint8_t* GetAssembledMemory(std::string code, Assembler::Assembly& result); // Assembled memory from std::string
}

extern Assembler::Assembly* currentAssembler;

void Error(std::string err, SourceFile* source); // Add error to error list. 
void Error(std::string err, int line); // Add error to error list. 

void ParseIfDirective(SourceFile* source, std::vector<IfExpr> &_IfBuffer);

bool isNumber(std::string str);
uint8_t StringToUInt8(std::string str, SourceFile* source); // Converts string to uint8. Could be hex(ending in 'h'), binary(ending in 'b') or dec. 
uint16_t StringToUInt16(std::string str, SourceFile* source, bool noerrors = false, bool *NaN = nullptr); // Same but for uint16_t

uint8_t* parse(SourceFile* source, Assembler::Assembly& result, bool scanning = false); // Parses the file, returing a dump of the assembled memory or simply scans the file, parsing but only saving labels/EQU/MACRO