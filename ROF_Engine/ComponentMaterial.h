#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* bearer);
	~ComponentMaterial();

};

#endif // !__COMPONENTMATERIAL_H__