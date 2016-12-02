#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>

class Panel;
class PanelConsole;
class PanelConfiguration;
class PanelHierarchy;
class PanelComponents;
class PanelTimeControl;
class GameObject;

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

	void SetSelectedGO(GameObject* go);

	//File explorer (all related to file explorer is done by Ric)
	bool FileDialog(const char* extension = nullptr, const char* from_folder = nullptr);
	const char* CloseFileDialog();

private:
	//File explorer
	void LoadFile(const char* filter_extension = nullptr, const char* from_dir = nullptr);
	void DrawDirectoryRecursive(const char* directory, const char* filter_extension);

private:
	PanelConsole* Console = nullptr;
	PanelConfiguration* Config = nullptr;
	PanelHierarchy* Hierarchy = nullptr;
	PanelComponents* Comp = nullptr;
	PanelTimeControl* TimeControl = nullptr;

	std::vector<Panel*> panels;

	uint camera_id;
	std::string camera_name;

	bool aabb_debug = false;
	bool octtree_debug = false;

	bool loading_scene = false;
	bool saving_scene = false;

	//File explorer
	enum file_dialog
	{
		closed,
		opened,
		ready_to_close
	} file_dialog = closed;

	std::string file_dialog_filter;
	std::string file_dialog_origin;

	bool in_modal = false;
	char selected_file[MEDIUM_STRING];
	bool file_explorer = false;
};

#endif // __MODULEEDITOR_H__
