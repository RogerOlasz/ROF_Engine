#include "PanelConsole.h"

PanelConsole::PanelConsole()
{}

PanelConsole::~PanelConsole()
{}

void PanelConsole::AddLog(const char* log)
{
	Items.push_back(strdup(log));
}

void PanelConsole::Draw(bool* open)
{
	ImGui::Begin("Console", open, ImVec2(500, 200));

	for (int i = 0; i < Items.Size; i++)
	{
		const char* item = Items[i];
		ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text. TODO
		if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f); //If you write [error] before LOGs it will appear in red
		else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(item);
		ImGui::PopStyleColor();
	}

	ImGui::End();
}
