#ifndef __PANELTIMECONTROL_H__
#define __PANELTIMECONTROL_H__

#include "ImGui/imgui.h"
#include "Panel.h"

class PanelTimeControl : public Panel
{
public:
	PanelTimeControl();
	virtual ~PanelTimeControl();

	void Draw();

private:

};

#endif // !__PANELTIMECONTROL_H__
