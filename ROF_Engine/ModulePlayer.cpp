#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	

	VehicleInfo car;
	Available_Controls = false;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5f, 2.5f, 6);
	car.chassis_offset.Set(0, 2, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 4.00f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 800.0f;
	car.frictionSlip = 20.5f;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.1f;
	float wheel_radius = 1.5f;
	float wheel_width = 1.1f;
	float suspensionRestLength = 0.6f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*1.2f;
	float half_length = car.chassis_size.z*0.8f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(88, 4, -128);
	vehicle->Orient(-(3.1415/2));

	
	//INITIALIZATION OF VARIABLES---------------------------------
	jump_counter = 4;
	follow_car = false;
	rev_sound = App->audio->LoadFx("Rev_sound.ogg");
	time = new Timer;
	time_lap = new Timer;
    state_of_game = "NULL";
	vehicle->Brake(2000.f);
	lose_sound = App->audio->LoadFx("You_Lose_Sound_Effect.wav");


	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	//CAMERA FOLLOW CAR------------------------------------------------------------------------------------
	if (follow_car == true)
	{
		float speed_cam = 0.05f;
		vec3 v_pos = vehicle->GetPos();
		vec3 v_forward = vehicle->GetForwardVector();

		vec3 car_camera_pos = { -8.0f, 6.0f, -20.0f };
		vec3 camera_new_position = { v_pos.x + (v_forward.x * car_camera_pos.x), v_pos.y + v_forward.y + car_camera_pos.y, v_pos.z + (v_forward.z * car_camera_pos.z) };
		vec3 camera_speed = camera_new_position - App->camera->Position;

		App->camera->Look(App->camera->Position + (speed_cam * camera_speed), v_pos);
	}
	
    //CONTROLS-------------------------------------------------------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && vehicle->GetKmh() < 150.f && Available_Controls == true)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && Available_Controls == true)
	{
		follow_car = !follow_car;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && Available_Controls == true)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && Available_Controls == true)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && Available_Controls == true)
	{
		brake = BRAKE_POWER;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && Available_Controls == true)
	{
		acceleration = MAX_B_ACCELERATION;
	}
    //JUMP-------------------------
	if (App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_DOWN && jump_counter > 0 && Available_Controls == true)
	{
		vehicle->Push(0, 3000, 0);
		jump_counter--;
	}

	//ChecKPoints-------------------------------------------------------------------------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		vehicle->Stop_Motion();
		vehicle->SetPos(App->scene_intro->pos_check_point.x, App->scene_intro->pos_check_point.y, App->scene_intro->pos_check_point.z);

		if (App->scene_intro->pos_check_point.x == -80 && App->scene_intro->pos_check_point.y == 4 && App->scene_intro->pos_check_point.z == 95)
		{
			vehicle->Orient((3.1415 / 2));
		}

		if (App->scene_intro->pos_check_point.x == 31 && App->scene_intro->pos_check_point.y == 4 && App->scene_intro->pos_check_point.z == -55)
		{
			vehicle->Orient(-(180*3.1415 /180 ));
		}

		
		if (App->scene_intro->pos_check_point.x == -135 && App->scene_intro->pos_check_point.y == 5 && App->scene_intro->pos_check_point.z == 159)
		{
			vehicle->Orient((3.1415 / 2));
		}
		
		if (App->scene_intro->pos_check_point.x == 88 && App->scene_intro->pos_check_point.y == 4 && App->scene_intro->pos_check_point.z == -128)
		{
           vehicle->Orient(-(3.1415 / 2));
		}

	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	//Timer Game-------------------------------------------------------------------
	vehicle->Render();

	static int min;
	static int sec;
	static int ms;

	static int total_secs;

	ms = time->Read();
	sec = ms / 1000;

	total_secs = time_lap->Read() / 1000;

	if (sec >= 60)
	{
		sec = 0;
		min++;
		time->Stop();
		time->Start();
	}

	//TRAFFIC LIGHT-----------------------------------------------
	if (sec == 3 || sec == 4 || sec == 5 || sec == 6 || sec == 7)
	{	
		if (sec == 3)
		{
			App->scene_intro->Traffic_Light[0].color = Green;
		}

		if (sec == 4)
		{
			App->scene_intro->Traffic_Light[1].color = Green;
		}
		if (sec == 5)
		{
			App->scene_intro->Traffic_Light[2].color = Green;
		}
		if (sec == 6)
		{
			App->scene_intro->Traffic_Light[3].color = Green;
           follow_car = true;
		   Available_Controls = true;
		   state_of_game = "NULL";
		}
	}
	else
	{
		App->scene_intro->Traffic_Light[0].color = Red;
		App->scene_intro->Traffic_Light[1].color = Red;
		App->scene_intro->Traffic_Light[2].color = Red;
		App->scene_intro->Traffic_Light[3].color = Red;
	}

	
	//WIN and LOSE Respawns--------------------------------------------
	if (min == 2 && sec == 1 || App->scene_intro->sensor_end == true)
	{
		Available_Controls = false;
		Reset_Vehicle(88, 4, -128, -(3.1415 / 2));
		if (App->scene_intro->sensor_end == false)
		{
			App->scene_intro->pos_check_point.x = 88;
			App->scene_intro->pos_check_point.y = 4;
			App->scene_intro->pos_check_point.z = -128;
			state_of_game = "----YOU LOSE-----";
			App->audio->PlayFx(lose_sound, 0);
		}
		if (min < 2)
		{
			state_of_game = "----YOU WIN-----";
		}
		
	}
	
	if (min == 2 && sec == 2 || App->scene_intro->sensor_end == true)
	{
		if (App->scene_intro->sensor_end == true)
		{
			App->camera->Move(vec3(-16, 3, -2));
		}
		else
		{
           App->camera->Move(vec3(0.7, 0.03, 0));
		}	
	}

	if (min == 2 && sec == 3 || App->scene_intro->sensor_end == true)
	{

		App->scene_intro->s[7].color = Blue;
		App->scene_intro->s[8].color = Blue;
		App->scene_intro->s[9].color = Blue;

		min = 0;
		sec = 0;
        time->Stop();
		time->Start();		
		follow_car = false;
		App->scene_intro->entry = false;
		App->scene_intro->sensor_end = false;
	}


    char title[80];
		
	sprintf_s(title, "%.1f Km/h Jumps left: %d Timer: %d min %d s   State of Player: %s", vehicle->GetKmh(), jump_counter, min, sec, state_of_game);

	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::Set_pos(float x, float y, float z)
{
	vehicle->SetPos(x, y, z);
}

void ModulePlayer::Reset_Vehicle(float x, float y, float z, float orient)
{
	vehicle->SetPos(x, y, z);
	vehicle->Stop_Motion();
	vehicle->Orient(orient);

}