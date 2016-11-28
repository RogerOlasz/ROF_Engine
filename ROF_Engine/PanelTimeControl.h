#ifndef __PANELTIMECONTROL_H__
#define __PANELTIMECONTROL_H__

#include "ImGui/imgui.h"
#include "Panel.h"

class GameTimeManager;

class PanelTimeControl : public Panel
{
public:
	PanelTimeControl();
	~PanelTimeControl();

	void Draw();

private:
	float game_time_speed = 0.0f;

};

#endif // !__PANELTIMECONTROL_H__
