#include "ComponentTransformation.h"
#include "Globals.h"

#include "SDL/include/SDL_opengl.h"

ComponentTransformation::ComponentTransformation(GameObject* bearer) : Component(bearer, Types::Transformation)
{

}

ComponentTransformation::~ComponentTransformation()
{

}

void ComponentTransformation::BuildTransMatrix()
{
	transform_matrix = float4x4::FromTRS(position, rotation, scale);
	LOG("Position after build matrix is %f", position.x);
	transform_matrix.Transpose();
}

void ComponentTransformation::PushMatrix()
{
	glPushMatrix();
	glMultMatrixf((float*)&transform_matrix);
}

void ComponentTransformation::PopMatrix()
{
	glPopMatrix();
}

vec ComponentTransformation::GetPosition() const
{
	return position;
}

vec ComponentTransformation::GetScale() const
{
	return scale;
}

vec ComponentTransformation::GetRotation() const
{
	vec tmp;
	tmp = rotation.ToEulerXYZ();
	tmp = RadToDeg(tmp);

	return tmp;
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

void ComponentTransformation::SetRotEuler(float x, float y, float z)
{
	rotation_deg = vec(x, y, z);
	rotation_rad = DegToRad(rotation_deg);
	rotation = rotation.FromEulerXYX(rotation_rad.x, rotation_rad.y, rotation_rad.z);
	
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
