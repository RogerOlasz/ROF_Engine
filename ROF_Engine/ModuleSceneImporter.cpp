#include "ModuleSceneImporter.h"

ModuleSceneImporter::ModuleSceneImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("SceneImporter");
}

// Destructor
ModuleSceneImporter::~ModuleSceneImporter()
{}

// Called before render is available
bool ModuleSceneImporter::Init()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool ModuleSceneImporter::CleanUp()
{
	
	return true;
}