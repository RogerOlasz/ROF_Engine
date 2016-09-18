#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ImGui\imgui.h"
#include"Random.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");

	//INITIALIZATION OF VARIABLES---------------------------------
	bool ret = true;

	App->camera->Move(vec(5, 5, 5));
	App->camera->LookAt(vec(0, 0, 0));

	//Generate 20 times a number between min and max test
	Random rand;
	int ran[40];
	for (int i = 0; i < 40; i++)
		{
			ran[i] = rand.RandomInt(2, 8);
			LOG("Rand number: %d", ran[i]);
		}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	Plane_P p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	//Creating an ImGui Simple UI
	if(ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Close", NULL))
		{
			return UPDATE_STOP;
		}

	 ImGui::EndMainMenuBar();
	}

	ImGui::ShowTestWindow();

	return UPDATE_CONTINUE;
}
