#include "Windows/CodeEditor.h"

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


namespace CodeEditor {

	std::vector<std::string> RecentFiles;

	ImFont* _Font;
	int FontSize;
	int InitialFontSize;

	int _PrevLineCount = 0;
	TextEditor::Coordinates _PrevCpos;

	TextEditor editor;
	std::string FilePath = "";
	std::string NewFilePath = "";

	bool ShouldLoadFile = false;
	bool FileLoaded = false;
	bool StuffToSave = false;

	void Init()
	{
		RecentFiles = getRecentFiles(RECENT_FILES);

		auto lang = TextEditor::LanguageDefinition::ASM8085();
		editor.SetLanguageDefinition(lang);
		editor.SetShowWhitespaces(false);

		InitialFontSize = ConfigIni::GetInt("CodeEditor", "FontSize", 20);
		_Font = LoadFont(InitialFontSize);

		FontSize = InitialFontSize;

		_Font->Scale = (float)FontSize / (float)InitialFontSize;

		//editor.SetPalette(TextEditor::GetLightPalette());
	}

	std::vector<std::string> getRecentFiles(std::string path)
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

	void AddRecentFile(std::string path)
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

	void SaveRecentFiles(std::string path)
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


	void SetFontSize(int size)
	{
		FontSize = size;

		_Font->Scale = (float)FontSize / (float)InitialFontSize;

		ConfigIni::SetInt("CodeEditor", "FontSize", FontSize);
	}

	bool TextEditorLoadFile()
	{
#ifdef NFD
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("8085,asm", NULL, &outPath);
		if (result == NFD_OKAY)
		{
			return TextEditorLoadFile(outPath);
			free(outPath);
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

	bool TextEditorLoadFile(std::string path)
	{
		std::ifstream t(path);
		if (t.good())
		{
			AddRecentFile(path);

			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			editor.SetText(str);
			FilePath = path;
			StuffToSave = false;
			FileLoaded = true;

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

	std::string GetSavePath()
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

	bool HasExtension(std::string filepath)
	{
		std::size_t found = filepath.find_last_of("/\\");
		std::string file = filepath.substr(found + 1);

		std::size_t extension = file.find_last_of(".");
		
		return extension != std::string::npos;
	}

	bool TextEditorSaveFile()
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

		StuffToSave = false;

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

	void Render()
	{
		if (editor.IsTextChanged())
		{
			if (!FileLoaded)
				StuffToSave = true;
			else
				FileLoaded = false;
		}

		if (Simulation::GetRunning())
		{
			editor.SetReadOnly(true);
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

		ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

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

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
					editor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
					editor.Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
					editor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
					editor.Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
					editor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					editor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					editor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					editor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
					editor.SetPalette(TextEditor::GetRetroBluePalette());

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
		ImGui::Text("%6d/%-6d %6d lines  | %s | %s%s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.GetLanguageDefinition().mName.c_str(), base_filename.c_str(), StuffToSave ? "*" : "");

		editor.Render("TextEditor");
		
		ImGui::End();

		if (ImGui::BeginPopupModal("Load File", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("There are unsaved changes in the current file!");

			if (ImGui::Button("Save"))
			{
				if (TextEditorSaveFile())
				{
					ShouldLoadFile = true;
					StuffToSave = false;

					ImGui::CloseCurrentPopup();
				}
				
			}
			
			ImGui::SameLine();

			if (ImGui::Button("Don't save"))
			{
				ShouldLoadFile = true;
				StuffToSave = false;

				ImGui::CloseCurrentPopup();
			}
			
			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ShouldLoadFile)
		{
			if (StuffToSave)
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
					FilePath = "";
					FileLoaded = true;
				}
				else
				{
					TextEditorLoadFile(NewFilePath);
				}

				StuffToSave = false;
				NewFilePath = "";
			}
		}

		ImGui::PopFont();
	}
}