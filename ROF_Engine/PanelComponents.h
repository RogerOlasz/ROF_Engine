#ifndef __PANELCOMPONENTS_H__
#define __PANELCOMPONENTS_H__

#include "Panel.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class PanelComponents : public Panel
{
public:

	PanelComponents();
	virtual ~PanelComponents();

	void Draw(GameObject* selected_go);

};

#endif // !__PANELCOMPONENTS_H__

