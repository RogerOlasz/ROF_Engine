#pragma once
#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

#include "ImGui\imgui.h"
#include <vector>

class PanelConfiguration
{
private:
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	int max_fps = 60;

public:

	PanelConfiguration();
	~PanelConfiguration();

	void Draw(bool* open);
	void Log(const float* fps, const float ms);
	unsigned int GetMaxFPS();

public:
	bool c_open = true;
};

#endif // __PANELCONFIGURATION_H__

