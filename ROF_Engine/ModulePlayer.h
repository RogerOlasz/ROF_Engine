#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;


#define MAX_ACCELERATION 3000.0f
#define MAX_B_ACCELERATION -4000.0f
#define TURN_DEGREES 30.0f * DEGTORAD
#define BRAKE_POWER 2000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void Set_pos(float x, float y, float z);
	void Reset_Vehicle(float x, float y, float z, float orient);


public:

	Timer* time;
	Timer* time_lap;
	uint jump_counter;
	bool follow_car;
	bool Available_Controls;
	int rev_sound;
	int lose_sound;
	char* state_of_game;

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
};