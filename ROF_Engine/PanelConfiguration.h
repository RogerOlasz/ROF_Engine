#pragma once
#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

#include "ImGui\imgui.h"

class PanelConfiguration
{
private:

public:

	PanelConfiguration();
	~PanelConfiguration();

	void Draw(bool* open);

public:
	bool c_open = true;
};

#endif // __PANELCONFIGURATION_H__

