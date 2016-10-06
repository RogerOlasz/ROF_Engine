#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "GameObject.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleGOManager::~ModuleGOManager()
{}

// Called before render is available
bool ModuleGOManager::Init()
{
	root = new GameObject("Root", nullptr);

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

//GameObject* ModuleGOManager::GetParent()
//{
//
//}

GameObject* ModuleGOManager::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* new_go = new GameObject(name, parent);

	return new_go;
}

void ModuleGOManager::RemoveGameObject(GameObject* to_delete)
{

}

void ModuleGOManager::ChangeParent(GameObject* to_move, GameObject* new_parnet)
{

}