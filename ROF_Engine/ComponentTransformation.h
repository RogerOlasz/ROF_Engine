#ifndef __COMPONENTTRANSFORMATION_H__
#define __COMPONENTTRANSFORMATION_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

using namespace math;

class ComponentTransformation : public Component
{
public:
	ComponentTransformation(GameObject* bearer);
	~ComponentTransformation();

private:
	Quat rotation = Quat::identity;
	vec position = vec::zero;
	vec scale = vec::zero;

public:
	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z, float w);
	void SetScale(float x, float y, float z);

};

#endif // !__COMPONENTTRANSFORMATION_H__