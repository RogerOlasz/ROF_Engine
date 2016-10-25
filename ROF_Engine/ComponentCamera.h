#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* bearer, int id);
	~ComponentCamera();

	void Update();

	void LookAt(const vec &position);

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;

	const float GetNearPlane() const;
	const float GetFarPlane() const;
	const float GetFOV() const;
	const float GetAspectRatio() const;
	const vec GetFront() const;
	const vec GetWorldRight() const;
	const vec GetUp() const;
	const vec GetPos() const;

	void SetNearPlane(float new_distance);
	void SetFarPlane(float new_distance);
	void SetFOV(float new_fov);
	void SetAspectRatio(float new_aspect_ratio);
	void SetPos(vec new_position);

private:
	//Help here: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php
	Frustum camera_frustum;
	bool frustum_culling = false;

public:
	bool debug_draw = true;

};

#endif // !__COMPONENTCAMERA_H__

