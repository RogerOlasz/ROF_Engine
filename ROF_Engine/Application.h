#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include <string>
#include "Globals.h"
#include "Timer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleScene;
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
	ModuleScene* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleGeometry* geometry = nullptr;
	ModuleFileSystem* physfs = nullptr;
	ModuleGOManager* go_manager = nullptr;
	ModuleSceneImporter* importer = nullptr;

	Timer time_since_startup;

private:

	Timer to_fps;
	float fps;
	float fps_last_sec;
	Timer ms_timer;
	float dt;
	float max_fps;
	float capped_ms;

	char app_name[SHORT_STRING];
	char organization[SHORT_STRING];

	bool want_to_save_scene;
	bool want_to_load_scene;

	std::string load_scene;
	std::string save_scene;

	std::string load_editor;
	std::string save_editor;

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

	void LoadEditorConfig(const char* file);
	void SaveEditorConfig(const char* file);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	bool LoadSceneNow();
	bool SaveSceneNow();

	bool LoadConfigNow();
	bool SaveConfigNow();

};

extern Application* App;

#endif // !__APPLICATION_H__