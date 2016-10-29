#include "PanelComponents.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
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

	if (ImGui::Button("Center view"))
	{
		App->camera->LookAt(selected_go->transform->GetPosition());
	}
	ImGui::Checkbox("Static test", &selected_go->static_go);

	//If actual game object is diferent of last one it must to set transformation
	if (last_go != selected_go || selected_go->static_go == true)
	{
		pos.Set(selected_go->transform->GetPosition().x, selected_go->transform->GetPosition().y, selected_go->transform->GetPosition().z);
		sca.Set(selected_go->transform->GetScale().x, selected_go->transform->GetScale().y, selected_go->transform->GetScale().z);
		rot.Set(selected_go->transform->GetRotation().x, selected_go->transform->GetRotation().y, selected_go->transform->GetRotation().z);
	}

	if (ImGui::CollapsingHeader(selected_go->transform->name.c_str()))
	{
		ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", selected_go->transform->GetID());
		ImGui::Separator();
		
		if (ImGui::DragFloat3("Postion", pos.ptr()))
		{
			selected_go->transform->SetPos(pos.x, pos.y, pos.z);
		}
		if (ImGui::DragFloat3("Scale", sca.ptr(), 0.01f))
		{
			selected_go->transform->SetScale(sca.x, sca.y, sca.z);
		}
		if (ImGui::DragFloat3("Rotation", rot.ptr()))
		{
			selected_go->transform->SetRotEuler(rot.x, rot.y, rot.z);
		}	
		
	}

	for (std::vector<Component*>::iterator tmp = selected_go->components.begin(); tmp != selected_go->components.end(); tmp++)
	{
		if ((*tmp)->GetType() == Component::Type::Geometry)
		{
			if (ImGui::CollapsingHeader(((ComponentMesh*)(*tmp))->name.c_str()))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentMesh*)(*tmp))->GetID());

				ImGui::SameLine(ImGui::GetWindowWidth() - 105);
				ImGui::Text("Active:");
				ImGui::SameLine(ImGui::GetWindowWidth() - 50);
				ImGui::Checkbox(((ComponentMesh*)(*tmp))->is_active.c_str(), &((ComponentMesh*)(*tmp))->active);

				ImGui::Checkbox(((ComponentMesh*)(*tmp))->wire.c_str(), &((ComponentMesh*)(*tmp))->wirefr);

				ImGui::Checkbox("AABB", &selected_go->aabb_debug);
				ImGui::Checkbox("OBB", &selected_go->obb_debug);
				ImGui::Separator();

				ImGui::Text("Number of vertex(Indices): %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_indices);
				ImGui::Text("Number of vertex in memory: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_vertices);
				ImGui::Text("Number of normals: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_normals);
				ImGui::Text("Number of texture coordinates: %d", ((ComponentMesh*)(*tmp))->GetMesh()->num_tex_coord);
			}
		}

		if ((*tmp)->GetType() == Component::Type::Material)
		{
			if (ImGui::CollapsingHeader(((ComponentMaterial*)(*tmp))->name.c_str()))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentMaterial*)(*tmp))->GetID());

				ImGui::Separator();
				ImGui::Image((ImTextureID*)((ComponentMaterial*)(*tmp))->GetTexture(), ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.0f, 0.6f, 0.6f, 1.0f));
				ImGui::Text("%s%s", "Texture path: ", ((ComponentMaterial*)(*tmp))->tex_path.c_str());
			}
		}

		if ((*tmp)->GetType() == Component::Type::Camera)
		{
			if (ImGui::CollapsingHeader(((ComponentCamera*)(*tmp))->name.c_str()))
			{
				near_plane = ((ComponentCamera*)(*tmp))->GetNearPlane();
				far_plane = ((ComponentCamera*)(*tmp))->GetFarPlane();
				field_of_view = ((ComponentCamera*)(*tmp))->GetFOV();
				aspect_ratio = ((ComponentCamera*)(*tmp))->GetAspectRatio();

				ImGui::Checkbox("Camera frustrum", &((ComponentCamera*)(*tmp))->debug_draw);
				
				if(ImGui::Checkbox("Use camera", &render_camera))
				{
					App->renderer3D->camera = ((ComponentCamera*)(*tmp));
					App->renderer3D->update_proj_matrix = true;
					App->camera->controls_disabled = true;
				}

				if(render_camera == false)
				{
					App->renderer3D->camera = App->camera->GetCamera();
					App->camera->controls_disabled = false;
					App->renderer3D->update_proj_matrix = true;
				}

				ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
				ImGui::SameLine();
				ImGui::Text("%d", ((ComponentCamera*)(*tmp))->GetID());
				
				if (ImGui::DragFloat("Near plane", &near_plane, 0.1f))
				{
					((ComponentCamera*)(*tmp))->SetNearPlane(near_plane);
					if (render_camera)
					{
						App->renderer3D->update_proj_matrix = true;
					}
				}
				if (ImGui::DragFloat("Far plane", &far_plane, 0.1f))
				{
					((ComponentCamera*)(*tmp))->SetFarPlane(far_plane);
					if (render_camera)
					{
						App->renderer3D->update_proj_matrix = true;
					}
				}
				if (ImGui::DragFloat("Field of view", &field_of_view, 0.1f))
				{
					((ComponentCamera*)(*tmp))->SetFOV(field_of_view);
					if (render_camera)
					{
						App->renderer3D->update_proj_matrix = true;
					}
				}
				if (ImGui::DragFloat("Aspect ratio", &aspect_ratio, 0.01f))
				{
					((ComponentCamera*)(*tmp))->SetAspectRatio(aspect_ratio);
					if (render_camera)
					{
						App->renderer3D->update_proj_matrix = true;
					}
				}
			}
		}
	}
	last_go = selected_go;
	ImGui::End();
}