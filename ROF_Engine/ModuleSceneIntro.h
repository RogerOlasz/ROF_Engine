#pragma once
#include "Module.h"
#include <array>
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 30

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

};
