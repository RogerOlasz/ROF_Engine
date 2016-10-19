#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "SDL/include/SDL_opengl.h"
#include "Color.h"
#include "GameObject.h"
#include "DebugPainter.h"

ComponentCamera::ComponentCamera(GameObject* bearer, int id) : Component(bearer, Types::Camera, id)
{
	active = true;
	char tmp[SHORT_STRING];
	sprintf(tmp, "Camera##%d", id);

	camera_frustum.type = FrustumType::PerspectiveFrustum;

	camera_frustum.pos = vec(0.0f, 10.0f, 0.0f);
	camera_frustum.front = vec::unitZ;
	camera_frustum.up = vec::unitY;

	camera_frustum.nearPlaneDistance = 5.0f;
	camera_frustum.farPlaneDistance = 15.0f;
	camera_frustum.verticalFov = DEGTORAD * 60.0f;
	camera_frustum.horizontalFov = DEGTORAD * 60.0f;
}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update()
{
	DebugDraw(camera_frustum, Blue);
}

void ComponentCamera::LookAt(const vec& position)
{
	vec dir = position - camera_frustum.pos;

	float3x3 matrix = float3x3::LookAt(camera_frustum.front, dir.Normalized(), camera_frustum.up, float3::unitY);

	camera_frustum.front = matrix.MulDir(camera_frustum.front).Normalized();
	camera_frustum.up = matrix.MulDir(camera_frustum.up).Normalized();
}

