#include "GameTimeManager.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "Timer.h"

GameTimeManager::GameTimeManager()
{
	game_time = 0.0f;
	game_speed = 1.0f;
	game_dt = 0.0f;
	time_since_startup = 0.0f;
}

GameTimeManager::~GameTimeManager()
{

}

void GameTimeManager::Start()
{
	global_game_time.Start();
	ticker.Start();
	game_time = 0.0f;
	game_started = true;
}

void GameTimeManager::Tick()
{
	game_dt = (float)(ticker.Read() * game_speed);
	game_time += (game_dt / 1000.0f);
	ticker.Start();
	time_since_startup = global_game_time.ReadSec();
}

void GameTimeManager::Stop()
{
	global_game_time.Stop();
	game_started = false;
}

void GameTimeManager::SetTimeSpeed(float new_speed)
{
	game_speed = new_speed;
}

float GameTimeManager::GetGameTimeSpeed() const
{
	return game_speed;
}

float GameTimeManager::GetGameTime() const
{
	return game_time;
}

float GameTimeManager::GetGameDt() const
{
	return game_dt;
}

float GameTimeManager::GetTimeSinceStartUp() const
{
	return time_since_startup;
}


bool GameTimeManager::GetGameState() const
{
	return game_started;
}