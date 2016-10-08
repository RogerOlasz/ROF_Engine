#ifndef __COMPONENTTRANSFORMATION_H__
#define __COMPONENTTRANSFORMATION_H__

#include "Component.h"

class ComponentTransformation : public Component
{
public:
	ComponentTransformation(GameObject* bearer);
	~ComponentTransformation();

};

#endif // !__COMPONENTTRANSFORMATION_H__