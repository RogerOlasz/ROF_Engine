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

			vec pos(vec(((ComponentTransformation*)(*tmp))->GetPosition().x, ((ComponentTransformation*)(*tmp))->GetPosition().y, ((ComponentTransformation*)(*tmp))->GetPosition().z));
			vec sca(vec(((ComponentTransformation*)(*tmp))->GetScale().x, ((ComponentTransformation*)(*tmp))->GetScale().y, ((ComponentTransformation*)(*tmp))->GetScale().z));
			vec rot(vec(((ComponentTransformation*)(*tmp))->GetRotation().x, ((ComponentTransformation*)(*tmp))->GetRotation().y, ((ComponentTransformation*)(*tmp))->GetRotation().z));

			if (ImGui::CollapsingHeader("Transform"))
			{
				if (ImGui::DragFloat3("Postion", pos.ptr()))
				{
					((ComponentTransformation*)(*tmp))->SetPos(pos.x, pos.y, pos.z);
				}
				if (ImGui::DragFloat3("Scale", sca.ptr()))
				{
					((ComponentTransformation*)(*tmp))->SetScale(sca.x, sca.y, sca.z);
				}
				if (ImGui::DragFloat3("Rotation", rot.ptr()))
				{
					((ComponentTransformation*)(*tmp))->SetRotEuler(rot.x, rot.y, rot.z);
				}				
			}
		}
	}
 ImGui::End();
}