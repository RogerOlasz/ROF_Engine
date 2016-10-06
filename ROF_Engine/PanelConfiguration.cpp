#include "PanelConfiguration.h"
#include "Application.h"
#include "ImGui/imgui.h"

PanelConfiguration::PanelConfiguration() : Panel("Configuration")
{
	while (fps_log.size() <= 49)
	{
		fps_log.push_back(0);
	}

	while (ms_log.size() <= 49)
	{
		ms_log.push_back(0);
	}
}

PanelConfiguration::~PanelConfiguration()
{}

void PanelConfiguration::Draw()
{
	ImGui::Begin("Configuration", &active);

	DrawApplication();

	ImGui::End();
}

void PanelConfiguration::DrawApplication()
{	
	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::Text("Limit Framerate:");
		ImGui::TextColored(IMGUI_YELLOW, "%i", max_fps);
		ImGui::SliderInt("Max FPS", &max_fps, 0, 120);

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	}

	ImGui::End();
}

void PanelConfiguration::Log(const float* fps, const float ms)
{
	for (int i = 0; i < fps_log.size() - 1; i++)
	{
		fps_log[i] = fps_log[i + 1];
	}
	fps_log[fps_log.size() - 1] = *fps;

	for (int i = 0; i < ms_log.size() - 1; i++)
	{
		ms_log[i] = ms_log[i + 1];
	}
	ms_log[ms_log.size() - 1] = ms;
}

unsigned int PanelConfiguration::GetMaxFPS()
{
	return max_fps;
}