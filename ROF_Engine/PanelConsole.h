#ifndef __PANELCONSOLE_H__
#define __PANELCONSOLE_H__

#include "ImGui/imgui.h"
#include "Panel.h"

class PanelConsole : public Panel
{
private:
	ImVector<char*> Items;

public:

	PanelConsole();
	virtual ~PanelConsole();

	void AddLog(const char* log);

	void Draw();

private:
	bool scroll_to_bottom = false;

};

#endif // __PANELCONSOLE_H__
