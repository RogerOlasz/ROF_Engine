#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

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

private:
	bool config_on = false;

	PanelConsole* Console = NULL;
	PanelConfiguration* Config = NULL;
};

#endif // __ModuleEditor_H__
