#ifndef __PANELCOMPONENTS_H__
#define __PANELCOMPONENTS_H__

#include "Panel.h"

class GameObject;

class PanelComponents : public Panel
{
public:

	PanelComponents();
	virtual ~PanelComponents();

	void Draw(GameObject* selected_go);

private:

};

#endif // !__PANELCOMPONENTS_H__

