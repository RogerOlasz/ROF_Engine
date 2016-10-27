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

	void BuildTransMatrix();
	void PushMatrix();
	void PopMatrix();

	vec GetPosition() const;
	vec GetScale() const;
	vec GetRotation() const;

	float4x4 GetGlobalMatrix() const;
	const float4x4* GetGlobalMatrixT() const;
	float4x4 GetLocalMatrix() const;

	void UpdateGlobalMatrix();

	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z, float w);
	void SetRotEuler(float x, float y, float z);
	void SetScale(float x, float y, float z);

private:
	Quat rotation = Quat::identity;
	vec position = vec::zero;
	vec scale = vec::one;

	vec rotation_rad = vec::zero;

	float4x4 transform_matrix = float4x4::identity;
	float4x4 global_transform_matrix = float4x4::identity;
	float4x4 global_transform_matrix_t = float4x4::identity;

public:
	bool global_matrix_changed = false;
	bool freeze = false;

};

#endif // !__COMPONENTTRANSFORMATION_H__