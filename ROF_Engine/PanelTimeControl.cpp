#include "PanelTimeControl.h"
#include "Application.h"
#include "ModuleWindow.h"

PanelTimeControl::PanelTimeControl() : Panel("Time Control")
{
	active = false;
	set_size = true;
}

PanelTimeControl::~PanelTimeControl()
{

}

void PanelTimeControl::Draw()
{
	if (set_size == true)
	{
		ImGui::SetNextWindowPos(ImVec2((App->window->GetWindowSize().x / 2) - 300, (App->window->GetWindowSize().y / 2) - 120));
		ImGui::SetNextWindowSize(ImVec2(500, 160));
		set_size = false;
	}

	ImGui::Begin("Time Control", &active);

	ImGui::Text("Time since engine startup: ");
	ImGui::SameLine();
	ImGui::Text("%.2f Seconds",App->time_since_startup.ReadSec());

	ImGui::End();
}