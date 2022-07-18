#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "assembler.h"

#include "source_file.h"

namespace InternalAssembler
{

	class Macro
	{
	private:
		SourceFile* _Source;
		std::vector<std::string> _Arguments;

		std::vector < std::pair<std::string, uint16_t> > labels;
		std::vector<std::pair<uint16_t, int>> Symbols;
		std::vector<IfExpr> _IfBuffer;


		int _StartLine = 0;

	public:
		std::string Name;


		Macro(std::string name, SourceFile* source);

		uint16_t Parse(SourceFile* source, uint16_t currentAddr, Assembler::Assembly& result, bool scanning = false);

		inline std::vector<std::pair<uint16_t, int>> GetSymbols() { return Symbols; }

	private:
		uint16_t Assemble(SourceFile* source, Assembler::Assembly& result, uint16_t currentAddr, SourceFile* ogSource, bool scanning = false);
	};

}