#include "ComponentTransformation.h"
#include "Globals.h"

ComponentTransformation::ComponentTransformation(GameObject* bearer) : Component(bearer, Types::Geometry)
{

}

ComponentTransformation::~ComponentTransformation()
{

}

void ComponentTransformation::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void ComponentTransformation::SetRot(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);
}

void ComponentTransformation::SetScale(float x, float y, float z)
{
	if (x != 0 && y != 0 && z != 0)
	{
		scale.Set(x, y, z);
	}
}
