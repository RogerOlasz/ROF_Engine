#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentCamera;

class ModuleCamera3D : public Module
{
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
	vec reference;
	
};

#endif // !__MODULECAMERA3D_H__