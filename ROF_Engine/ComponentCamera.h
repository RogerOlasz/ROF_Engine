#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* bearer, int id);
	~ComponentCamera();

	void Update();

	void OnSave(pugi::xml_node&);
	void OnLoad(pugi::xml_node&);

	void LookAt(const vec &position);

	const float* GetViewMatrix() const;
	const float* GetProjectionMatrix() const;

	const float GetNearPlane() const;
	const float GetFarPlane() const;
	const float GetFOV() const;
	const float GetAspectRatio() const;
	const vec GetFront() const;
	const vec GetWorldRight() const;
	const vec GetUp() const;
	const vec GetPos() const;
	const Frustum GetFrustum() const;

	void SetNearPlane(float new_distance);
	void SetFarPlane(float new_distance);
	void SetFOV(float new_fov);
	void SetAspectRatio(float new_aspect_ratio);
	void SetPos(vec new_position);
	void SetPos(float4x4* transform);
	void SetFront(float4x4* transform);
	void SetUp(float4x4* transform);
	void SetFrame(float4x4* transform);
	void SetFrame(vec new_pos, vec front_dir, vec up_dir);

	bool Intersects(const AABB &aabb);

private:
	//Help here: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php
	Frustum camera_frustum;

public:
	bool debug_draw = true;
	bool frustum_culling = false;

};

#endif // !__COMPONENTCAMERA_H__

