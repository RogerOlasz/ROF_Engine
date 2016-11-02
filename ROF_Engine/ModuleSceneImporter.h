#ifndef __MODULESCENEIMPORTER_H__
#define __MODULESCENEIMPORTER_H__

#include "Module.h"

class ModuleSceneImporter : public Module
{
public:

	ModuleSceneImporter(Application* app, bool start_enabled = true);
	~ModuleSceneImporter();

	bool Init();
	bool Update();
	bool CleanUp();

};

#endif // !__MODULESCENEIMPORTER_H__