#include "PanelConfiguration.h"

PanelConfiguration::PanelConfiguration()
{}

PanelConfiguration::~PanelConfiguration()
{}

void PanelConfiguration::Draw(bool* open)
{
	ImGui::Begin("Configuration", open, ImVec2(500, 200));

	ImGui::End();
}
