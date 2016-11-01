#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentCamera;

class ModuleCamera3D : public Module
{
friend class ComponentCamera;

public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	ComponentCamera* GetCamera() const;

	void LookAt(const vec &position);
	void Move(float dt);
	void ReferenceOrbit();

private:
	ComponentCamera* camera = nullptr;
	//Maybe could have reference on Comp Camera
	vec reference;

public:
	bool controls_disabled = false;
	bool invert_controls_x = true;
	bool invert_controls_y = true;
	
};

#endif // !__MODULECAMERA3D_H__