#ifndef __COMPONENTTRANSFORMATION_H__
#define __COMPONENTTRANSFORMATION_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

using namespace math;

class ComponentTransformation : public Component
{
public:
	ComponentTransformation(GameObject* bearer, int id);
	~ComponentTransformation();

private:
	Quat rotation = Quat::identity;
	vec position = vec::zero;
	vec scale = vec::one;

	vec rotation_rad = vec::zero;

	float4x4 transform_matrix = float4x4::identity;
	float4x4 global_transform_matrix = float4x4::identity;

public:
	void BuildTransMatrix();
	void PushMatrix();
	void PopMatrix();

	void UpdateGlobalMatrix();

	vec GetPosition() const;
	vec GetScale() const;
	vec GetRotation() const;

	float4x4 GetGlobalMatrix() const;
	float4x4 GetLocalMatrix() const;

	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z, float w);
	void SetRotEuler(float x, float y, float z);
	void SetScale(float x, float y, float z);

};

#endif // !__COMPONENTTRANSFORMATION_H__