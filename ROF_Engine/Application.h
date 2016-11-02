#ifndef __Application_H__
#define __Application_H__

#include <list>
#include <string>
#include "Globals.h"
#include "Timer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class ModuleGeometry;
class ModuleFileSystem;
class ModuleGOManager;
class ModuleSceneImporter;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleGeometry* geometry = nullptr;
	ModuleFileSystem* physfs = nullptr;
	ModuleGOManager* go_manager = nullptr;
	ModuleSceneImporter* importer = nullptr;

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

	bool want_to_save;
	bool want_to_load;

	std::string load_scene;
	std::string save_scene;

	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	const char *GetAppName();
	const char *GetOrganization();

	bool RequestBrowser(const char* link);
	void Log(const char* log);

	void LoadScene(const char* file);
	void SaveScene(const char* file);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	bool LoadSceneNow();
	bool SaveSceneNow();	

};

extern Application* App;

#endif // __Application_H__