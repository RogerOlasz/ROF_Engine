#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Globals.h"
#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* bearer, int id);
	~ComponentMaterial();

	void OnSave(pugi::xml_node&);
	void OnLoad(pugi::xml_node&);
	
};

#endif // !__COMPONENTMATERIAL_H__