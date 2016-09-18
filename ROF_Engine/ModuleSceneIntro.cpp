#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ImGui\imgui.h"

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

	App->camera->Move(vec3(5, 5, 5));
	App->camera->LookAt(vec3(0, 0, 0));

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

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	//Creating an ImGui Simple UI
	if(ImGui::BeginMenuBar())
	{

		if (ImGui::BeginMenu("ROF Engine"))
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

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	//Effects of collisions, sensors and checkpoints------------------------------------
}

void  ModuleSceneIntro::Create_Bodys_World(float size_x, float size_y, float size_z, float pos_x, float pos_y, float pos_z, Cube primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass, bool sensor, bool rotation, float r_x, float r_y, float r_z, int angle, bool num_primitive, int num_prim)
{
	primitive_cube[PB_Num].Create_Cube(size_x, size_y, size_z, pos_x, pos_y, pos_z);
	if (rotation == true)
	{
		primitive_cube[PB_Num].SetRotation(angle, vec3(r_x, r_y, r_z));
	}

	if (sensor == true)
	{
		if (num_primitive == true)
		{
          PB_3D[PB_Num + num_prim] = App->physics->AddBody(primitive_cube[PB_Num], mass);
		  PB_3D[PB_Num + num_prim]->SetAsSensor(true);
		  PB_3D[PB_Num + num_prim]->collision_listeners.add(this);
		}
		else
		{
			PB_3D[PB_Num] = App->physics->AddBody(primitive_cube[PB_Num], mass);
			PB_3D[PB_Num]->SetAsSensor(true);
			PB_3D[PB_Num]->collision_listeners.add(this);
		}
		
	}
	else
	{
		if (num_primitive == true)
		{
			PB_3D[PB_Num + num_prim] = App->physics->AddBody(primitive_cube[PB_Num], mass);
		}
		else
		{
			PB_3D[PB_Num] = App->physics->AddBody(primitive_cube[PB_Num], mass);
		}
	}
	
}

void ModuleSceneIntro::Create_Circuit_World(float size_x, float size_y, float size_z, float pos_x, float pos_y, float pos_z, Cube primitive_cube[], PhysBody3D* PB_3D[], int init_Num, int final_num, float parameter, float mass, bool distance, bool height, bool axis_x, bool positive, int value)
{
	for (int i = init_Num, height_or_distance = parameter; i < final_num; i++)
	{
		if (height == true)
		{
           primitive_cube[i].Create_Cube(size_x, size_y, size_z, pos_x, height_or_distance, pos_z);
		   PB_3D[i] = App->physics->AddBody(primitive_cube[i], mass);
		   height_or_distance += 2;
		}
		else
		{
			if (axis_x == true)
			{
			   primitive_cube[i].Create_Cube(size_x, size_y, size_z, height_or_distance, pos_y, pos_z);
			   PB_3D[i] = App->physics->AddBody(primitive_cube[i], mass);
			   
			}
			else
			{
				primitive_cube[i].Create_Cube(size_x, size_y, size_z, pos_x, pos_y, height_or_distance);
				PB_3D[i] = App->physics->AddBody(primitive_cube[i], mass);
			}
			
             if (positive == true)
				 height_or_distance += value;
			 else
				 height_or_distance -= value;
		}
		
	}
}

void  ModuleSceneIntro::Create_Curves(float size_x, float size_y, float size_z, float pos_x_a, float pos_y_a, float pos_z_a, Cube primitive_cube[], PhysBody3D* PB_3D[], int init_Num, int final_num, float parameter_1, float parameter_2, float parameter_3, float parameter_4, float parameter_5, float value_parameter_1_z, float value_parameter_2_z, float value_parameter_3_z, float value_parameter_4_z, float value_parameter_5_z,
	float value_parameter_1_x, float value_parameter_2_x, float value_parameter_3_x, float value_parameter_4_x, float value_parameter_5_x)
{
	for (int i = init_Num, distance = pos_x_a, pos_z = pos_z_a; i < final_num; i++)
	{
		

		if (i == parameter_1)
		{
			pos_z += value_parameter_1_z;
			distance += value_parameter_1_x;
		}

		if (i == parameter_2)
		{
			pos_z += value_parameter_2_z;
			distance += value_parameter_2_x;
		}

		if (i == parameter_3)
		{
			pos_z += value_parameter_3_z;
			distance += value_parameter_3_x;
		}

		if (i == parameter_4)
		{
			pos_z += value_parameter_4_z;
			distance += value_parameter_4_x;
		}

		if (i == parameter_5)
		{
			pos_z += value_parameter_5_z;
			distance += value_parameter_5_x;
		}

		primitive_cube[i].Create_Cube(size_x, size_y, size_z, distance, pos_y_a, pos_z);
		PB_3D[i] = App->physics->AddBody(primitive_cube[i], 0);


	}
}


void  ModuleSceneIntro::Create_Cylinder_Bodys_World(float radius_a, float height_a, float pos_x, float pos_y, float pos_z, Cylinder primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass)
{
	primitive_cube[PB_Num].radius = radius_a;
	primitive_cube[PB_Num].height = height_a;
	primitive_cube[PB_Num].SetPos(pos_x, pos_y, pos_z);
	PB_3D[PB_Num] = App->physics->AddBody(primitive_cube[PB_Num], mass);
}

void  ModuleSceneIntro::Create_Sphere_Bodys_World(float radius_a, float pos_x, float pos_y, float pos_z, Sphere primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass, bool num_primitive, int num_prim)
{   
	primitive_cube[PB_Num].radius = radius_a;
	primitive_cube[PB_Num].SetPos(pos_x, pos_y, pos_z);
	if (num_primitive == true)
	{
		PB_3D[PB_Num + num_prim] = App->physics->AddBody(primitive_cube[PB_Num], mass);
	}
	else
	{
		PB_3D[PB_Num] = App->physics->AddBody(primitive_cube[PB_Num], mass);
	}
}