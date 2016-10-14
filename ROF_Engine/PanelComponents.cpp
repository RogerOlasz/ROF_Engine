#include "PanelComponents.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "Mesh.h"
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
	ImGui::Begin("Components", &active);

	ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.00f, 1.00f), "GameObject: ");
	ImGui::SameLine();
	ImGui::Text(selected_go->GetName());

	for (std::list<Component*>::iterator tmp = selected_go->components.begin(); tmp != selected_go->components.end(); tmp++)
	{
		if ((*tmp)->GetType() == Component::Types::Transformation)
		{			
			//If actual game object is diferent of last one it must to set transformation
			if (last_go != selected_go)
			{
				pos.Set(((ComponentTransformation*)(*tmp))->GetPosition().x, ((ComponentTransformation*)(*tmp))->GetPosition().y, ((ComponentTransformation*)(*tmp))->GetPosition().z);
				sca.Set(((ComponentTransformation*)(*tmp))->GetScale().x, ((ComponentTransformation*)(*tmp))->GetScale().y, ((ComponentTransformation*)(*tmp))->GetScale().z);
				rot.Set(((ComponentTransformation*)(*tmp))->GetRotation().x, ((ComponentTransformation*)(*tmp))->GetRotation().y, ((ComponentTransformation*)(*tmp))->GetRotation().z);
			}

			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: "); 
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentTransformation*)(*tmp))->GetID());
				ImGui::Separator();
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

		if ((*tmp)->GetType() == Component::Types::Geometry)
		{		
			if (ImGui::CollapsingHeader(((ComponentMesh*)(*tmp))->name.c_str()))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentMesh*)(*tmp))->GetID());
				ImGui::Separator();
				ImGui::Text("Number of vertex(Indices): %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_indices);
				ImGui::Text("Number of vertex in memory: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_vertices);
				ImGui::Text("Number of normals: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_normals);
				ImGui::Text("Number of texture coordinates: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_tex_coord);
			}									
		}

		if ((*tmp)->GetType() == Component::Types::Material)
		{
			if (ImGui::CollapsingHeader("Material"))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentMaterial*)(*tmp))->GetID());
				ImGui::Separator();
				ImGui::Image((void*)((ComponentMaterial*)(*tmp))->GetTexture(), ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
				ImGui::Text("%s%s", "Texture path: ", ((ComponentMaterial*)(*tmp))->tex_path);
			}
		}
	}
 last_go = selected_go;
 ImGui::End();
}