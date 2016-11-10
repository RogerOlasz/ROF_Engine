#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include <vector>
#include <string>

class Panel;
class PanelConsole;
class PanelConfiguration;
class PanelHierarchy;
class PanelComponents;
class PanelTimeControl;

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

	bool Load(pugi::xml_node &config);
	bool Save(pugi::xml_node &config) const;

	void Log(const char* log);
	void LogFPS(const float* fps, const float ms);
	void SetMaxFPS(uint new_fps_cap);
	uint GetMaxFPS() const;

public:
	PanelConsole* Console = nullptr;
	PanelConfiguration* Config = nullptr;
	PanelHierarchy* Hierarchy = nullptr;
	PanelComponents* Comp = nullptr;
	PanelTimeControl* TimeControl = nullptr;

private:
	std::vector<Panel*> panels;

	uint camera_id;
	std::string camera_name;

	bool aabb_debug = false;
	bool octtree_debug = false;

};

#endif // __MODULEEDITOR_H__
