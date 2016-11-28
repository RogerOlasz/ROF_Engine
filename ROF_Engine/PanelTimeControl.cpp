#include "PanelTimeControl.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "GameTimeManager.h"

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

	game_time_speed = App->game_timer->GetGameTimeSpeed();

	if (ImGui::Button("PLAY", ImVec2(50,25)))
	{
		if (App->game_timer->GetGameState() == false)
		{
			App->game_timer->Start();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("STOP", ImVec2(50, 25)))
	{
		if (App->game_timer->GetGameState())
		{
			App->game_timer->Stop();
		}
	}

	if (ImGui::DragFloat("Game time multiplier", &game_time_speed, 0.01f, -100.0f, 100.0f))
	{
		App->game_timer->SetTimeSpeed(game_time_speed);
	}

	ImGui::Text("Total time: %.2f Seconds", App->game_timer->GetTimeSinceStartUp());
	ImGui::Text("Game time: %.2f Seconds", App->game_timer->GetGameTime());
	ImGui::Text("Dt: %.2f Miliseconds", App->game_timer->GetGameDt());

	ImGui::End();
}