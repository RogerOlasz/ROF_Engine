#include "PanelComponents.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ImGui/imgui.h"

PanelComponents::PanelComponents() : Panel("Components")
{
	active = false;
}

PanelComponents::~PanelComponents()
{

}

void PanelComponents::Draw(GameObject* selected_go)
{
	ImGui::SetNextWindowPos(ImVec2(((App->window->GetWindowSize().x - 335)), 25));
	ImGui::SetNextWindowSize(ImVec2(330, 610));
	ImGui::Begin(selected_go->GetName(), &active);

	//TODO 
	for (std::list<Component*>::iterator tmp = selected_go->components.begin(); tmp != selected_go->components.end(); tmp++)
	{
		if ((*tmp)->GetType() == Component::Types::Transformation)
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				if (ImGui::DragFloat3("Postion", ((ComponentTransformation*)(*tmp))->GetPosition().ptr()))
				{
					((ComponentTransformation*)(*tmp))->BuildTransMatrix();
				}
				if (ImGui::DragFloat3("Scale", ((ComponentTransformation*)(*tmp))->GetScale().ptr()))
				{
					((ComponentTransformation*)(*tmp))->BuildTransMatrix();
				}
				if (ImGui::DragFloat3("Rotation", ((ComponentTransformation*)(*tmp))->GetRotation().ptr()))
				{
					((ComponentTransformation*)(*tmp))->BuildTransMatrix();
				}				
			}
		}
	}
 ImGui::End();
}