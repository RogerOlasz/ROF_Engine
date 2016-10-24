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
	name = tmp;

	camera_frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	camera_frustum.SetPos(vec(0.0f, 10.0f, 0.0f));
	camera_frustum.SetFront(vec::unitZ);
	camera_frustum.SetUp(vec::unitY);
	//Set near and far planes
	camera_frustum.SetViewPlaneDistances(5.0f, 15.0f);
	//Set horizontalFov and verticalFov
	camera_frustum.SetPerspective(DEGTORAD * 60.0f, DEGTORAD * 60.0f);
}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update()
{
	//TODO This line here must be temporal to debug
	if (debug_draw)
	{
		DebugDraw(camera_frustum, Blue);
	}	
}

void ComponentCamera::LookAt(const vec& position)
{
	vec dir = position - camera_frustum.Pos();

	float3x3 lookat_matrix = float3x3::LookAt(camera_frustum.Front(), dir.Normalized(), camera_frustum.Up(), vec::unitY);

	camera_frustum.SetFront(lookat_matrix.MulDir(camera_frustum.Front()).Normalized());
	camera_frustum.SetUp(lookat_matrix.MulDir(camera_frustum.Up()).Normalized());
}

float* ComponentCamera::GetViewMatrix()
{
	float4x4 tmp = camera_frustum.ViewMatrix();
	tmp.Transpose();

	return *tmp.v;
}

float* ComponentCamera::GetProjectionMatrix()
{
	float4x4 tmp = camera_frustum.ProjectionMatrix();
	tmp.Transpose();

	return *tmp.v;
}

float ComponentCamera::GetNearPlane() const
{
	return camera_frustum.NearPlaneDistance();
}

float ComponentCamera::GetFarPlane() const
{
	return camera_frustum.FarPlaneDistance();
}

float ComponentCamera::GetFOV() const
{
	return camera_frustum.VerticalFov() * RADTODEG;
}

float ComponentCamera::GetAspectRatio() const
{
	return camera_frustum.AspectRatio();
}

void ComponentCamera::SetNearPlane(float distance)
{
	if (distance > 0 && distance < camera_frustum.FarPlaneDistance())
	{
		camera_frustum.SetViewPlaneDistances(distance, camera_frustum.FarPlaneDistance());
	}		
}

void ComponentCamera::SetFarPlane(float distance)
{
	if (distance > 0 && distance > camera_frustum.NearPlaneDistance())
	{
		camera_frustum.SetViewPlaneDistances(camera_frustum.NearPlaneDistance(), distance);
	}		
}

void ComponentCamera::SetFOV(float fov)
{
	camera_frustum.SetVerticalFovAndAspectRatio((fov * DEGTORAD), camera_frustum.AspectRatio());
}

void ComponentCamera::SetAspectRatio(float aspect_ratio)
{
	camera_frustum.SetHorizontalFovAndAspectRatio(camera_frustum.HorizontalFov(), aspect_ratio);
}

