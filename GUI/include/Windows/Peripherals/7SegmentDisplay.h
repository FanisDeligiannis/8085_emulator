#pragma once

#include "Windows/Window.h"

#include <cstdint>

class SegmentDisplay : public Window
{
public:
	static SegmentDisplay* Instance;
	
	char chars[6];
	int show = 0;
	
	bool _Saved = true;

public:
	void Init() override;
	void Open() override;
	void Close() override;
	void SimulationStart() override;
	void Render() override;

	bool IsValidCharacter(uint8_t val);
};