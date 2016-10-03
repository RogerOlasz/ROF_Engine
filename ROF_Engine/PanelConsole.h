#ifndef __PANELCONSOLE_H__
#define __PANELCONSOLE_H__

#include "ImGui\imgui.h"

class PanelConsole
{
private:
	ImVector<char*> Items;

public:

	PanelConsole();
	~PanelConsole();

	void AddLog(const char* log);
	void Draw();

public:
	bool c_open = true;
};

#endif // __PANELCONSOLE_H__
