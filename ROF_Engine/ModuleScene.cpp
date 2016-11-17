#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleGOManager.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ModuleEditor.h"
#include "Mesh.h"
#include "Primitive.h"
#include "DebugPainter.h"
#include "ImGui/imgui.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Scene");
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	picking = LineSegment(vec::zero, vec::unitX);

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	Plane_P p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && ImGui::IsMouseHoveringAnyWindow() == false)
	{
		float2 mouse_click(App->input->GetMouseX(), App->input->GetMouseY());

		float mc_normalized_x = -(1.0f - (mouse_click.x * 2.0f) / App->window->GetWindowSize().x);
		float mc_normalized_y = (1.0f - (mouse_click.y * 2.0f) / App->window->GetWindowSize().y);

		picking = App->renderer3D->GetRenderingCamera()->GetFrustum().UnProjectLineSegment(mc_normalized_x, mc_normalized_y);
		//LOG("Segment point A: %.2f point B: %.2f", picking.a, picking.b);

		std::vector<GameObject*> candidates;
		App->go_manager->FindCandidates(picking, candidates);
		LOG("Candidates: %d", candidates.size());

		if (candidates.size() == 0)
		{
			App->editor->SetSelectedGO(nullptr);
		}

		GameObject* picked = nullptr;
		float min_dist = App->camera->GetCamera()->GetFarPlane();

		for (uint i = 0; i < candidates.size(); i++)
		{
			GameObject* go = candidates[i];
			ComponentMesh* mesh = (ComponentMesh*)go->GetComponentByType(Component::Type::Geometry);

			for (uint i = 0; i < mesh->GetMesh()->num_indices; i+=3)
			{
				vec vertex_1 = mesh->GetMesh()->vertices[mesh->GetMesh()->indices[i]];
				vec vertex_2 = mesh->GetMesh()->vertices[mesh->GetMesh()->indices[i + 1]];
				vec vertex_3 = mesh->GetMesh()->vertices[mesh->GetMesh()->indices[i + 2]];

				Triangle to_test(vertex_1, vertex_2, vertex_3);
				float hit_distance = 0;
				vec hit_point = vec::zero;

				if (to_test.Intersects(picking, &hit_distance, &hit_point))
				{
					if (hit_distance < min_dist)
					{
						picked = go;
						min_dist = hit_distance;
					}					
				}
			}
		}
		App->editor->SetSelectedGO(picked);
	}

	DebugDraw(picking, Red);

	return UPDATE_CONTINUE;
}