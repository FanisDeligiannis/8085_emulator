﻿#include "Windows/Core/CodeEditor.h"

#include <fstream>
#include <filesystem>

#include "Backend/GUI_backend.h"
#include "Simulation.h"
#include "assembler.h"

#include "imgui_internal.h"

#include "ConfigIni.h"

#ifdef NFD
#include "nfd.h"
#endif
#include "../../fonts/MonoLisa.cpp"

CodeEditor* CodeEditor::Instance;

void CodeEditor::Init()
{
	Instance = this;

	RecentFiles = getRecentFiles(RECENT_FILES);

	LanguageDefinition = TextEditor::LanguageDefinition::ASM8085();

	editor.SetLanguageDefinition(LanguageDefinition);
	editor.SetShowWhitespaces(false);

	InitialFontSize = ConfigIni::GetInt("CodeEditor", "FontSize", 20);
	_Font = LoadFont(InitialFontSize);	

	FontSize = InitialFontSize;

	_Font->Scale = (float)FontSize / (float)InitialFontSize;

	DynamicLabelHighlight = ConfigIni::GetInt("Experimental", "DynamicLabelHighlight", 1);


	std::string palette = ConfigIni::GetString("CodeEditor", "Palette", "Mariana");

	if (palette == "Dark")
		editor.SetPalette(TextEditor::GetDarkPalette());
	else if (palette == "Mariana")
		editor.SetPalette(TextEditor::GetMarianaPalette());
	else if (palette == "Light")
		editor.SetPalette(TextEditor::GetLightPalette());
	else if (palette == "Retro")
		editor.SetPalette(TextEditor::GetRetroBluePalette());


	//editor.SetPalette(TextEditor::GetLightPalette());
}

std::vector<std::string> CodeEditor::getRecentFiles(std::string path)
{
	std::vector<std::string> result;

	//std::ifstream f(path.c_str());
	std::string abspath = std::filesystem::absolute(path).string();
	std::ifstream f(abspath);

	if (!f)
	{
		printf("Recents file could not be opened: %s\n", abspath.c_str()); // Report error
		printf("Error code: %s\n", strerror(errno)); // Get some info as to why
		return result;
	}

	std::string	line;

	while (std::getline(f, line))
	{
		if (!line.empty())
		{
			std::ifstream recent_file(line.c_str());
			if(recent_file.good())
				result.push_back(line);
		}
	}

	return result;
}

void CodeEditor::AddRecentFile(std::string path)
{
	bool already_added = false;

	for (int i = 0; i < RecentFiles.size(); i++)
	{
		if (RecentFiles.at(i) == path)
		{
			already_added = true;

			RecentFiles.erase(RecentFiles.begin() + i);
			RecentFiles.push_back(path);

			SaveRecentFiles(RECENT_FILES);
		}
	}

	if (already_added)
		return;

	while (RecentFiles.size() >= 6)
	{
		RecentFiles.erase(RecentFiles.begin());
	}

	RecentFiles.push_back(path);
		
	SaveRecentFiles(RECENT_FILES);
}

void CodeEditor::SaveRecentFiles(std::string path)
{
	std::ofstream file;
	file.open(path, std::ios_base::out);
		
	for (int i = 0; i < RecentFiles.size(); i++)
	{
		file << RecentFiles.at(i);

		if (i != RecentFiles.size() - 1)
			file << "\n";
	}

	file.close();
}


void CodeEditor::SetFontSize(int size)
{
	FontSize = size;

	_Font->Scale = (float)FontSize / (float)InitialFontSize;

	ConfigIni::SetInt("CodeEditor", "FontSize", FontSize);
}

bool CodeEditor::TextEditorLoadFile()
{
#ifdef NFD
	nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_OpenDialog("8085,asm", NULL, &outPath);
	if (result == NFD_OKAY)
	{
		bool ret = TextEditorLoadFile(outPath);
		free(outPath);

		return ret;
	}
	else if (result == NFD_CANCEL)
	{
#ifdef _DEBUG
		printf("User pressed cancel.\n");
#endif
		return false;
	}
	else
	{
#ifdef _DEBUG
		printf("Error: %s\n", NFD_GetError());
#endif
		return false;
	}
#else
	printf("Error opening file dialog\n");
#endif
	return false;
}

bool CodeEditor::TextEditorLoadFile(std::string path)
{
	std::ifstream t(path);
	if (t.good())
	{
		AddRecentFile(path);

		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor.SetText(str);
		editor.SetSelection({ 0,0 }, { 0,0 });
		FilePath = path;
		//editor.IsFileDirty = false;
		FileLoaded = true;

		if (DynamicLabelHighlight)
		{
			LabelHighlight();
		}

		return true;
	}
	else
	{
#ifdef _DEBUG
		printf("Error loading file %s \n", path.c_str());
#endif
		return false;
	}
}

std::string CodeEditor::GetSavePath()
{
#ifdef NFD
	nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_SaveDialog("8085,asm", NULL, &outPath);
	if (result == NFD_OKAY)
	{
		std::string p = outPath;
		free(outPath);
		return p;
	}
	else if (result == NFD_CANCEL)
	{
#ifdef _DEBUG
		printf("User pressed cancel.\n");
#endif
	}
	else
	{
#ifdef _DEBUG
		printf("Error: %s\n", NFD_GetError());
#endif
	}
#else
	printf("Error opening file dialog\n");
#endif

	return "";
}

bool CodeEditor::HasExtension(std::string filepath)
{
	std::size_t found = filepath.find_last_of("/\\");
	std::string file = filepath.substr(found + 1);

	std::size_t extension = file.find_last_of(".");
		
	return extension != std::string::npos;
}

void CodeEditor::LabelHighlight()
{
	//std::vector<std::pair<std::string, uint16_t>> labels = Assembler::GetLabels(editor.GetText()); // Found it to be slow when trying with 100+ labels.

	/*for (auto& k : labels)
	{
		TextEditor::Identifier id;
		editor.GetLanguageDefinition().mLabels.insert(std::make_pair(k.first, id));
	}*/

	static const boost::regex label_finder("\\b([A-Za-z0-9_]+):"); // seems to be a bit less reliable, but instant even with thousands of labels.


	std::string text = editor.GetText();

	boost::sregex_token_iterator iter(text.cbegin(), text.cend(), label_finder, 0);
	boost::sregex_token_iterator end;

	editor.GetLanguageDefinition()->mLabels.clear();

	for (; iter != end; ++iter)
	{
		std::string label = *iter;
		label.pop_back();
		std::transform(label.begin(), label.end(), label.begin(), ::toupper);
		TextEditor::Identifier id;
		editor.GetLanguageDefinition()->mLabels.insert(std::make_pair(label, id));
	}

	editor.ForceColorizeAll();

	//Tested with 5000 lines of labels and another 5000 lines of actual code. Everything was instant, CPU usage spiked from 3-5% to ~18% for a moment. Assembling such a program would be a much bigger issue than that right now. 
	//More testing needed, but all looks good.
}

bool CodeEditor::TextEditorSaveFile()
{
	if (editor.SaveFileAs || FilePath == "")
	{
		std::string temp = FilePath;
		FilePath = GetSavePath();

		if (FilePath == "")
		{
			editor.SaveFile = false;
			editor.SaveFileAs = false;

			FilePath = temp;
			return false;
		}
			
		if (!HasExtension(FilePath))
		{
			FilePath = FilePath + ".8085";
		}

		AddRecentFile(FilePath);
	}


	editor.SaveFile = false;
	editor.SaveFileAs = false;

	//editor.IsFileDirty = false;
	editor.SetFileNotDirty();

	std::string text = editor.GetText();
	text = text.substr(0, text.length() - 1);

	std::ofstream file;

	file.open(FilePath, std::ios_base::out);

	if (file.good())
	{
		file << text;
		file.close();
	}

	return true;
}

void CodeEditor::Render()
{
	if (editor.IsTextChanged())
	{
		//if (!FileLoaded)
		//	editor.IsFileDirty = true;
		//else
		//	FileLoaded = false;

		if (FileLoaded)
			FileLoaded = false;


		if (DynamicLabelHighlight)
		{
			if (editor.GetCurrentLineText().find(':') != std::string::npos) // Let's only do that if there's a ':' in there. Issue: will not be done when removing a ':'. Maybe I can peek in the undo buffer?
			{
				LabelHighlight();
			}
		}
	}

	if (Simulation::GetRunning())
	{
		editor.SetReadOnly(true);

		if (editor._BreakpointsChanged)
		{
			Simulation::cpu->UpdateBreakpoints();
			editor._BreakpointsChanged = false;
		}
	}
	else
	{
		editor.SetReadOnly(false);
	}

	auto cpos = editor.GetCursorPosition();
		
	if (editor.IsTextChanged())
	{	
		if (editor.GetTotalLines() > _PrevLineCount)
		{
			_PrevLineCount = editor.GetTotalLines();

			for (int i = 0; i < Simulation::program.Errors.size(); i++)
			{
				if (cpos.mLine + 1 <= Simulation::program.Errors.at(i).first)
				{
					Simulation::program.Errors.at(i).first++;
				}
				else if (cpos.mLine + 1 == Simulation::program.Errors.at(i).first + 1 && _PrevCpos.mColumn == 0)
				{
					Simulation::program.Errors.at(i).first++;
				}
			}
		}
		else if (editor.GetTotalLines() < _PrevLineCount)
		{
			_PrevLineCount = editor.GetTotalLines();

			for (int i = 0; i < Simulation::program.Errors.size(); i++)
			{
				if (cpos.mLine + 1 < Simulation::program.Errors.at(i).first)
				{
					Simulation::program.Errors.at(i).first--;
				}
			}
		}
		else
		{
			for (int i = 0; i < Simulation::program.Errors.size(); i++)
			{
				if (cpos.mLine + 1 == Simulation::program.Errors.at(i).first)
					Simulation::program.Errors.erase(Simulation::program.Errors.begin() + i);
			}
		}
	}
	_PrevCpos = cpos;

	TextEditor::ErrorMarkers markers;
	for (int i = 0; i < Simulation::program.Errors.size(); i++)
	{
		markers.insert(Simulation::program.Errors.at(i));
	}
	editor.SetErrorMarkers(markers);

	if (Simulation::GetRunning() && (Simulation::GetPaused() || Simulation::_Stepping || Simulation::cpu->GetHalted()))
	{
		auto symbols = Simulation::program.Symbols;

		for (int i = 0; i < symbols.size(); i++)
		{
			if (symbols.at(i).first == Simulation::cpu->PC->Get())
			{
				editor.CurrentLine = symbols.at(i).second - 1;
			}
		}
	}
	else
	{
		editor.CurrentLine = -1;
	}

	ImGui::PushFont(_Font);

	ImGui::Begin("Code Editor", 0, ImGuiWindowFlags_MenuBar);

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_Equal, false))
	{
		SetFontSize(FontSize + 1);
	}

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_Minus, false))
	{
		SetFontSize(FontSize - 1);
	}

	if (editor.SaveFile || editor.SaveFileAs)
	{
		TextEditorSaveFile();
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", 0, false, !editor.IsReadOnly()))
			{
				NewFilePath = ".";
				ShouldLoadFile = true;
				Simulation::program.Errors.clear();
			}
			if (ImGui::MenuItem("Load", 0, false, !editor.IsReadOnly()))
			{
				NewFilePath = "";
				ShouldLoadFile = true;
				Simulation::program.Errors.clear();
			}

			if (ImGui::BeginMenu("Recent", !editor.IsReadOnly()))
			{
				for (int i = RecentFiles.size() - 1; i >= 0; i--)
				{
					std::string filename = std::filesystem::path(RecentFiles.at(i)).filename().string();

					if (ImGui::MenuItem((filename.c_str())))
					{
						ShouldLoadFile = true;
						NewFilePath = RecentFiles.at(i);
						Simulation::program.Errors.clear();
					}
				}
				ImGui::EndMenu();
			}


			if (ImGui::MenuItem("Save", "CTRL+S"))
			{
				editor.SaveFile = false;
				editor.SaveFileAs = false;

				TextEditorSaveFile();
			}
			if (ImGui::MenuItem("Save As", "CTRL+SHIFT+S"))
			{
				editor.SaveFile = false;
				editor.SaveFileAs = true;

				TextEditorSaveFile();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();

			if (ImGui::MenuItem("Undo", "ALT+Backspace", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::Separator();
			
			if (ImGui::MenuItem("Find in file", "CTRL+F", nullptr, true))
				editor.ShowFindInFile();
			if (ImGui::MenuItem("Replace in file", "Ctrl+SHIFT+F", nullptr, !ro))
				editor.ShowReplaceInFile();


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
			{
				editor.SetPalette(TextEditor::GetDarkPalette());
				
				std::string palette = "Dark";
				ConfigIni::SetString("CodeEditor", "Palette", palette);
			}
			if (ImGui::MenuItem("MarianaPalette"))
			{
				editor.SetPalette(TextEditor::GetMarianaPalette());

				std::string palette = "Mariana";
				ConfigIni::SetString("CodeEditor", "Palette", palette);
			}
			if (ImGui::MenuItem("Light palette"))
			{
				editor.SetPalette(TextEditor::GetLightPalette());

				std::string palette = "Light";
				ConfigIni::SetString("CodeEditor", "Palette", palette);
			}
			if (ImGui::MenuItem("Retro blue palette"))
			{
				editor.SetPalette(TextEditor::GetRetroBluePalette());

				std::string palette = "Retro";
				ConfigIni::SetString("CodeEditor", "Palette", palette);
			}

			ImGui::Separator();

			if (ImGui::Checkbox("Dynamic Label Highlight (experimental)", &DynamicLabelHighlight))
			{
				if (!DynamicLabelHighlight)
				{
					ConfigIni::SetInt("Experimental", "DynamicLabelHighlight", 0);

					editor.GetLanguageDefinition()->mLabels.clear();
					editor.ForceColorizeAll();
				}
				else
				{
					ConfigIni::SetInt("Experimental", "DynamicLabelHighlight", 1);
					LabelHighlight();
				}
			}


			ImGui::Separator();

			ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
				
			if (ImGui::MenuItem("Size--"))
			{
				SetFontSize(FontSize - 1);
			}

			if (ImGui::MenuItem("Size++"))
			{
				SetFontSize(FontSize + 1);
			}

			ImGui::PopItemFlag();
				
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	std::string base_filename = FilePath.substr(FilePath.find_last_of("/\\") + 1);


	ImGui::Text("%6d/%-6d %6d lines  | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.GetLanguageDefinition()->mName.c_str(), editor.IsFileDirty() ? "*" : " ");
	
	ImGui::SameLine();

	ImVec2 cursorPos = ImGui::GetCursorPos();
	ImGui::Text("%s", base_filename.c_str());


	if (base_filename != "")
	{
		ImGui::SetCursorPos(cursorPos);
		
		ImVec2 hoverTriggerSize = ImGui::CalcTextSize(base_filename.c_str());
		ImGui::InvisibleButton("##hoverTrigger", hoverTriggerSize);

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("%s", base_filename.c_str());
			ImGui::EndTooltip();
		}
	}

	editor.Render("TextEditor", ImGui::IsWindowFocused());
		
	ImGui::End();

	if (ImGui::BeginPopupModal("Load File", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("There are unsaved changes in the current file!");

		if (ImGui::Button("Save"))
		{
			if (TextEditorSaveFile())
			{
				ShouldLoadFile = true;
				//editor.IsFileDirty = false;
				editor.SetFileNotDirty();

				ImGui::CloseCurrentPopup();
			}	
		}
			
		ImGui::SameLine();

		if (ImGui::Button("Don't save"))
		{
			ShouldLoadFile = true;
			//editor.IsFileDirty = false;
			editor.SetFileNotDirty();

			ImGui::CloseCurrentPopup();
		}
			
		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ClosePopup)
	{
		ImGui::OpenPopup("Close Application");
		ClosePopup = false;
	}

	if (ImGui::BeginPopupModal("Close Application", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("There are unsaved changes in the current file!");

		if (ImGui::Button("Save"))
		{
			if (TextEditorSaveFile())
			{
				ImGui::CloseCurrentPopup();
				CloseApplication();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Don't save"))
		{
			ImGui::CloseCurrentPopup();
			CloseApplication();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			CancelCloseApplication();
		}

		ImGui::EndPopup();
	}

	if (ShouldLoadFile)
	{
		if (editor.IsFileDirty())
		{
			ImGui::OpenPopup("Load File");
			ShouldLoadFile = false;
		}
		else
		{
			ShouldLoadFile = false;

			if (NewFilePath == "")
			{
				TextEditorLoadFile();
			}
			else if (NewFilePath == ".")
			{
				editor.SetText("");
				editor.SetSelection({ 0,0 }, { 0,0 });
				FilePath = "";
				FileLoaded = true;
			}
			else
			{
				TextEditorLoadFile(NewFilePath);
			}

			//StuffToSave = false;
			editor.SetFileNotDirty();
			NewFilePath = "";
		}
	}

	ImGui::PopFont();
}

void CodeEditor::PreDestroy()
{
	if (editor.IsFileDirty())
	{
		ClosePopup = true;
	}
	else
	{
		CloseApplication();
	}
}
