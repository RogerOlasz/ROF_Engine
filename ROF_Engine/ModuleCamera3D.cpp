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
	camera->camera_frustum.SetViewPlaneDistances(0.1f, 600.0f);

	LookAt(vec(0, 0, 0));
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

ComponentCamera* ModuleCamera3D::GetCamera() const
{
	return camera;
}

update_status ModuleCamera3D::Update(float dt)
{	
	Move(dt);
	ReferenceOrbit();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::LookAt(const vec &position)
{
	camera->LookAt(position);
	reference = position;
}

void ModuleCamera3D::Move(float dt)
{
	vec tmp(0.0f, 0.0f, 0.0f);
	tmp = camera->camera_frustum.Pos();

	float speed = 15.0f * dt;
	
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed = 45.0f * dt;
	}		

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{
		tmp.y += speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		tmp.y -= speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		tmp += camera->camera_frustum.Front() * speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		tmp -= camera->camera_frustum.Front() * speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		tmp -= camera->camera_frustum.WorldRight() * speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		tmp += camera->camera_frustum.WorldRight() * speed;
	}

	reference += (tmp - camera->camera_frustum.Pos());
	camera->camera_frustum.SetPos(tmp);
}

void ModuleCamera3D::ReferenceOrbit()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float sensitivity = 0.005f;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		vec position = (camera->camera_frustum.Pos() - reference);

		if (dx != 0)
		{
			Quat x_quat(camera->camera_frustum.Up(), dx * sensitivity);
			position = x_quat.Transform(position);
		}
		
		if (dy != 0)
		{
			Quat y_quat(camera->camera_frustum.WorldRight(), dy * sensitivity);
			position = y_quat.Transform(position);
		}

		camera->camera_frustum.SetPos(position + reference);
		LookAt(reference);
	}
}