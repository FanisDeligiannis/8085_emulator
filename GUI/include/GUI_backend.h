#pragma once

#include "imgui.h"

extern ImFont* _Font;
extern int FontSize;

ImFont* LoadFont(int size);

int InitImGui();
