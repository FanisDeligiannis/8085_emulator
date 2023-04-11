#pragma once

#include "Windows/Window.h"

#include <memory>
#include <vector>
#include <string>

#include "TextEditor.h"

#define RECENT_FILES ".8085emu/recents"

class CodeEditor : public Window {

private:
	std::vector<std::string> RecentFiles;

	TextEditor::LanguageDefinition LanguageDefinition;

	ImFont* _Font;
	int FontSize;
	int InitialFontSize;

	int _PrevLineCount = 0;
	TextEditor::Coordinates _PrevCpos;

	std::string NewFilePath = "";

	bool ShouldLoadFile = false;
	bool FileLoaded = false;
	bool StuffToSave = false;
	bool ClosePopup = false;

	bool DynamicLabelHighlight = true;

private:
	bool HasExtension(std::string filepath);

	void LabelHighlight();

public:
	static CodeEditor* Instance;

	TextEditor editor;
	std::string FilePath;

public:
	void Init() override;

	std::vector<std::string> getRecentFiles(std::string path);
	void AddRecentFile(std::string path);
	void SaveRecentFiles(std::string path);

	bool TextEditorLoadFile();
	bool TextEditorLoadFile(std::string path);
	std::string GetSavePath();
	bool TextEditorSaveFile();
	void SetFontSize(int size);


	void Render() override;

	void PreDestroy() override;
};