#pragma once

#include "TextEditor.h"

namespace CodeEditor {

	extern TextEditor editor;
	extern std::string FilePath;

	void Init();

	void TextEditorLoadFile();
	void TextEditorLoadFile(std::string path);
	std::string GetSavePath();
	void TextEditorSaveFile();
	
	void Render();
}