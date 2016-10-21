#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"

#include "ComponentCamera.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Camera3D");

	camera = new ComponentCamera(nullptr, 0);

	camera->camera_frustum.SetPos(vec(50.0f, 50.0f, 50.0f));
	camera->camera_frustum.SetFront(vec::unitZ);
	camera->camera_frustum.SetUp(vec::unitY);
	
	camera->camera_frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	camera->camera_frustum.SetPerspective(DEGTORAD * 60.0f, DEGTORAD * 60.0f);

	LookAt(vec(0, 0, 0));

	camera->proj_matrix_update = true;
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init()
{
	bool ret = true;
	App->renderer3D->camera = camera;

	return ret;
}

bool ModuleCamera3D::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	App->renderer3D->camera = nullptr;
	RELEASE(camera);

	return true;
}

update_status ModuleCamera3D::Update(float dt)
{
	

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::LookAt(const vec &position)
{
	camera->LookAt(position);
}

