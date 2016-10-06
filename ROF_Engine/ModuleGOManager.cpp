#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleGOManager::~ModuleGOManager()
{}

// Called before render is available
bool ModuleGOManager::Init()
{
	return true;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGOManager::CleanUp()
{
	return true;
}