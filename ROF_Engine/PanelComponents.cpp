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
#include "ModuleGOManager.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"
#include "ImGui/imgui.h"

PanelComponents::PanelComponents() : Panel("Components")
{
	active = false;
	set_size = true;

	last_go = nullptr;
}

PanelComponents::~PanelComponents()
{

}

void PanelComponents::Draw(GameObject* selected_go)
{
	if (selected_go != nullptr)
	{
		if (last_go != selected_go)
		{
			if (last_go != nullptr)
			{
				if (last_go->GetComponentByType(Component::Type::Geometry) != nullptr)
				{
					((ComponentMesh*)last_go->GetComponentByType(Component::Type::Geometry))->wirefr = false;
				}
			}
		}

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

		if (ImGui::BeginMenu("Add component"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				selected_go->CreateComponent(Component::Type::Camera);
			}

			ImGui::EndMenu();
		}

		if (ImGui::Button("Center view"))
		{
			App->camera->LookAt(selected_go->transform->GetPosition());
		}

		if (ImGui::Checkbox("Static test", &selected_go->static_go))
		{
			if (selected_go->children.size() != 0)
			{
				static_ask = selected_go->static_go;
			}
		}

		if (static_ask)
		{
			ImGuiWindowFlags window_flags = (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
			ImGui::SetNextWindowPos(ImVec2((App->window->GetWindowSize().x / 2) - 190, (App->window->GetWindowSize().y / 2) - 50));
			ImGui::Begin("Hierarchy problem", nullptr, window_flags);
			ImGui::Text("Convert all %s children to static?", selected_go->GetName());

			ImVec2 size = ImVec2(80.0f, 20.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 0.75f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.75f, 0.0f, 0.75f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.75f, 0.0f, 1.0f));
			if (ImGui::Button("Yes", size))
			{
				selected_go->static_children = true;
				static_ask = false;
			}
			ImGui::PopStyleColor(3);

			ImGui::SameLine(0.0f, 100.0f);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 0.75f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.0f, 0.0f, 0.75f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.75f, 0.0f, 0.0f, 1.0f));
			if (ImGui::Button("No", size))
			{
				selected_go->static_children = false;
				static_ask = false;
			}
			ImGui::PopStyleColor(3);

			ImGui::End();
		}

		//If actual game object is diferent of last one it must to set transformation
		if (last_go != selected_go || selected_go->static_go == true)
		{
			pos.Set(selected_go->transform->GetPosition().x, selected_go->transform->GetPosition().y, selected_go->transform->GetPosition().z);
			sca.Set(selected_go->transform->GetScale().x, selected_go->transform->GetScale().y, selected_go->transform->GetScale().z);
			rot.Set(selected_go->transform->GetRotation().x, selected_go->transform->GetRotation().y, selected_go->transform->GetRotation().z);
		}

		DrawTransformation(selected_go->transform, selected_go);

		for (std::vector<Component*>::iterator tmp = selected_go->components.begin(); tmp != selected_go->components.end(); tmp++)
		{
			if ((*tmp)->GetType() == Component::Type::Geometry)
			{
				if (last_go == selected_go)
				{
					((ComponentMesh*)(*tmp))->wirefr = true;
				}
				else
				{
					if (last_go != nullptr)
					{
						if (last_go->GetComponentByType(Component::Type::Geometry) != nullptr)
						{
							((ComponentMesh*)last_go->GetComponentByType(Component::Type::Geometry))->wirefr = false;
						}
					}
				}

				DrawMesh(((ComponentMesh*)(*tmp)), selected_go);
			}

			if ((*tmp)->GetType() == Component::Type::Material)
			{
				DrawMaterial(((ComponentMaterial*)(*tmp)), selected_go);
			}

			if ((*tmp)->GetType() == Component::Type::Camera)
			{
				DrawCamera(((ComponentCamera*)(*tmp)), selected_go);
			}
		}
		last_go = selected_go;
		ImGui::End();
	}
	else
	{
		if (last_go != nullptr)
		{
			if (last_go->GetComponentByType(Component::Type::Geometry) != nullptr)
			{
				((ComponentMesh*)last_go->GetComponentByType(Component::Type::Geometry))->wirefr = false;
			}
		}
	}
}

void PanelComponents::DrawTransformation(ComponentTransformation* go_transform, GameObject* go_selected)
{
	if (ImGui::CollapsingHeader(go_transform->name.c_str()))
	{
		ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", go_transform->GetID());
		ImGui::Separator();

		if (ImGui::DragFloat3("Postion", pos.ptr()))
		{
			go_transform->SetPos(pos.x, pos.y, pos.z);
		}
		if (ImGui::DragFloat3("Scale", sca.ptr(), 0.01f))
		{
			go_transform->SetScale(sca.x, sca.y, sca.z);
		}
		if (ImGui::DragFloat3("Rotation", rot.ptr()))
		{
			go_transform->SetRotEuler(rot.x, rot.y, rot.z);
		}
	}
}

void PanelComponents::DrawMesh(ComponentMesh* go_mesh, GameObject* go_selected)
{
	if (ImGui::CollapsingHeader(go_mesh->name.c_str()))
	{
		ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", (go_mesh->GetID()));
		ImGui::Separator();

		ImGui::SameLine(ImGui::GetWindowWidth() - 105);
		ImGui::Text("Active:");
		ImGui::SameLine(ImGui::GetWindowWidth() - 50);
		ImGui::Checkbox(go_mesh->is_active.c_str(), &go_mesh->active);

		ImGui::Checkbox(go_mesh->wire.c_str(), &go_mesh->wirefr);

		ImGui::Checkbox("AABB", &go_selected->aabb_debug);
		ImGui::Checkbox("OBB", &go_selected->obb_debug);
		ImGui::Separator();

		ImGui::Text("Number of vertex(Indices): %d", ((ResourceMesh*)go_mesh->GetResource())->num_indices);
		ImGui::Text("Number of vertex in memory: %d", ((ResourceMesh*)go_mesh->GetResource())->num_vertices);
		ImGui::Text("Number of normals: %d", ((ResourceMesh*)go_mesh->GetResource())->num_normals);
		ImGui::Text("Number of texture coordinates: %d", ((ResourceMesh*)go_mesh->GetResource())->num_tex_coord);
	}
}

void PanelComponents::DrawMaterial(ComponentMaterial* go_material, GameObject* go_selected)
{
	if (ImGui::CollapsingHeader(go_material->name.c_str()))
	{
		ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", go_material->GetID());

		ImGui::Separator();
		if (((ResourceMaterial*)go_material->GetResource())->texture)
		{
			ImGui::Image((ImTextureID*)((ResourceMaterial*)go_material->GetResource())->texture->id_texture, ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.0f, 0.6f, 0.6f, 1.0f));
			ImGui::Text("%s%s", "Texture path: ", ((ResourceMaterial*)go_material->GetResource())->texture->GetOriginFile());
		}
		ImGui::Text("Material color:");
		ImGui::SameLine(120, 0);
		ImVec4 color(((ResourceMaterial*)go_material->GetResource())->diffuse_color.r, ((ResourceMaterial*)go_material->GetResource())->diffuse_color.g, ((ResourceMaterial*)go_material->GetResource())->diffuse_color.b, ((ResourceMaterial*)go_material->GetResource())->diffuse_color.a);
		ImGui::ColorButton(color);
		if (ImGui::BeginPopupContextItem("Change material color"))
		{
			ImGui::Text("Edit color");
			if (ImGui::ColorEdit3("##edit", (float*)&color))
			{
				((ResourceMaterial*)go_material->GetResource())->diffuse_color.Set(color.x, color.y, color.z);
			}
			ImGui::EndPopup();
		}
	}
}

void PanelComponents::DrawCamera(ComponentCamera* go_camera, GameObject* go_selected)
{
	if (ImGui::CollapsingHeader(go_camera->name.c_str()))
	{
		near_plane = go_camera->GetNearPlane();
		far_plane = go_camera->GetFarPlane();
		field_of_view = go_camera->GetFOV();
		aspect_ratio = go_camera->GetAspectRatio();

		ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", go_camera->GetID());
		ImGui::Separator();

		ImGui::Checkbox("Camera frustrum", &go_camera->debug_draw);
		if (ImGui::Checkbox("Camera culling", &go_camera->frustum_culling))
		{
			App->go_manager->AddCameraCulling(go_camera, go_camera->frustum_culling);
		}

		if (ImGui::Checkbox("Use camera", &render_camera))
		{
			App->renderer3D->camera = go_camera;
			App->renderer3D->update_proj_matrix = true;
			App->camera->controls_disabled = true;
		}

		if (render_camera == false)
		{
			App->renderer3D->camera = App->camera->GetCamera();
			App->camera->controls_disabled = false;
			App->renderer3D->update_proj_matrix = true;
		}

		if (ImGui::DragFloat("Near plane", &near_plane, 0.1f))
		{
			go_camera->SetNearPlane(near_plane);
			if (render_camera)
			{
				App->renderer3D->update_proj_matrix = true;
			}
		}
		if (ImGui::DragFloat("Far plane", &far_plane, 0.1f))
		{
			go_camera->SetFarPlane(far_plane);
			if (render_camera)
			{
				App->renderer3D->update_proj_matrix = true;
			}
		}
		if (ImGui::DragFloat("Field of view", &field_of_view, 0.1f))
		{
			go_camera->SetFOV(field_of_view);
			if (render_camera)
			{
				App->renderer3D->update_proj_matrix = true;
			}
		}
		if (ImGui::DragFloat("Aspect ratio", &aspect_ratio, 0.01f))
		{
			go_camera->SetAspectRatio(aspect_ratio);
			if (render_camera)
			{
				App->renderer3D->update_proj_matrix = true;
			}
		}
	}
}