#include "ComponentTransformation.h"
#include "Globals.h"
#include "GameObject.h"

#include "SDL/include/SDL_opengl.h"

ComponentTransformation::ComponentTransformation(GameObject* bearer, int id) : Component(bearer, Types::Transformation, id)
{
	//Component names are to solve problems with ImGui same names
	char tmp[SHORT_STRING];
	sprintf(tmp, "Transformation##%d", id);
	name = tmp;

	if (game_object->GetParent())
	{
		global_transform_matrix = game_object->GetParent()->transform->global_transform_matrix * transform_matrix;
	}
	else
	{
		global_transform_matrix = transform_matrix;
	}

	global_transform_matrix_t = global_transform_matrix.Transposed();
}

ComponentTransformation::~ComponentTransformation()
{

}

void ComponentTransformation::BuildTransMatrix()
{
	transform_matrix = float4x4::FromTRS(position, rotation, scale);
	global_matrix_changed = true;
	UpdateGlobalMatrix();
}

void ComponentTransformation::PushMatrix()
{
	glPushMatrix();
	glMultMatrixf((float*)&global_transform_matrix_t);
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
	vec ret;
	ret = RadToDeg(rotation.ToEulerXYZ());

	return ret;
}

float4x4 ComponentTransformation::GetGlobalMatrix() const
{
	return global_transform_matrix;
}

float4x4 ComponentTransformation::GetLocalMatrix() const
{
	return transform_matrix;
}

//Update global matrix
void ComponentTransformation::UpdateGlobalMatrix()
{
	global_transform_matrix = game_object->GetParent()->transform->global_transform_matrix * transform_matrix;
	global_transform_matrix_t = global_transform_matrix.Transposed();
}

void ComponentTransformation::SetPos(float x, float y, float z)
{
	if (freeze == false)
	{
		position.x = x;
		position.y = y;
		position.z = z;

		BuildTransMatrix();
	}	
}

void ComponentTransformation::SetRot(float x, float y, float z, float w)
{
	if (freeze == false)
	{
		rotation.Set(x, y, z, w);

		BuildTransMatrix();
	}
}

void ComponentTransformation::SetRotEuler(float x, float y, float z)
{
	if (freeze == false)
	{
		while (x < 0) { x += 360; }
		while (y < 0) { y += 360; }
		while (z < 0) { z += 360; }

		rotation_rad = DegToRad(vec(x, y, z));
		rotation = rotation.FromEulerXYZ(rotation_rad.x, rotation_rad.y, rotation_rad.z);

		//LOG("Quaternion is = X: %f Y: %f Z: %f W: %f", rotation.x, rotation.y, rotation.z, rotation.w);

		BuildTransMatrix();
	}
}

void ComponentTransformation::SetScale(float x, float y, float z)
{
	if (freeze == false)
	{
		if (x != 0 && y != 0 && z != 0)
		{
			scale.Set(x, y, z);
		}

		BuildTransMatrix();
	}
}
