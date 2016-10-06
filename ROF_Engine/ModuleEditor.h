#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include <vector>

class Panel;
class PanelConsole;
class PanelConfiguration;

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	bool CleanUp();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void Log(const char* log);
	void LogFPS(const float* fps, const float ms);
	uint GetMaxFPS();

public:
	PanelConsole* Console = nullptr;
	PanelConfiguration* Config = nullptr;

private:
	std::vector<Panel*> panels;

};

#endif // __MODULEEDITOR_H__
