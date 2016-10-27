#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"
#include "Color.h"
#include "GameObject.h"
#include "DebugPainter.h"

ComponentCamera::ComponentCamera(GameObject* bearer, int id) : Component(bearer, Type::Camera, id)
{
	active = true;
	char tmp[SHORT_STRING];
	sprintf(tmp, "Camera##%d", id);
	name = tmp;

	camera_frustum.SetKind(FrustumSpaceGL, FrustumRightHanded); //OpenGl works under right hand rules for normals
	camera_frustum.SetPos(vec(0.0f, 0.0f, 0.0f));
	camera_frustum.SetFront(vec::unitZ);
	camera_frustum.SetUp(vec::unitY);
	//Set near and far planes
	camera_frustum.SetViewPlaneDistances(5.0f, 100.0f);
	//Set horizontalFov and verticalFov
	camera_frustum.SetPerspective(DEGTORAD * 60.0f, DEGTORAD * 60.0f);

	UpdatePlanes();
}

ComponentCamera::~ComponentCamera()
{
	//RELEASE(f_planes);
}

void ComponentCamera::Update()
{
	//TODO This line here must be temporal to debug
	if (debug_draw)
	{
		DebugDraw(camera_frustum, Blue);
	}	
}

void ComponentCamera::LookAt(const vec &position)
{
	vec dir = position - camera_frustum.Pos();

	//Look at doc: http://clb.demon.fi/MathGeoLib/nightly/docs/float3x3_LookAt.php
	float3x3 lookat_matrix = float3x3::LookAt(camera_frustum.Front(), dir.Normalized(), camera_frustum.Up(), vec::unitY);

	camera_frustum.SetFront(lookat_matrix.MulDir(camera_frustum.Front()).Normalized());

	camera_frustum.SetUp(lookat_matrix.MulDir(camera_frustum.Up()).Normalized());
}

//Gets ------------------------------------------------------------------------------
float* ComponentCamera::GetViewMatrix() const
{
	float4x4 tmp = camera_frustum.ViewMatrix();
	tmp.Transpose();

	return *tmp.v;
}

float* ComponentCamera::GetProjectionMatrix() const
{
	float4x4 tmp = camera_frustum.ProjectionMatrix();
	tmp.Transpose();

	return *tmp.v;
}

const float ComponentCamera::GetNearPlane() const
{
	return camera_frustum.NearPlaneDistance();
}

const float ComponentCamera::GetFarPlane() const
{
	return camera_frustum.FarPlaneDistance();
}

const float ComponentCamera::GetFOV() const
{
	return camera_frustum.VerticalFov() * RADTODEG;
}

const float ComponentCamera::GetAspectRatio() const
{
	return camera_frustum.AspectRatio();
}

const vec ComponentCamera::GetFront() const
{
	return camera_frustum.Front();
}

const vec ComponentCamera::GetWorldRight() const
{
	return camera_frustum.WorldRight();
}

const vec ComponentCamera::GetUp() const
{
	return camera_frustum.Up();
}

const vec ComponentCamera::GetPos() const
{
	return camera_frustum.Pos();
}

//Sets-------------------------------------------------------------------------------------
void ComponentCamera::SetNearPlane(float new_distance)
{
	if (new_distance != camera_frustum.NearPlaneDistance() && new_distance > 0 && new_distance < camera_frustum.FarPlaneDistance())
	{
		camera_frustum.SetViewPlaneDistances(new_distance, camera_frustum.FarPlaneDistance());
		UpdatePlanes();
	}		
}

void ComponentCamera::SetFarPlane(float new_distance)
{
	if (new_distance != camera_frustum.FarPlaneDistance() && new_distance > 0 && new_distance > camera_frustum.NearPlaneDistance())
	{
		camera_frustum.SetViewPlaneDistances(camera_frustum.NearPlaneDistance(), new_distance);
		UpdatePlanes();
	}		
}

void ComponentCamera::SetFOV(float new_fov)
{
	//Should add some limits?
	camera_frustum.SetVerticalFovAndAspectRatio((new_fov * DEGTORAD), camera_frustum.AspectRatio());
	UpdatePlanes();
}

void ComponentCamera::SetAspectRatio(float new_aspect_ratio)
{
	//Should add some limits?
	camera_frustum.SetHorizontalFovAndAspectRatio(camera_frustum.HorizontalFov(), new_aspect_ratio);
	UpdatePlanes();
}

void ComponentCamera::SetPos(vec new_position)
{
	camera_frustum.SetPos(new_position);
}

//ACTUALLY USELESS
bool ComponentCamera::Intersects(AABB &aabb)
{
	bool ret = false;
	
	vec vertex[8];
	aabb.GetCornerPoints(vertex);

	for (uint i = 0; i < 6; i++)
	{
		for (uint j = 0; j < 8; j++)
		{
			if (f_planes[i].IsOnPositiveSide(vertex[j]) == false)
			{
				ret = true;
			}
		}
	}

	return ret;
}

void ComponentCamera::UpdatePlanes()
{
	f_planes[0] = camera_frustum.NearPlane();
	f_planes[1] = camera_frustum.FarPlane();
	f_planes[2] = camera_frustum.LeftPlane();
	f_planes[3] = camera_frustum.RightPlane();
	f_planes[4] = camera_frustum.BottomPlane();
	f_planes[5] = camera_frustum.TopPlane();
}
