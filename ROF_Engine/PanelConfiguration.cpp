#include "PanelConfiguration.h"

PanelConfiguration::PanelConfiguration()
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

void PanelConfiguration::Draw(bool* open)
{
	ImGui::Begin("Configuration", open, ImVec2(500, 200));

	if (ImGui::CollapsingHeader("Application"))
	{
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
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