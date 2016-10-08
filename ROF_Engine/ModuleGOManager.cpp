#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "GameObject.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGOManager::~ModuleGOManager()
{

}

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
	//Here i have to delete all checked GOs with to_delete

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGOManager::CleanUp()
{
	return true;
}

GameObject* ModuleGOManager::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* new_go = new GameObject(name, parent);
	
	if (parent == NULL)
	{
		parent = root;
	}

	return new_go;
}

void ModuleGOManager::RemoveGameObjects(GameObject* go_to_delete)
{
	go_to_delete->to_delete = true;
}
