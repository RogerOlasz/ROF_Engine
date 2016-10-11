#include "ComponentTransformation.h"
#include "Globals.h"

#include "SDL\include\SDL_opengl.h"

ComponentTransformation::ComponentTransformation(GameObject* bearer) : Component(bearer, Types::Transformation)
{

}

ComponentTransformation::~ComponentTransformation()
{

}

void ComponentTransformation::BuildTransMatrix()
{
	TransformMatrix = float4x4::FromTRS(position, rotation, scale);
	TransformMatrix.Transpose();
}

void ComponentTransformation::PushMatrix()
{
	glPushMatrix();
	glMultMatrixf((float*)&TransformMatrix);
}

void ComponentTransformation::PopMatrix()
{
	glPopMatrix();
}

void ComponentTransformation::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	BuildTransMatrix();
}

void ComponentTransformation::SetRot(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);

	BuildTransMatrix();
}

void ComponentTransformation::SetScale(float x, float y, float z)
{
	if (x != 0 && y != 0 && z != 0)
	{
		scale.Set(x, y, z);
	}

	BuildTransMatrix();
}
