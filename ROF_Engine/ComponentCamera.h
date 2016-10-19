#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Color;

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* bearer, int id);
	~ComponentCamera();

	void Update();

	void LookAt(const vec &position);

	float GetFOV() const;

	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);

public:
	//Help here: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php
	Frustum camera_frustum;
	float fov;
	float aspect_ratio;

	bool frustum_culling = false;

};

#endif // !__COMPONENTCAMERA_H__

