#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#include "assembler.h"

#include "source_file.h"

namespace InternalAssembler
{

	class Macro
	{
	private:
		std::shared_ptr<SourceFile> _Source;
		std::vector<std::string> _Arguments;

		std::vector < std::pair<std::string, uint16_t> > labels;
		std::vector<std::pair<uint16_t, int>> Symbols;
		std::vector<IfExpr> _IfBuffer;


		int _StartLine = 0;

	public:
		std::string Name;


		Macro(std::string name, std::shared_ptr<SourceFile> source);

		uint16_t Parse(std::shared_ptr<SourceFile> source, uint16_t currentAddr, Assembler::Assembly& result, bool scanning = false);

		inline std::vector<std::pair<uint16_t, int>> GetSymbols() { return Symbols; }

	private:
		uint16_t Assemble(std::shared_ptr<SourceFile> source, Assembler::Assembly& result, uint16_t currentAddr, std::shared_ptr<SourceFile> ogSource, bool scanning = false);
	};

}