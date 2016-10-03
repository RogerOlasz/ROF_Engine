#include "Globals.h"
#include "Application.h"
#include "ModuleGeometry.h"

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleGeometry::~ModuleGeometry()
{}

// Called before render is available
bool ModuleGeometry::Init()
{
	return true;
}

// PreUpdate: clear buffer
update_status ModuleGeometry::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGeometry::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGeometry::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGeometry::CleanUp()
{
	return true;
}

void ModuleGeometry::LoadGeometry(const char *file_name)
{

}