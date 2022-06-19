#pragma once

#include "TextEditor.h"

#define RECENT_FILES ".8085emu/recents"

namespace CodeEditor {

	extern TextEditor editor;
	extern std::string FilePath;

	void Init();


	std::vector<std::string> getRecentFiles(std::string path);
	void AddRecentFile(std::string path);
	void SaveRecentFiles(std::string path);

	bool TextEditorLoadFile();
	bool TextEditorLoadFile(std::string path);
	std::string GetSavePath();
	bool TextEditorSaveFile();
	
	void Render();

	void PreDestroy();
}