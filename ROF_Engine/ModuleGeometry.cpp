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

// Called before quitting
bool ModuleGeometry::CleanUp()
{

	return true;
}