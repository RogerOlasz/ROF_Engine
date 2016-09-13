#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");

	//INITIALIZATION OF VARIABLES---------------------------------
	bool ret = true;
	entry = false;
	sensor_end = false;
	pos_check_point.x = 88;
	pos_check_point.y = 4;
	pos_check_point.z = -128;

	App->camera->Move(vec3(110, 8, -133));
	App->camera->LookAt(vec3(8, 4, -128));

	//Load and play music--------------------------------------------------------------------------------------------
	App->audio->PlayMusic("Background_Music.ogg");
	sound = App->audio->LoadFx("Truck_IDEL.ogg");
	App->audio->PlayFx(sound, -1);


	//Inivisible Walss-------------------------------------------------------------------------------------------------------
	Create_Bodys_World(380, 80, 3, 0, 30, -145, Invisible_Walls, PB_Invisible_Walls, 0, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(380, 80, 3, 0, 30, 200, Invisible_Walls, PB_Invisible_Walls, 1, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(30, 80, 115, 191, 30, -89, Invisible_Walls, PB_Invisible_Walls, 2, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(5, 80, 60, 205, 30, -5, Invisible_Walls, PB_Invisible_Walls, 3, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(30, 80, 115, 191, 30, 73, Invisible_Walls, PB_Invisible_Walls, 4, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(115, 80, 90, 140, 30, 160, Invisible_Walls, PB_Invisible_Walls, 5, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(110, 80, 30, -150, 30, 190, Invisible_Walls, PB_Invisible_Walls, 6, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(3, 80, 360, -176, 30, 20, Invisible_Walls, PB_Invisible_Walls, 7, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(30, 80, 110, 1, 30, 0, Invisible_Walls, PB_Invisible_Walls, 8, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(60, 80, 189, 74, 30, -20, Invisible_Walls, PB_Invisible_Walls, 9, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(40, 80, 59, 115, 30, 45, Invisible_Walls, PB_Invisible_Walls, 10, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(40, 80, 85, 115, 30, -73, Invisible_Walls, PB_Invisible_Walls, 11, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(100, 80, 20, -5, 30, -105, Invisible_Walls, PB_Invisible_Walls, 12, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(85, 80, 25, -98, 30, 12.5, Invisible_Walls, PB_Invisible_Walls, 13, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(42, 80, 28, -120, 30, 129.5, Invisible_Walls, PB_Invisible_Walls, 14, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(16, 80, 95, -133, 30, 70, Invisible_Walls, PB_Invisible_Walls, 15, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(117, 80, 5, -40, 30, 118, Invisible_Walls, PB_Invisible_Walls, 16, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(39, 80, 5, 63, 30, 118, Invisible_Walls, PB_Invisible_Walls, 17, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(112, 80, 20, -39, 30, 65, Invisible_Walls, PB_Invisible_Walls, 18, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(62, 80, 55, -87, 30, -84, Invisible_Walls, PB_Invisible_Walls, 19, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(10, 80, 65, -61, 30, -24, Invisible_Walls, PB_Invisible_Walls, 20, 0, false, false, 0, 0, 0, 0, false, 0);

	//Sensors-----------------------------------------------------------------------------------------------------------------
	Create_Bodys_World(185, 2, 80, -5, 1.0f, 160, s, sensor, 0, 0, true, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(100, 2, 48, 150, 1.0f, -7.5, s, sensor, 1, 0, true, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(145, 1.5, 35, 30, 0.0f, 95, s, sensor, 2, 0, true, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(20, 1, 38, 1, 0.5, -75, s, sensor, 3, 0, true, false, 0, 0, 0, 0,false,0);
	Create_Bodys_World(40, 1, 27, -34, 0.5, -25, s, sensor, 4, 0, true, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(40, 1, 27, -34, 0.5, 1, s, sensor, 5, 0, true, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(1, 15, 20, 100, 4, -128, s, sensor, 6, 0, true, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(3, 3.5, 25, -135, 0, 159, s, sensor, 7, 0, true, false, 0, 0, 0, 0, false, 0);
    s[7].color = Blue;
	Create_Bodys_World(25, 3.5, 3, 31, 0, -55, s, sensor, 8, 0, true, false, 0, 0, 0, 0, false, 0);
	s[8].color = Blue;
	Create_Bodys_World(3, 3.5, 25, -80, 0, 95, s, sensor, 9, 0, true, false, 0, 0, 0, 0, false, 0);
	s[9].color = Blue;
	Create_Bodys_World(1, 15, 20, 120, 4, -128, s, sensor, 10, 0, true, false, 0, 0, 0, 0, false, 0);


	//Creation of Ciurcuit Start-------------------------------------------------------------------------------------------------
	Create_Circuit_World(2, 2, 2, 80, 1, -120, Circuit_Start, PB_Circuit_Start, 0, 4, 1, 0, false, true, false, false, 10);
	Create_Circuit_World(2, 2, 2, 80, 1, -135, Circuit_Start, PB_Circuit_Start, 4, 8, 1, 0, false, true, false, false, 10);
	Create_Circuit_World(2, 2, 2, -45, 1, 110, Circuit_Start, PB_Circuit_Start, 8, 12, 1, 0, false, true, false, false, 10);
	Create_Circuit_World(2, 2, 2, -45, 1, 80, Circuit_Start, PB_Circuit_Start, 12, 16, 1, 0, false, true, false, false, 10);
	Create_Circuit_World(2, 2, 2, 105, 1, 110, Circuit_Start, PB_Circuit_Start, 16, 20, 1, 0, false, true, false, false, 10);
	Create_Circuit_World(2, 2, 2, 105, 1, 80, Circuit_Start, PB_Circuit_Start, 20, 24, 1, 0, false, true, false, false, 10);

	
	//Creation of Ciurcuit Trace---------------------------------------------------------------------------------------------------

	Create_Circuit_World(2, 2, 2, -110, 1, -115, Circuit_Trace, PB_Circuit_Trace, 0, 20, -110, 0, true, false, true, true, 10);
	Create_Circuit_World(2, 2, 2, -110, 1, -140, Circuit_Trace, PB_Circuit_Trace, 20, 40, -110, 0, true, false, true, true, 10);
	Create_Circuit_World(2, 2, 2, -141, 1, 14, Circuit_Trace, PB_Circuit_Trace, 40, 54, 14, 0, true, false, false, true, 10);
	Create_Circuit_World(2, 2, 2, -174, 1, 14, Circuit_Trace, PB_Circuit_Trace, 54, 71, 14, 0, true, false, false, true, 10);
	Create_Circuit_World(2, 2, 2, -130, 1, 144, Circuit_Trace, PB_Circuit_Trace, 71, 75, -130, 0, true, false, true, true, 10);
	Create_Circuit_World(2, 2, 2, -164, 1, 174, Circuit_Trace, PB_Circuit_Trace, 75, 82, -164, 0, true, false, true, true, 10);
	Create_Circuit_World(2, 2, 2, 17, 1, 65, Circuit_Trace, PB_Circuit_Trace, 82, 95, 65, 0, true, false, false, false, 10);
	Create_Circuit_World(2, 2, 2, 45, 1, 65, Circuit_Trace, PB_Circuit_Trace, 95, 112, 65, 0, true, false, false, false, 10);
	Create_Circuit_World(2, 2, 2, 35, 1, -95, Circuit_Trace, PB_Circuit_Trace, 112, 121, 35, 0, true, false, true, false, 10);
	Create_Circuit_World(2, 2, 2, 5, 1, -55, Circuit_Trace, PB_Circuit_Trace, 121, 124, 5, 0, true, false, true, false, 10);
	Create_Circuit_World(2, 2, 2, -15, 1, -45, Circuit_Trace, PB_Circuit_Trace, 124, 135, -45, 0, true, false, false, true, 10);
	Create_Circuit_World(2, 2, 2, -55, 1, -95, Circuit_Trace, PB_Circuit_Trace, 135, 147, -95, 0, true, false, false, true, 10);
	Create_Circuit_World(2, 2, 2, -55, 1, 25, Circuit_Trace, PB_Circuit_Trace, 147, 155, -55, 0, true, false, true, false, 10);
	Create_Circuit_World(2, 2, 2, -25, 1, 55, Circuit_Trace, PB_Circuit_Trace, 155, 162, -25, 0, true, false, true, false, 10);
	Create_Circuit_World(2, 2, 2, -95, 1, 55, Circuit_Trace, PB_Circuit_Trace, 162, 165, 55, 0, true, false, false, true, 10);
	Create_Circuit_World(2, 2, 2, -85, 1, 75, Circuit_Trace, PB_Circuit_Trace, 165, 172, -85, 0, true, false, true, true, 10);
	Create_Circuit_World(2, 2, 2, -125, 1, 25, Circuit_Trace, PB_Circuit_Trace, 172, 181, 25, 0, true, false, false, true, 10);
	Create_Circuit_World(2, 2, 2, -125, 1, 115, Circuit_Trace, PB_Circuit_Trace, 181, 192, -125, 0, true, false, true, true, 10);
	Create_Circuit_World(2, 2, 2, 85, 1, 115, Circuit_Trace, PB_Circuit_Trace, 192, 201, 85, 0, true, false, true, true, 10);
	Create_Circuit_World(2, 2, 2, 175, 1, 115, Circuit_Trace, PB_Circuit_Trace, 201, 212, 115, 0, true, false, false, false, 10);
	Create_Circuit_World(2, 2, 2, 85, 1, 75, Circuit_Trace, PB_Circuit_Trace, 212, 217, 85, 0, true, false, true, true, 10);
	Create_Circuit_World(2, 2, 2, 135, 1, 75, Circuit_Trace, PB_Circuit_Trace, 217, 224, 75, 0, true, false, false, false, 10);
	Create_Circuit_World(2, 2, 2, 135, 1, -35, Circuit_Trace, PB_Circuit_Trace, 224, 233, -35, 0, true, false, false, false, 10);
	Create_Circuit_World(2, 2, 2, 175, 1, -35, Circuit_Trace, PB_Circuit_Trace, 233, 244, -35, 0, true, false, false, false, 10);
	Create_Circuit_World(2, 2, 2, 175, 1, -140, Circuit_Trace, PB_Circuit_Trace, 244, 254, 175, 0, true, false, true, false, 10);
	Create_Circuit_World(2, 2, 2, 125, 1, -115, Circuit_Trace, PB_Circuit_Trace, 254, 259, 125, 0, true, false, true, false, 10);

	//Creation of Ciurcuit Curves---------------------------------------------------------------------------------------------------

	//Left Curves------------------------------------------------------
	Create_Curves(2, 2, 2, -120, 1, -114, Circuit_Curve, PB_Circuit_Curve, 0, 5, 0, 1, 2, 3, 4, 2, 4, 6, 8, 10, 0, -8, -6, -5, -2.5);
	Create_Curves(2, 2, 2, -141.5, 1, -75, Circuit_Curve, PB_Circuit_Curve, 5, 10, 5, 6, 7, 8, 9, 2, 7, 6, 3, 3, 2.5, 5, 6, 7, 9);
	Create_Curves(2, 2, 2, -101, 1, -54, Circuit_Curve, PB_Circuit_Curve, 10, 16, 11, 12, 13, 14, 15, 1, 4, 6, 7, 8, 10, 9, 7, 5, 2);
	Create_Curves(2, 2, 2, -68, 1, -28, Circuit_Curve, PB_Circuit_Curve, 16, 21, 16, 17, 18, 19, 20, 8, 7, 6, 4, 1, -2, -5, -7, -9, -10);
	Create_Curves(2, 2, 2, -101, 1, -2, Circuit_Curve, PB_Circuit_Curve, 21, 26, 21, 22, 23, 24, 25, 0, 0, 0, 0, 6, -10, -10, -10, -10, 0);



	//Left Curves------------------------------------------------------
	Create_Curves(2, 2, 2, -116, 1, -138, Circuit_Curve, PB_Circuit_Curve, 26, 31, 26, 27, 28, 29, 30, -2, 3, 4, 5, 7, -4, -10, -10, -8, -8);
	Create_Curves(2, 2, 2, -141, 1, -120, Circuit_Curve, PB_Circuit_Curve, 31, 36, 31, 32, 33, 34, 35, 7, 10, 10, 10, 10, -20, -5, -3, 0, 1);
	Create_Curves(2, 2, 2, -168, 1, -73, Circuit_Curve, PB_Circuit_Curve, 36, 41, 36, 37, 38, 39, 40, 10, 10, 8, 5, 5, 3, 4, 6, 7, 7);
	Create_Curves(2, 2, 2, -141, 1, -35, Circuit_Curve, PB_Circuit_Curve, 41, 46, 41, 42, 43, 44, 45, 3, 2, 2, 0, 2, 8, 10, 10, 10, -20);
	Create_Curves(2, 2, 2, -123, 1, -26, Circuit_Curve, PB_Circuit_Curve, 46, 51, 46, 47, 48, 49, 50, 0, 0, 0, 0, 0, -10, -10, -10, -10, -10);
	Create_Curves(2, 2, 2, -173, 1, -26, Circuit_Curve, PB_Circuit_Curve, 51, 54, 51, 52, 53, 54, 55, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0);


	//Creation of Ciurcuit Building---------------------------------------------------------------------------------------------------

	Create_Bodys_World(15, 8.5, 20, -90, 4.25, 160, Circuit_Building, PB_Circuit_Building, 0, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(35, 1, 20, -67, 13.5, 160, Circuit_Building, PB_Circuit_Building, 1, 0, false, true, 0, 0, 1, 19, false, 0);
	Create_Bodys_World(20, 19.8, 30, -40.6, 9.75, 160, Circuit_Building, PB_Circuit_Building, 2, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(25, 19.8, 30, 31, 9.75, 160, Circuit_Building, PB_Circuit_Building, 3, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(22.5, 1, 82, 31, 9.02, 106, Circuit_Building, PB_Circuit_Building, 4, 0, false, true, 1, 0, 0, -14.3, false, 0);
	Create_Bodys_World(1, 14, 82, 42, 15, 104, Circuit_Building, PB_Circuit_Building, 5, 0, false, true, 1, 0, 0, -14.3, false, 0);
	Create_Bodys_World(1, 14, 82, 20, 15, 104, Circuit_Building, PB_Circuit_Building, 6, 0, false, true, 1, 0, 0, -14.3, false, 0);
	Create_Bodys_World(50, 1, 26, -25, 8, 95, Circuit_Building, PB_Circuit_Building, 7, 0, false, true, 0, 0, 1, 20, false, 0);
	Create_Bodys_World(50, 1, 26, 85, 8, 95, Circuit_Building, PB_Circuit_Building, 8, 0, false, true, 0, 0, 1, -20, false, 0);
	Create_Bodys_World(35, 1, 20, 155, 4, 15, Circuit_Building, PB_Circuit_Building, 9, 0, false, true, 1, 0, 0, 29, false, 0);
	Create_Bodys_World(35, 10.5, 20, 155, 4, -3.5, Circuit_Building, PB_Circuit_Building, 10, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(35, 15, 18, 155, 6.2, -22.5, Circuit_Building, PB_Circuit_Building, 11, 0, false, false, 0, 0, 0, 0, false, 0);

	

	//Creation of Ciurcuit Bridge-----------------------------------------------------------------------------------------------------

	Create_Bodys_World(8, 0.2, 10, -24, 21, 160, Bridge, PB_Bridge, 0, 20, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(8, 0.2, 10, -11, 21, 160, Bridge, PB_Bridge, 1, 20, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(8, 0.2, 10, 2, 21, 160, Bridge, PB_Bridge, 2, 20, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(8, 0.2, 10, 21, 160, 160, Bridge, PB_Bridge, 3, 20, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(8, 0.2, 10, 28, 21, 160, Bridge, PB_Bridge, 4, 20, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(8, 0.2, 10, 41, 21, 160, Bridge, PB_Bridge, 5, 20, false, false, 0, 0, 0, 0, false, 0);

	//Creation of Circuit Obstacles---------------------------------------------------------------------------------------------------
	
	Create_Bodys_World(6, 6, 11, 2, 3, -122, Circuit_Obstacles, PB_Circuit_Obstacles, 0, 0, false, false, 0, 0, 0, 0, false,0);
	Create_Bodys_World(6, 6, 11, -23, 3, -133, Circuit_Obstacles, PB_Circuit_Obstacles, 1, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(6, 6, 11, 27, 3, -133, Circuit_Obstacles, PB_Circuit_Obstacles, 2, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(6, 8, 5.65, -55, 4, -127.5, Circuit_Obstacles, PB_Circuit_Obstacles, 3, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(6, 1, 22, -55, 8.55, -127.5, Circuit_Obstacles, PB_Circuit_Obstacles, 4, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(6, 1, 9.1, -55, 4.5, -116, Circuit_Obstacles, PB_Circuit_Obstacles, 5, 0, false, true, 1, 0, 0, 90, false, 0);
	Create_Bodys_World(6, 1, 9.1, -55, 4.5, -139, Circuit_Obstacles, PB_Circuit_Obstacles, 6, 0, false, true, 1, 0, 0, 90, false, 0);
	Create_Bodys_World(8, 1, 7.5, -58, 4, -134.5, Circuit_Obstacles, PB_Circuit_Obstacles, 7, 0, false, true, 0, 0, 1, 90, false, 0);
	Create_Bodys_World(6, 6, 11, -85, 3, -122, Circuit_Obstacles, PB_Circuit_Obstacles, 8, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(31, 1, 11, -157.5, 2, 20, Circuit_Obstacles, PB_Circuit_Obstacles, 9, 0, false, true, 1, 0, 0, -28, false, 0);
	Create_Bodys_World(31, 1, 11, -157.5, 2, 35, Circuit_Obstacles, PB_Circuit_Obstacles, 10, 0, false, true, 1, 0, 0, 28, false, 0);
	Create_Bodys_World(31, 1, 11, -157.5, 2, 85, Circuit_Obstacles, PB_Circuit_Obstacles, 11, 0, false, true, 1, 0, 0, -28, false, 0);
	Create_Bodys_World(31, 1, 11, -157.5, 2, 100, Circuit_Obstacles, PB_Circuit_Obstacles, 12, 0, false, true, 1, 0, 0, 28, false, 0);
	Create_Bodys_World(11, 1, 28, -115, 2, 159, Circuit_Obstacles, PB_Circuit_Obstacles, 13, 0, false, true, 0, 0, 1, 28, false, 0);
	Create_Bodys_World(15, 5, 2, -158, 2, 27.5, Circuit_Obstacles, PB_Circuit_Obstacles, 14, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(15, 5, 2, -158, 2, 92.5, Circuit_Obstacles, PB_Circuit_Obstacles, 15, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(18, 12, 2, -47.9, 5, 16.5, Circuit_Obstacles, PB_Circuit_Obstacles, 16, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(18, 12, 2, -20.1, 5, 16.5, Circuit_Obstacles, PB_Circuit_Obstacles, 17, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(18, 12, 2, -47.9, 5, -40.5, Circuit_Obstacles, PB_Circuit_Obstacles, 18, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(18, 12, 2, -20.1, 5, -40.5, Circuit_Obstacles, PB_Circuit_Obstacles, 19, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(2, 12, 15, -9, 5, -62.5, Circuit_Obstacles, PB_Circuit_Obstacles, 20, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(2, 12, 15, -9, 5, -87.5, Circuit_Obstacles, PB_Circuit_Obstacles, 21, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(2, 12, 15, 10, 5, -62.5, Circuit_Obstacles, PB_Circuit_Obstacles, 22, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(2, 12, 15, 10, 5, -87.5, Circuit_Obstacles, PB_Circuit_Obstacles, 23, 0, false, false, 0, 0, 0, 0, false, 0);
	
	//Creation of Bumps----------------------------------------------------------------------------------------------------------------

    Create_Cylinder_Bodys_World(2, 20, 31, -0.5, 55, Bump, PB_Bump, 0, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, 45, Bump, PB_Bump, 1, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, 35, Bump, PB_Bump, 2, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, 25, Bump, PB_Bump, 3, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, 15, Bump, PB_Bump, 4, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, 5, Bump, PB_Bump, 5, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, -5, Bump, PB_Bump, 6, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, -15, Bump, PB_Bump, 7, 0);
    Create_Cylinder_Bodys_World(2, 20, 31, -0.5, -25, Bump, PB_Bump, 8, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, -35, Bump, PB_Bump, 9, 0);
	Create_Cylinder_Bodys_World(2, 20, 31, -0.5, -45, Bump, PB_Bump, 10, 0);
	Create_Cylinder_Bodys_World(2, 30, 155, -0.5, 30, Bump, PB_Bump, 11, 0);
	Create_Cylinder_Bodys_World(2, 30, 155, -0.5, 40, Bump, PB_Bump, 12, 0);
	Create_Cylinder_Bodys_World(2, 30, 155, -0.5, 50, Bump, PB_Bump, 13, 0);
	Create_Cylinder_Bodys_World(2, 30, 155, -0.5, 60, Bump, PB_Bump, 14, 0);
	Create_Cylinder_Bodys_World(2, 30, 155, -0.5, 70, Bump, PB_Bump, 15, 0);

	
	//Pendulo------------------------------------------------
	//Cubes
	Create_Bodys_World(2, 2, 2, 5.0f, 45.0f, 160, Pendulum, PB_Pendulum, 0, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(2, 2, 2, -6.0f, 40.0f, 160, Pendulum, PB_Pendulum, 1, 0, false, false, 0, 0, 0, 0, true, 1);
	Create_Bodys_World(2, 2, 2, -16.0f, 45.0f, 160, Pendulum, PB_Pendulum, 2, 0, false, false, 0, 0, 0, 0, true, 2);
	Create_Bodys_World(2, 2, 2, 155, 35, -3.5, Pendulum, PB_Pendulum, 3, 0, false, false, 0, 0, 0, 0, true, 3);
	Create_Bodys_World(2, 2, 2, 155, 35, -22.5, Pendulum, PB_Pendulum, 4, 0, false, false, 0, 0, 0, 0, true, 4);
	Create_Bodys_World(2, 2, 2, -33, 25, -12.5, Pendulum, PB_Pendulum, 5, 0, false, false, 0, 0, 0, 0, true, 5);

	//Spheres
	Create_Sphere_Bodys_World(2.5, 5.0f, 15.0f, 173, Pendulum_1, PB_Pendulum, 0, 3000, true, 1);
	Create_Sphere_Bodys_World(2.5, -6.0f, 15.0f, 149, Pendulum_1, PB_Pendulum, 1, 3000, true, 2);
	Create_Sphere_Bodys_World(2.5, -16.0f, 15.0f, 173, Pendulum_1, PB_Pendulum, 2, 3000, true, 3);
	Create_Sphere_Bodys_World(4, 173, 15, -3.5, Pendulum_1, PB_Pendulum, 3, 3000, true, 4);
	Create_Sphere_Bodys_World(4, 140, 15, -22.5, Pendulum_1, PB_Pendulum, 4, 3000, true, 5);
	Create_Sphere_Bodys_World(3.5, -45, 40, -12.5, Pendulum_1, PB_Pendulum, 5, 3000, true, 6);

	//Traffic Lights
	Create_Sphere_Bodys_World(0.5, 80, 10, -126.5, Traffic_Light, PB_Traffic_Light, 0, 0, false, 0);
	Traffic_Light[0].color = Red;
	Create_Sphere_Bodys_World(0.5, 80, 10, -127.5, Traffic_Light, PB_Traffic_Light, 1, 0, false, 0);
	Traffic_Light[1].color = Red;
	Create_Sphere_Bodys_World(0.5, 80, 10, -128.5, Traffic_Light, PB_Traffic_Light, 2, 0, false, 0);
	Traffic_Light[2].color = Red;
	Create_Sphere_Bodys_World(0.5, 80, 10, -129.5, Traffic_Light, PB_Traffic_Light, 3, 0, false, 0);
	Traffic_Light[3].color = Red;

	//Constrains Pendulum
	App->physics->AddConstraintP2P(*PB_Pendulum[0], *PB_Pendulum[1], vec3(0, -2, 0), vec3(0, 21.5, 0));//Right
	App->physics->AddConstraintP2P(*PB_Pendulum[2], *PB_Pendulum[3], vec3(0, -2, 0), vec3(0, 17, 0));//Right
	App->physics->AddConstraintP2P(*PB_Pendulum[4], *PB_Pendulum[5], vec3(0, -2, 0), vec3(0, 21.5, 0));//Right
	App->physics->AddConstraintP2P(*PB_Pendulum[6], *PB_Pendulum[7], vec3(0, -2, 0), vec3(0, 17, 0));//Right
	App->physics->AddConstraintP2P(*PB_Pendulum[8], *PB_Pendulum[9], vec3(0, -2, 0), vec3(0, 14.5, 0));//Right
	App->physics->AddConstraintP2P(*PB_Pendulum[10], *PB_Pendulum[11], vec3(0, -2, 0), vec3(0, 14.5, 0));//Right


	//Trampoline---------------------------------------------------------------------------------------------------------
	
	Create_Bodys_World(20, 1, 4, 7, 4.8, -72, Trampoline, PB_Trampoline, 0, 0, false, true, 0, 0, 1, -32, false, 0);
	Create_Bodys_World(20, 1, 4, 7, 4.8, -78, Trampoline, PB_Trampoline, 1, 0, false, true, 0, 0, 1, -32, false, 0);
	Create_Bodys_World(20, 1, 4, -9, 4.8, -72, Trampoline, PB_Trampoline, 2, 0, false, true, 0, 0, 1, 32, false, 0);
	Create_Bodys_World(20, 1, 4, -9, 4.8, -78, Trampoline, PB_Trampoline, 3, 0, false, true, 0, 0, 1, 32, false, 0);

	Create_Bodys_World(4, 1, 25, -31, 7, -40, Trampoline, PB_Trampoline, 4, 0, false, true, 1, 0, 0, -40, false, 0);
	Create_Bodys_World(4, 1, 25, -37, 7, -40, Trampoline, PB_Trampoline, 5, 0, false, true, 1, 0, 0, -40, false, 0);
	Create_Bodys_World(4, 1, 25, -31, 7, -21.5, Trampoline, PB_Trampoline, 6, 0, false, true, 1, 0, 0, 40, false, 0);
	Create_Bodys_World(4, 1, 25, -37, 7, -21.5, Trampoline, PB_Trampoline, 7, 0, false, true, 1, 0, 0, 40, false, 0);

	

	Create_Bodys_World(4, 1, 25, -31, 7, -4, Trampoline, PB_Trampoline, 8, 0, false, true, 1, 0, 0, -40, false, 0);
	Create_Bodys_World(4, 1, 25, -37, 7, -4, Trampoline, PB_Trampoline, 9, 0, false, true, 1, 0, 0, -40, false, 0);
	Create_Bodys_World(4, 1, 25, -31, 7, 14.5, Trampoline, PB_Trampoline, 10, 0, false, true, 1, 0, 0, 40, false, 0);
	Create_Bodys_World(4, 1, 25, -37, 7, 14.5, Trampoline, PB_Trampoline, 11, 0, false, true, 1, 0, 0, 40, false, 0);

	Create_Bodys_World(3, 2, 3, -37, 1, -13, Trampoline, PB_Trampoline, 12, 0, false, false, 0, 0, 0, 0, false, 0);
	Create_Bodys_World(3, 2, 3, -31, 1, -13, Trampoline, PB_Trampoline, 13, 0, false, false, 0, 0, 0, 0, false, 0);

	//BRIDGE CONSTRAINTS---------------------------------------------------------------------------------------------------
	App->physics->AddConstraintP2P(*PB_Circuit_Building[2], *PB_Bridge[0], vec3(10.1, 9.75, 2), vec3(-4.1, 0, 2));//Right
	App->physics->AddConstraintP2P(*PB_Circuit_Building[2], *PB_Bridge[0], vec3(10.1, 9.75, -2), vec3(-4.1, 0, -2));//Right
	App->physics->AddConstraintP2P(*PB_Bridge[0], *PB_Bridge[1], vec3(4, 0, 2), vec3(-4.2, 0, 2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[0], *PB_Bridge[1], vec3(4, 0, -2), vec3(-4.2, 0, -2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[1], *PB_Bridge[2], vec3(4, 0, 2), vec3(-4.2, 0, 2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[1], *PB_Bridge[2], vec3(4, 0, -2), vec3(-4.2, 0, -2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[2], *PB_Bridge[3], vec3(4, 0, 2), vec3(-4.2, 0, 2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[2], *PB_Bridge[3], vec3(4, 0, -2), vec3(-4.2, 0, -2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[3], *PB_Bridge[4], vec3(4, 0, 2), vec3(-4.2, 0, 2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[3], *PB_Bridge[4], vec3(4, 0, -2), vec3(-4.2, 0, -2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[4], *PB_Bridge[5], vec3(4, 0, 2), vec3(-4.2, 0, 2));//Left
	App->physics->AddConstraintP2P(*PB_Bridge[4], *PB_Bridge[5], vec3(4, 0, -2), vec3(-4.2, 0, -2));//Left

	App->physics->AddConstraintP2P(*PB_Bridge[5], *PB_Circuit_Building[3], vec3(4.1, 0, 2), vec3(-13, 9.75, 2));//Right
	App->physics->AddConstraintP2P(*PB_Bridge[5], *PB_Circuit_Building[3], vec3(4.1, 0, -2), vec3(-13, 9.75, -2));//Right


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	//Render Circuit------------------------------------------------

	PB_Pendulum[0]->GetTransform(&(Pendulum[0].transform));
	Pendulum[0].Render();

	PB_Pendulum[1]->GetTransform(&(Pendulum_1[0].transform));
	Pendulum_1[0].Render();

	PB_Pendulum[2]->GetTransform(&(Pendulum[1].transform));
	Pendulum[1].Render();

	PB_Pendulum[3]->GetTransform(&(Pendulum_1[1].transform));
	Pendulum_1[1].Render();

	PB_Pendulum[4]->GetTransform(&(Pendulum[2].transform));
	Pendulum[2].Render();

	PB_Pendulum[5]->GetTransform(&(Pendulum_1[2].transform));
	Pendulum_1[2].Render();

	PB_Pendulum[6]->GetTransform(&(Pendulum[3].transform));
	Pendulum[3].Render();

	PB_Pendulum[7]->GetTransform(&(Pendulum_1[3].transform));
	Pendulum_1[3].Render();

	PB_Pendulum[8]->GetTransform(&(Pendulum[4].transform));
	Pendulum[4].Render();

	PB_Pendulum[9]->GetTransform(&(Pendulum_1[4].transform));
	Pendulum_1[4].Render();

	PB_Pendulum[10]->GetTransform(&(Pendulum[5].transform));
	Pendulum[5].Render();

	PB_Pendulum[11]->GetTransform(&(Pendulum_1[5].transform));
	Pendulum_1[5].Render();


	for (int i = 0; i < 4; i++)
	{
		PB_Traffic_Light[i]->GetTransform(&(Traffic_Light[i].transform));
		Traffic_Light[i].Render();
	}


	for (int i = 0; i < 24; i++)
	{
		PB_Circuit_Start[i]->GetTransform(&(Circuit_Start[i].transform));

		if ((i % 2) == 1)
			Circuit_Start[i].color = Red;

		Circuit_Start[i].Render();

	}

	for (int i = 0; i < 259; i++)
	{
		PB_Circuit_Trace[i]->GetTransform(&(Circuit_Trace[i].transform));

		if ((i % 2) == 1)
			Circuit_Trace[i].color = Red;

		Circuit_Trace[i].Render();
	}
	
	for (int i = 0; i < 16; i++)
	{
		Bump[i].color = Red;
		Bump[i].Render();
	}
	
	for (int i = 0; i < 14; i++)
	{
		PB_Trampoline[i]->GetTransform(&(Trampoline[i].transform));
		Trampoline[i].Render();
	}


	
	for (int i = 0; i < 54; i++)
	{
		PB_Circuit_Curve[i]->GetTransform(&(Circuit_Curve[i].transform));

		if ((i % 2) == 1)
			Circuit_Curve[i].color = Red;

		Circuit_Curve[i].Render();
	}

	
	for (int i = 0; i < 24; i++)
	{
		PB_Circuit_Obstacles[i]->GetTransform(&(Circuit_Obstacles[i].transform));
		Circuit_Obstacles[i].Render();
	}


	for (int i = 0; i < 12; i++)
	{
		PB_Circuit_Building[i]->GetTransform(&(Circuit_Building[i].transform));
		if (i == 1 || i == 5 || i == 6 || i == 9)
			Circuit_Building[i].color = Red;
		Circuit_Building[i].Render();
	}
	
	
	for (int i = 0; i < 6; i++)
	{
		PB_Bridge[i]->GetTransform(&(Bridge[i].transform));
		if ((i % 2) == 1)
			Bridge[i].color = Red;

		Bridge[i].Render();
	}

	
	s[7].Render();
	s[8].Render();
	s[9].Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");
	//Effects of collisions, sensors and checkpoints------------------------------------
	if (body1 == sensor[0])
	{
		App->player->Reset_Vehicle(-40.6, 26, 160, 3.1415 / 2);

		PB_Pendulum[1]->Stop_Motion();
		PB_Pendulum[1]->SetPos(5.0f, 42.0f, 173);
		PB_Pendulum[3]->Stop_Motion();
		PB_Pendulum[3]->SetPos(-6.0f, 42.0f, 156);
		PB_Pendulum[5]->Stop_Motion();
		PB_Pendulum[5]->SetPos(-16.0f, 42.0f, 173);

	}

	if (body1 == sensor[1])
	{
		App->player->Reset_Vehicle(158, 15, 90, 3.1415);
		App->player->turn = 45;

		PB_Pendulum[7]->Stop_Motion();
		PB_Pendulum[9]->Stop_Motion();

		PB_Pendulum[7]->SetPos(173, 15, -3.5);
		PB_Pendulum[9]->SetPos(140, 15, -22.5);
	}


	if (body1 == sensor[2])
	{
		
		App->player->Reset_Vehicle(-80, 15.0f, 95, 3.1415 / 2);

	}

	if (body1 == sensor[3])
	{
		App->player->Reset_Vehicle(32, 0.5, -75, -(3.1415 / 2));
	}

	if (body1 == sensor[4] || body1 == sensor[5])
	{
		PB_Pendulum[11]->Stop_Motion();
		PB_Pendulum[11]->SetPos(-45, 30, -12.5);
		App->player->Reset_Vehicle(-34, 0.5, -75, 0);
	}

	if (body1 == sensor[6] && entry == true)
	{    	
		    sensor_end = true;
			PB_Pendulum[1]->Stop_Motion();
			PB_Pendulum[1]->SetPos(5.0f, 42.0f, 173);
			PB_Pendulum[3]->Stop_Motion();
			PB_Pendulum[3]->SetPos(-6.0f, 42.0f, 156);
			PB_Pendulum[5]->Stop_Motion();
			PB_Pendulum[5]->SetPos(-16.0f, 42.0f, 173);

			PB_Pendulum[7]->Stop_Motion();
			PB_Pendulum[9]->Stop_Motion();

			PB_Pendulum[7]->SetPos(173, 15, -3.5);
			PB_Pendulum[9]->SetPos(140, 15, -22.5);

			PB_Pendulum[11]->Stop_Motion();
			PB_Pendulum[11]->SetPos(-45, 30, -12.5);

			s[9].color = Blue;
			pos_check_point.x = 88;
			pos_check_point.y = 4;
			pos_check_point.z = -128;		
           	
	}

	if (body1 == sensor[7])
	{
		PB_Pendulum[1]->Stop_Motion();
		PB_Pendulum[1]->SetPos(5.0f, 42.0f, 173);
		PB_Pendulum[3]->Stop_Motion();
		PB_Pendulum[3]->SetPos(-6.0f, 42.0f, 156);
		PB_Pendulum[5]->Stop_Motion();
		PB_Pendulum[5]->SetPos(-16.0f, 42.0f, 173);

		s[7].color = Green;
		pos_check_point.x = -135;
		pos_check_point.y = 5;
		pos_check_point.z = 159;
	}

	if (body1 == sensor[8])
	{
		s[7].color = Blue;
		s[8].color = Green; 

		PB_Pendulum[11]->Stop_Motion();
		PB_Pendulum[11]->SetPos(-45, 30, -12.5);

		pos_check_point.x = 31;
		pos_check_point.y = 4;
		pos_check_point.z = -55;
	}

	if (body1 == sensor[9])
	{
		PB_Pendulum[7]->Stop_Motion();
		PB_Pendulum[9]->Stop_Motion();
		PB_Pendulum[7]->SetPos(173, 15, -3.5);
		PB_Pendulum[9]->SetPos(140, 15, -22.5);

		s[8].color = Blue;
		s[9].color = Green; 
		

		pos_check_point.x = -80;
		pos_check_point.y = 4;
		pos_check_point.z = 95;
	}

	if (body1 == sensor[10])
	{
		entry = true;
	}

	

}

void  ModuleSceneIntro::Create_Bodys_World(float size_x, float size_y, float size_z, float pos_x, float pos_y, float pos_z, Cube primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass, bool sensor, bool rotation, float r_x, float r_y, float r_z, int angle, bool num_primitive, int num_prim)
{
	primitive_cube[PB_Num].Create_Cube(size_x, size_y, size_z, pos_x, pos_y, pos_z);
	if (rotation == true)
	{
		primitive_cube[PB_Num].SetRotation(angle, vec3(r_x, r_y, r_z));
	}

	if (sensor == true)
	{
		if (num_primitive == true)
		{
          PB_3D[PB_Num + num_prim] = App->physics->AddBody(primitive_cube[PB_Num], mass);
		  PB_3D[PB_Num + num_prim]->SetAsSensor(true);
		  PB_3D[PB_Num + num_prim]->collision_listeners.add(this);
		}
		else
		{
			PB_3D[PB_Num] = App->physics->AddBody(primitive_cube[PB_Num], mass);
			PB_3D[PB_Num]->SetAsSensor(true);
			PB_3D[PB_Num]->collision_listeners.add(this);
		}
		
	}
	else
	{
		if (num_primitive == true)
		{
			PB_3D[PB_Num + num_prim] = App->physics->AddBody(primitive_cube[PB_Num], mass);
		}
		else
		{
			PB_3D[PB_Num] = App->physics->AddBody(primitive_cube[PB_Num], mass);
		}
	}
	
}

void ModuleSceneIntro::Create_Circuit_World(float size_x, float size_y, float size_z, float pos_x, float pos_y, float pos_z, Cube primitive_cube[], PhysBody3D* PB_3D[], int init_Num, int final_num, float parameter, float mass, bool distance, bool height, bool axis_x, bool positive, int value)
{
	for (int i = init_Num, height_or_distance = parameter; i < final_num; i++)
	{
		if (height == true)
		{
           primitive_cube[i].Create_Cube(size_x, size_y, size_z, pos_x, height_or_distance, pos_z);
		   PB_3D[i] = App->physics->AddBody(primitive_cube[i], mass);
		   height_or_distance += 2;
		}
		else
		{
			if (axis_x == true)
			{
			   primitive_cube[i].Create_Cube(size_x, size_y, size_z, height_or_distance, pos_y, pos_z);
			   PB_3D[i] = App->physics->AddBody(primitive_cube[i], mass);
			   
			}
			else
			{
				primitive_cube[i].Create_Cube(size_x, size_y, size_z, pos_x, pos_y, height_or_distance);
				PB_3D[i] = App->physics->AddBody(primitive_cube[i], mass);
			}
			
             if (positive == true)
				 height_or_distance += value;
			 else
				 height_or_distance -= value;
		}
		
	}
}

void  ModuleSceneIntro::Create_Curves(float size_x, float size_y, float size_z, float pos_x_a, float pos_y_a, float pos_z_a, Cube primitive_cube[], PhysBody3D* PB_3D[], int init_Num, int final_num, float parameter_1, float parameter_2, float parameter_3, float parameter_4, float parameter_5, float value_parameter_1_z, float value_parameter_2_z, float value_parameter_3_z, float value_parameter_4_z, float value_parameter_5_z,
	float value_parameter_1_x, float value_parameter_2_x, float value_parameter_3_x, float value_parameter_4_x, float value_parameter_5_x)
{
	for (int i = init_Num, distance = pos_x_a, pos_z = pos_z_a; i < final_num; i++)
	{
		

		if (i == parameter_1)
		{
			pos_z += value_parameter_1_z;
			distance += value_parameter_1_x;
		}

		if (i == parameter_2)
		{
			pos_z += value_parameter_2_z;
			distance += value_parameter_2_x;
		}

		if (i == parameter_3)
		{
			pos_z += value_parameter_3_z;
			distance += value_parameter_3_x;
		}

		if (i == parameter_4)
		{
			pos_z += value_parameter_4_z;
			distance += value_parameter_4_x;
		}

		if (i == parameter_5)
		{
			pos_z += value_parameter_5_z;
			distance += value_parameter_5_x;
		}

		primitive_cube[i].Create_Cube(size_x, size_y, size_z, distance, pos_y_a, pos_z);
		PB_3D[i] = App->physics->AddBody(primitive_cube[i], 0);


	}
}


void  ModuleSceneIntro::Create_Cylinder_Bodys_World(float radius_a, float height_a, float pos_x, float pos_y, float pos_z, Cylinder primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass)
{
	primitive_cube[PB_Num].radius = radius_a;
	primitive_cube[PB_Num].height = height_a;
	primitive_cube[PB_Num].SetPos(pos_x, pos_y, pos_z);
	PB_3D[PB_Num] = App->physics->AddBody(primitive_cube[PB_Num], mass);
}

void  ModuleSceneIntro::Create_Sphere_Bodys_World(float radius_a, float pos_x, float pos_y, float pos_z, Sphere primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass, bool num_primitive, int num_prim)
{   
	primitive_cube[PB_Num].radius = radius_a;
	primitive_cube[PB_Num].SetPos(pos_x, pos_y, pos_z);
	if (num_primitive == true)
	{
		PB_3D[PB_Num + num_prim] = App->physics->AddBody(primitive_cube[PB_Num], mass);
	}
	else
	{
		PB_3D[PB_Num] = App->physics->AddBody(primitive_cube[PB_Num], mass);
	}
}