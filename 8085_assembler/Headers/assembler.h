#pragma once

#include <string>
#include <vector>
#include "source_file.h"

std::vector<std::pair<std::string, uint16_t>> GetLabels();

uint8_t StringToUInt8(std::string str);
uint16_t StringToUInt16(std::string str);

std::string parse(SourceFile* source);