#include "PanelComponents.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "Mesh.h"
#include "ImGui/imgui.h"

PanelComponents::PanelComponents() : Panel("Components")
{
	active = false;
	set_size = true;
}

PanelComponents::~PanelComponents()
{

}

void PanelComponents::Draw(GameObject* selected_go)
{	
	ImGui::SetNextWindowPos(ImVec2(((App->window->GetWindowSize().x - 335)), 25));
	
	if (set_size == true)
	{
		ImGui::SetNextWindowSize(ImVec2(330, 610));
		set_size = false;
	}
	
	ImGui::Begin("Components", &active);

	ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.00f, 1.00f), "GameObject: ");
	ImGui::SameLine();
	ImGui::Text(selected_go->GetName());

	for (std::vector<Component*>::iterator tmp = selected_go->components.begin(); tmp != selected_go->components.end(); tmp++)
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

			if (ImGui::CollapsingHeader(((ComponentTransformation*)(*tmp))->name.c_str()))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: "); 
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentTransformation*)(*tmp))->GetID());
				ImGui::Separator();

				if (ImGui::DragFloat3("Postion", pos.ptr()))
				{
					((ComponentTransformation*)(*tmp))->SetPos(pos.x, pos.y, pos.z);
				}
				if (ImGui::DragFloat3("Scale", sca.ptr(), 0.01f))
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

				ImGui::SameLine(ImGui::GetWindowWidth() - 80);
				ImGui::Text("Active: ");
				ImGui::SameLine(ImGui::GetWindowWidth() - 25);
				ImGui::Checkbox(((ComponentMesh*)(*tmp))->is_active.c_str(), &((ComponentMesh*)(*tmp))->active);

				ImGui::Text("Wireframe: ");
				ImGui::SameLine(ImGui::GetWindowWidth() - 25);
				ImGui::Checkbox(((ComponentMesh*)(*tmp))->wire.c_str(), &((ComponentMesh*)(*tmp))->wirefr);
				ImGui::Separator();

				ImGui::Text("Number of vertex(Indices): %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_indices);
				ImGui::Text("Number of vertex in memory: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_vertices);
				ImGui::Text("Number of normals: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_normals);
				ImGui::Text("Number of texture coordinates: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_tex_coord);
			}									
		}

		if ((*tmp)->GetType() == Component::Types::Material)
		{
			if (ImGui::CollapsingHeader(((ComponentMaterial*)(*tmp))->name.c_str()))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentMaterial*)(*tmp))->GetID());

				ImGui::Separator();
				ImGui::Image((void*)((ComponentMaterial*)(*tmp))->GetTexture(), ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.0f, 0.6f, 0.6f, 1.0f));
				ImGui::Text("%s%s", "Texture path: ", ((ComponentMaterial*)(*tmp))->tex_path);
			}
		}

		if ((*tmp)->GetType() == Component::Types::Camera)
		{
			if (ImGui::CollapsingHeader(((ComponentCamera*)(*tmp))->name.c_str()))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentCamera*)(*tmp))->GetID());

				ImGui::DragFloat("Near plane", &((ComponentCamera*)(*tmp))->camera_frustum.nearPlaneDistance, 0.1f);
				ImGui::DragFloat("Far plane", &((ComponentCamera*)(*tmp))->camera_frustum.farPlaneDistance, 0.1f);
				if (ImGui::DragFloat("Field of view", &((ComponentCamera*)(*tmp))->fov, 0.1f))
				{
					((ComponentCamera*)(*tmp))->SetFOV(((ComponentCamera*)(*tmp))->fov);
				}
				if (ImGui::DragFloat("Aspect ratio", &((ComponentCamera*)(*tmp))->aspect_ratio, 0.1f))
				{
					((ComponentCamera*)(*tmp))->SetAspectRatio(((ComponentCamera*)(*tmp))->aspect_ratio);
				}
			}
		}
	}
 last_go = selected_go;
 ImGui::End();
}