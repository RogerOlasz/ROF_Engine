#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"

#include "ComponentCamera.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Camera3D");

	camera = new ComponentCamera(nullptr, 0);
	camera->SetNearPlane(0.1f);
	camera->SetFarPlane(600.0f);
	camera->SetPos(vec(20.0f, 20.0f, 20.0f));

	LookAt(vec(0.0f, 0.0f, 0.0f));
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init()
{
	bool ret = true;

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

	RELEASE(camera);

	return true;
}

bool ModuleCamera3D::Load(pugi::xml_node &config)
{
	vec tmp;
	tmp.x = config.child("Position").attribute("X").as_float(20.0f);
	tmp.y = config.child("Position").attribute("Y").as_float(20.0f);
	tmp.z = config.child("Position").attribute("Z").as_float(20.0f);

	camera->SetPos(tmp);

	return true;
}

bool ModuleCamera3D::Save(pugi::xml_node &config) const
{
	vec tmp = camera->GetPos();

	pugi::xml_node tmp_pos = config.append_child("Position");

	tmp_pos.append_attribute("X") = tmp.x;
	tmp_pos.append_attribute("Y") = tmp.y;
	tmp_pos.append_attribute("Z") = tmp.z;

	return true;
}

ComponentCamera* ModuleCamera3D::GetCamera() const
{
	return camera;
}

update_status ModuleCamera3D::Update(float dt)
{	
	if (controls_disabled == false)
	{
		Move(dt);
		ReferenceOrbit();
	}	

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
	tmp = camera->GetPos();

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
		tmp += camera->GetFront() * speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		tmp -= camera->GetFront()  * speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		tmp -= camera->GetWorldRight() * speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		tmp += camera->GetWorldRight() * speed;
	}

	reference += (tmp - camera->GetPos());
	camera->SetPos(tmp);
}

void ModuleCamera3D::ReferenceOrbit()
{
	int dx;
	int dy;

	if (invert_controls_x)
	{
		dx = -App->input->GetMouseXMotion();
	}
	else
	{
		dx = App->input->GetMouseXMotion();
	}
	
	if (invert_controls_y)
	{
		dy = -App->input->GetMouseYMotion();
	}
	else
	{
		dy = App->input->GetMouseYMotion();
	}

	float sensitivity = 0.005f;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		vec position = (camera->GetPos() - reference);

		if (dx != 0)
		{
			Quat x_quat(camera->GetUp(), dx * sensitivity);
			position = x_quat.Transform(position);
		}
		
		if (dy != 0)
		{
			Quat y_quat(camera->GetWorldRight(), dy * sensitivity);
			position = y_quat.Transform(position);
		}

		camera->SetPos(position + reference);
		LookAt(reference);
	}
}

void ModuleCamera3D::SetDefault()
{
	camera->SetNearPlane(0.1f);
	camera->SetFarPlane(600.0f);
	camera->SetPos(vec(20.0f, 20.0f, 20.0f));
	camera->SetFrame(vec(20.0f, 20.0f, 20.0f), vec::unitZ, vec::unitY);

	LookAt(vec(0.0f, 0.0f, 0.0f));
}