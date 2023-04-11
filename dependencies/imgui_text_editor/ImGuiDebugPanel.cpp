#include <../imgui_text_editor/TextEditor.h>

void TextEditor::ImGuiDebugPanel(const std::string& panelName)
{
	ImGui::Begin(panelName.c_str());

	if (ImGui::CollapsingHeader("Cursor info"))
	{
		ImGui::DragInt("Cursor count", &mState.mCurrentCursor);
		for (int i = 0; i <= mState.mCurrentCursor; i++)
		{
			ImGui::DragInt2("Cursor", &mState.mCursors[i].mCursorPosition.mLine);
			ImGui::DragInt2("Selection start", &mState.mCursors[i].mSelectionStart.mLine);
			ImGui::DragInt2("Selection end", &mState.mCursors[i].mSelectionEnd.mLine);
			ImGui::DragInt2("Interactive start", &mState.mCursors[i].mInteractiveStart.mLine);
			ImGui::DragInt2("Interactive end", &mState.mCursors[i].mInteractiveEnd.mLine);
		}
	}
	if (ImGui::CollapsingHeader("Undo"))
	{
		static std::string numberOfRecordsText;
		numberOfRecordsText = "Number of records: " + std::to_string(mUndoBuffer.size());
		ImGui::Text(numberOfRecordsText.c_str());
		ImGui::DragInt("Undo index", &mState.mCurrentCursor);
		for (int i = 0; i < mUndoBuffer.size(); i++)
		{
			if (ImGui::CollapsingHeader(std::to_string(i).c_str()))
			{

				ImGui::Text("Operations");
				for (int j = 0; j < mUndoBuffer[i].mOperations.size(); j++)
				{
					ImGui::Text(mUndoBuffer[i].mOperations[j].mText.c_str());
					ImGui::Text(mUndoBuffer[i].mOperations[j].mType == UndoOperationType::Add ? "Add" : "Delete");
					ImGui::DragInt2("Start", &mUndoBuffer[i].mOperations[j].mStart.mLine);
					ImGui::DragInt2("End", &mUndoBuffer[i].mOperations[j].mEnd.mLine);
					ImGui::Separator();
				}
			}
		}
	}
	if (ImGui::Button("Run unit tests"))
	{
		UnitTests();
	}
	ImGui::End();
}