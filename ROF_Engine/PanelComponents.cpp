#include "PanelComponents.h"
#include "GameObject.h"
#include "ImGui/imgui.h"

PanelComponents::PanelComponents() : Panel("Components")
{

}

PanelComponents::~PanelComponents()
{

}

void PanelComponents::Draw(GameObject* selected_go)
{
	ImGui::Begin(selected_go->GetName(), &active);



	ImGui::End();
}