#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Globals.h"
#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* bearer);
	~ComponentMaterial();

	void Init();

private:

};

#endif // !__COMPONENTMATERIAL_H__