#ifndef __PANELCOMPONENTS_H__
#define __PANELCOMPONENTS_H__

#include "Panel.h"
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class PanelComponents : public Panel
{
public:

	PanelComponents();
	virtual ~PanelComponents();

	void Draw(GameObject* selected_go);

private:
	GameObject* last_go;

	vec pos = vec::zero;
	vec sca = vec::zero;
	vec rot = vec::zero;
};

#endif // !__PANELCOMPONENTS_H__

