#pragma once

#include <string>
#include <vector>

#include "source_file.h"

void Error(std::string err, SourceFile* source); // Add error to error list. 

std::vector<std::pair<std::string, uint16_t>> GetLabels(); // Returns all labels

uint8_t StringToUInt8(std::string str, SourceFile* source); // Converts string to uint8. Could be hex(ending in 'h'), binary(ending in 'b') or dec. 
uint16_t StringToUInt16(std::string str, SourceFile* source, bool noerrors = false, bool *NaN = nullptr); // Same but for uint16_t

void ScanForLabels(SourceFile* source); // Parses the file, only saving labels.

uint8_t* parse(SourceFile* source); // Parses the file, returing a dump of the assembled memory

namespace Assembler
{
	SourceFile* ReadSourceFile(std::string fileName); // Take in file name, return SourceFile* of it.

	std::vector<std::pair<int, std::string>> GetErrors(); // Get all errors
	std::vector<std::pair<uint16_t, int>> GetSymbols(); // Get Symbols. Symbols match an address to a line of code.

	uint8_t* GetAssembledMemory(SourceFile* source); // Assembled memory from SourceFile*
	uint8_t* GetAssembledMemory(std::string code); // Assembled memory from std::string
	uint8_t* GetAssembledMemory(char* file); // Assembled memory from filename

	void SaveAssembledMemory(uint8_t* memory, std::string outFileName); // Save uint8_t* to outFileName
	void SaveAssembledMemory(SourceFile* source, std::string outfile); // Assemble SourceFile* and save to outfile
	void SaveAssembledMemory(std::string inFileName, std::string outFileName); // Read file, assemble it, and save it to outFileName
}