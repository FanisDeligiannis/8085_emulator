#ifdef _DIST
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <stdio.h>
#include <string>
#include <thread>
#include <iostream>

#include "Application.h"
#include "GUI_backend.h"


int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		std::string fileToOpen = argv[1];
		Application::DefaultFile = fileToOpen;
	}

	//Initialise ImGui from Backend. 
	//it initialises the application and calls Render.
	InitImGui();

	return 0;
}