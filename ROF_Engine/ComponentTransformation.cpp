#include "ComponentTransformation.h"
#include "Globals.h"
#include "GameObject.h"

ComponentTransformation::ComponentTransformation(GameObject* bearer, int id) : Component(bearer, Type::Transformation, id)
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

void ComponentTransformation::OnSave(pugi::xml_node &scene)
{
	scene = scene.append_child("Transformation");

	scene.append_child("Type").append_attribute("Value") = this->GetType();

	scene = scene.append_child("Translate");
	AddXMLVector3(scene, position);
	scene = scene.parent();

	scene = scene.append_child("Scale");
	AddXMLVector3(scene, scale);
	scene = scene.parent();

	scene = scene.append_child("Rotation");
	AddXMLVector3(scene, GetRotation());
	scene = scene.parent().parent();
}

void ComponentTransformation::OnLoad(pugi::xml_node &scene)
{
	SetPos(GetXMLVector3(scene.child("Transformation"), "Translate"));
	SetScale(GetXMLVector3(scene.child("Transformation"), "Scale"));
	SetRotEuler(GetXMLVector3(scene.child("Transformation"), "Rotation"));
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

const float4x4* ComponentTransformation::GetGlobalMatrixT() const
{
	return &global_transform_matrix_t;
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

void ComponentTransformation::SetPos(vec new_pos)
{
	SetPos(new_pos.x, new_pos.y, new_pos.z);
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
		rotation_rad = DegToRad(vec(x, y, z));
		rotation = rotation.FromEulerXYZ(rotation_rad.x, rotation_rad.y, rotation_rad.z);

		//LOG("Quaternion is = X: %f Y: %f Z: %f W: %f", rotation.x, rotation.y, rotation.z, rotation.w);

		BuildTransMatrix();
	}
}

void ComponentTransformation::SetRotEuler(vec new_rot)
{
	SetRotEuler(new_rot.x, new_rot.y, new_rot.z);
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

void ComponentTransformation::SetScale(vec new_sca)
{
	SetScale(new_sca.x, new_sca.y, new_sca.z);
}
