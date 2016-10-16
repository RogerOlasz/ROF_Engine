#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleEditor.h"
#include "ModuleGeometry.h"
#include "ModuleFileSystem.h"
#include "ModuleGOManager.h"

using namespace std;

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	editor = new ModuleEditor(this);
	geometry = new ModuleGeometry(this);
	physfs = new ModuleFileSystem(this);
	go_manager = new ModuleGOManager(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(editor); //To recieve all module LOGs
	AddModule(physfs);
	AddModule(go_manager);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);	
	AddModule(geometry);	

	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);	

	// Strings
	sprintf_s(app_name, SHORT_STRING, TITLE);
	sprintf_s(organization, SHORT_STRING, "CITM");
}

Application::~Application()
{
	list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete (*item);
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	
	to_fps.Start();
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
	capped_ms = (1000.f / max_fps);
	max_fps = App->editor->GetMaxFPS();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	float frame_ms = ms_timer.Read();
	if (frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - frame_ms);
	}

	fps++;
	App->editor->LogFPS(&fps_last_sec, (float)ms_timer.Read());

	if (to_fps.Read() >= 1000.f)
	{
		to_fps.Start();
		fps_last_sec = fps;
		fps = 0;
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	return ret;
}

bool Application::RequestBrowser(const char* link)
{
	ShellExecuteA(0, 0, "chrome.exe", link, 0, SW_SHOWMAXIMIZED);

	return true;
}

void Application::Log(const char* log)
{
	App->editor->Log(log);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

const char *Application::GetAppName() { return app_name; }
const char *Application::GetOrganization() { return organization; }