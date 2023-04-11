#pragma once

#include <string>
#include <vector>
#include <memory>

#include "source_file.h"

struct IfExpr
{
	bool expression;
	bool isElse;
};

namespace InternalAssembler { class Macro; }

namespace Assembler
{
	struct Assembly
	{
		std::shared_ptr<uint8_t> Memory = nullptr;

		std::vector < std::pair<std::string, uint16_t> > Labels;
		std::vector<std::pair<int, std::string>> Errors;
		std::vector<std::pair<uint16_t, int>> Symbols;
		std::vector<IfExpr> IfBuffer;
		std::vector<InternalAssembler::Macro*> Macros;
	};

	std::shared_ptr<InternalAssembler::SourceFile> ReadSourceFile(std::string fileName); // Take in file name, return SourceFile* of it.

	std::shared_ptr<uint8_t> GetAssembledMemory(std::shared_ptr<InternalAssembler::SourceFile> source, Assembler::Assembly& result); // Assembled memory from SourceFile*

	std::shared_ptr<uint8_t> GetAssembledMemory(std::string code, Assembler::Assembly& result); // Assembled memory from std::string

	std::vector<std::pair<std::string, uint16_t>> GetLabels(std::string code);
}

namespace InternalAssembler
{

	extern Assembler::Assembly* currentAssembler;

	void Error(std::string err, std::shared_ptr<SourceFile> source); // Add error to error list. 
	void Error(std::string err, int line); // Add error to error list. 

	void ParseIfDirective(std::shared_ptr<SourceFile> source, std::vector<IfExpr>& _IfBuffer);

	bool isNumber(std::string str);
	uint8_t StringToUInt8(std::string str, std::shared_ptr<SourceFile> source); // Converts string to uint8. Could be hex(ending in 'h'), binary(ending in 'b') or dec. 
	uint16_t StringToUInt16(std::string str, std::shared_ptr<SourceFile> source, bool noerrors = false, bool* NaN = nullptr); // Same but for uint16_t

	std::shared_ptr<uint8_t> parse(std::shared_ptr<SourceFile> source, Assembler::Assembly& result, bool scanning = false, bool bootloader = false); // Parses the file, returing a dump of the assembled memory or simply scans the file, parsing but only saving labels/EQU/MACRO
}