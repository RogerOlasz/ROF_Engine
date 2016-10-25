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

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetFOV() const;
	float GetAspectRatio() const;
	vec GetPos() const;

	void SetNearPlane(float distance);
	void SetFarPlane(float distance);
	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);

public:
	//Help here: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php
	Frustum camera_frustum;

	bool frustum_culling = false;
	bool debug_draw = true;

};

#endif // !__COMPONENTCAMERA_H__

