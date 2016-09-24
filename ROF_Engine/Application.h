#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleEditor.h"

//class Module;
//class ModuleWindow;
//class ModuleInput;
//class ModuleAudio;
//class ModuleSceneIntro;
//class ModuleRenderer3D;
//class ModuleCamera3D;
//class ModulePhysics3D;
//class ModuleEditor;

using namespace std;

class Application
{
public:
	ModuleWindow* window = NULL;
	ModuleInput* input = NULL;
	ModuleAudio* audio = NULL;
	ModuleSceneIntro* scene_intro = NULL;
	ModuleRenderer3D* renderer3D = NULL;
	ModuleCamera3D* camera = NULL;
	ModulePhysics3D* physics = NULL;
	ModuleEditor* editor = NULL;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void Log(const char* log);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;