#ifndef __Application_H__
#define __Application_H__

#include <list>
#include "Globals.h"
#include "Timer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModulePhysics3D;
class ModuleEditor;
class ModuleGeometry;
class ModuleFileSystem;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModulePhysics3D* physics = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleGeometry* geometry = nullptr;
	ModuleFileSystem* physfs = nullptr;

private:

	Timer to_fps;
	float fps;
	float fps_last_sec;
	Timer	ms_timer;
	float	dt;
	float max_fps;
	float capped_ms;

	char app_name[SHORT_STRING];
	char organization[SHORT_STRING];

	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	const char *GetAppName();
	const char *GetOrganization();

	void Log(const char* log);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif // __Application_H__