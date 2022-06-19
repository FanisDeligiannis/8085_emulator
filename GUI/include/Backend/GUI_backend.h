#pragma once

#include "imgui.h"

extern ImFont* _Font;
extern int FontSize;

extern ImFont* _SevenSegmentFont;

void CloseApplication();
void CancelCloseApplication();

ImFont* LoadFont(int size);

int InitImGui();
