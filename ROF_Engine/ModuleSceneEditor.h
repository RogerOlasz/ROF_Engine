#ifndef __MODULESCENEEDITOR_H__
#define __MODULESCENEEDITOR_H__

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Primitive;

class ModuleSceneEditor : public Module
{
public:
	ModuleSceneEditor(Application* app, bool start_enabled = true);
	~ModuleSceneEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	LineSegment picking;
	Plane_P grid;

};

#endif // !__MODULESCENEEDITOR_H__