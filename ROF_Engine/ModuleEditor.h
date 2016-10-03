#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"

class PanelConsole;
class PanelConfiguration;

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	bool CleanUp();
	update_status Update(float dt);

	void Log(const char* log);
	void LogFPS(const float* fps, const float ms);
	uint MaxFPS();

private:
	PanelConsole* Console = nullptr;
	PanelConfiguration* Config = nullptr;

	bool console = false;
	bool config = false;
};

#endif // __MODULEEDITOR_H__
