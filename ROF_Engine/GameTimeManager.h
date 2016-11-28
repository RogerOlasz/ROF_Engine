#ifndef __GAMETIMEMANAGER_H__
#define __GAMETIMEMANAGER_H__

#include "Timer.h"

class GameTimeManager
{
public:
	GameTimeManager();
	~GameTimeManager();

	void Start();
	void Tick();
	void Stop();
	
	void SetTimeSpeed(float new_speed);

	float GetGameTimeSpeed() const;
	float GetGameTime() const;
	float GetGameDt() const;
	float GetTimeSinceStartUp() const;

	bool GetGameState() const;

private:
	Timer global_game_time;
	Timer ticker;
	float game_time = 0.0f;	
	float game_speed = 1.0f;
	float time_since_startup = 0.0f;

	float logic_fps;
	float game_dt;

	bool game_started = false;

};

#endif // !__GAMETIMEMANAGER_H__