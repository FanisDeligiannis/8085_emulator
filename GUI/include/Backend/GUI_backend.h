#pragma once

#include "imgui.h"

extern ImFont* _Font;
extern int FontSize;

extern ImFont* _SevenSegmentFont;

extern int _TargetFPS;

void CloseApplication();
void CancelCloseApplication();

int GetFPS();
void SetFPS(int fps);

ImFont* LoadFont(int size);

int InitImGui();
