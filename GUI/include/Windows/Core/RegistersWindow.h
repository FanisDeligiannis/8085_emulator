#pragma once

#include "Windows/Window.h"

#include "imgui.h"
#include "Simulation.h"

class RegistersWindow : public Window
{
public:
	void Init() override { }

	void Render() override;
};