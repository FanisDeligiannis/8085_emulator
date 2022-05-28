#include "CodeEditor.h"
#include <fstream>

#include "Simulation.h"

#ifdef NFD
#include "nfd.h"
#endif


namespace CodeEditor {

	TextEditor editor;
	std::string FilePath = "";

	void Init()
	{
		auto lang = TextEditor::LanguageDefinition::ASM8085();
		editor.SetLanguageDefinition(lang);
		editor.SetShowWhitespaces(false);

		//editor.SetPalette(TextEditor::GetLightPalette());
	}

	void TextEditorLoadFile()
	{
#ifdef NFD
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("8085;txt", NULL, &outPath);
		if (result == NFD_OKAY)
		{
			std::ifstream t(outPath);
			if (t.good())
			{
				std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
				editor.SetText(str);
				FilePath = outPath;
			}
			free(outPath);
		}
		else if (result == NFD_CANCEL)
		{
			puts("User pressed cancel.");
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
		}
#else
		printf("Error opening file dialog");
#endif
	}

	void TextEditorLoadFile(std::string path)
	{
		std::ifstream t(path);
		if (t.good())
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			editor.SetText(str);
			FilePath = path;
		}
		else
		{
			printf("Error loading file %s \n", path.c_str());
		}
	}

	std::string GetSavePath()
	{
#ifdef NFD
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog("8085;", NULL, &outPath);
		if (result == NFD_OKAY)
		{
			std::string p = outPath;
			free(outPath);
			return p;
		}
		else if (result == NFD_CANCEL)
		{
			puts("User pressed cancel.");
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
		}
#else
		printf("Error opening file dialog");
#endif

		return "";
	}

	void TextEditorSaveFile()
	{
		if (editor.SaveFileAs || FilePath == "")
		{
			FilePath = GetSavePath();
		}


		editor.SaveFile = false;
		editor.SaveFileAs = false;

		std::string text = editor.GetText();
		std::ofstream file;
		file.open(FilePath, std::ios_base::out);
		file << text;
		file.close();
	}

	void Render()
	{
		TextEditor::ErrorMarkers markers;
		for (int i = 0; i < Simulation::Errors.size(); i++)
		{
			markers.insert(Simulation::Errors.at(i));
		}
		editor.SetErrorMarkers(markers);


		ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

		if (editor.SaveFile || editor.SaveFileAs)
		{
			TextEditorSaveFile();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load"))
				{
					TextEditorLoadFile();
				}

				if (ImGui::MenuItem("Save"))
				{
					editor.SaveFile = false;
					editor.SaveFileAs = false;

					TextEditorSaveFile();
				}
				if (ImGui::MenuItem("SaveAs"))
				{
					editor.SaveFile = false;
					editor.SaveFileAs = false;

					TextEditorSaveFile();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					editor.SetReadOnly(ro);
				ImGui::Separator();

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
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("Assemble"))
				{
					Simulation::Assemble(editor.GetText());
				}

				if (ImGui::MenuItem("Run"))
				{
					if (Simulation::Running)
					{
						Simulation::Stop();
					}

					Simulation::Run();
				
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		auto cpos = editor.GetCursorPosition();
		std::string base_filename = FilePath.substr(FilePath.find_last_of("/\\") + 1);
		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(), base_filename.c_str());

		editor.Render("TextEditor");
		ImGui::End();
	}
}