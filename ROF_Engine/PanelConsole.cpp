#include "PanelConsole.h"
#include "Application.h"
#include "ModuleWindow.h"

PanelConsole::PanelConsole() : Panel("Console")
{
	active = true;
	set_size = true;
}

PanelConsole::~PanelConsole()
{

}

void PanelConsole::AddLog(const char* log)
{
	Items.push_back(strdup(log));
}

void PanelConsole::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(5, (App->window->GetWindowSize().y - 205)));

	if (set_size == true)
	{
		ImGui::SetNextWindowSize(ImVec2(935, 200));
		set_size = false;
	}
	
	ImGui::Begin("Console", &active);

	if (ImGui::SmallButton("Scroll to bottom"))
	{
		scroll_to_bottom = true;
	}
	ImGui::Separator();

	for (int i = 0; i < Items.Size; i++)
	{
		const char* item = Items[i];
		ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  
		if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f); //If you write [error] before LOGs it will appear in red
		else if (strstr(item, "[warning]")) col = ImColor(1.0f, 1.0f, 0.4f, 1.0f); //If you write [warning] before LOGs it will appear in yellow
		else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(item);
		ImGui::PopStyleColor();			
	}

	if (scroll_to_bottom == true)
	{
		//Scroll bar: 0.0f top of last item, 0.5f vertical center of last item, 1.0f bottom of last item.
		ImGui::SetScrollHere(1.0f);
	}
	scroll_to_bottom = false;

	ImGui::End();
}
