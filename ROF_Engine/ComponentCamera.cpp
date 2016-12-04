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

	camera_frustum.SetKind(FrustumSpaceGL, FrustumRightHanded); //If a Frustum is right-handed, then the camera looks towards -Z, +Y is up, and +X is right. 
	camera_frustum.SetFrame(vec(0.0f, 0.0f, 0.0f), vec::unitZ, vec::unitY); //Faster than set one by one (pos, front and up)
	//Set near and far planes
	camera_frustum.SetViewPlaneDistances(5.0f, 100.0f);
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

void ComponentCamera::OnSave(pugi::xml_node &scene)
{
	scene = scene.append_child("Camera");

	scene.append_child("Type").append_attribute("Value") = this->GetType();

	scene.append_child("NearPlane").append_attribute("Value") = this->GetNearPlane();
	scene.append_child("FarPlane").append_attribute("Value") = this->GetFarPlane();
	scene.append_child("FieldOfView").append_attribute("Value") = this->GetFOV();
	scene.append_child("AspectRatio").append_attribute("Value") = this->GetAspectRatio();

	scene = scene.parent();
}

void ComponentCamera::OnLoad(pugi::xml_node &scene)
{
	scene = scene.child("Camera");

	SetNearPlane(scene.child("NearPlane").attribute("Value").as_float());
	SetFarPlane(scene.child("FarPlane").attribute("Value").as_float());
	SetFOV(scene.child("FieldOfView").attribute("Value").as_float());
	SetAspectRatio(scene.child("AspectRatio").attribute("Value").as_float());

	scene = scene.parent();
}

void ComponentCamera::LookAt(const vec &position)
{
	vec dir = position - camera_frustum.Pos();

	//Look at doc: http://clb.demon.fi/MathGeoLib/nightly/docs/float3x3_LookAt.php
	float3x3 lookat_matrix = float3x3::LookAt(camera_frustum.Front(), dir.Normalized(), camera_frustum.Up(), vec::unitY);
	camera_frustum.SetFront(lookat_matrix.MulDir(camera_frustum.Front()).Normalized()); //Get front dir from matrix
	camera_frustum.SetUp(lookat_matrix.MulDir(camera_frustum.Up()).Normalized()); //Get up dir from matrix
}

//Gets ------------------------------------------------------------------------------
const float* ComponentCamera::GetViewMatrix() const
{
	float4x4 tmp = camera_frustum.ViewMatrix();
	tmp.Transpose();

	return *tmp.v;
}

const float* ComponentCamera::GetProjectionMatrix() const
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

const Frustum ComponentCamera::GetFrustum() const
{
	return camera_frustum;
}

//Sets-------------------------------------------------------------------------------------
void ComponentCamera::SetNearPlane(float new_distance)
{
	if (new_distance != camera_frustum.NearPlaneDistance() && new_distance > 0 && new_distance < camera_frustum.FarPlaneDistance())
	{
		camera_frustum.SetViewPlaneDistances(new_distance, camera_frustum.FarPlaneDistance());
	}		
}

void ComponentCamera::SetFarPlane(float new_distance)
{
	if (new_distance != camera_frustum.FarPlaneDistance() && new_distance > 0 && new_distance > camera_frustum.NearPlaneDistance())
	{
		camera_frustum.SetViewPlaneDistances(camera_frustum.NearPlaneDistance(), new_distance);
	}		
}

void ComponentCamera::SetFOV(float new_fov)
{
	//Should add some limits?
	camera_frustum.SetVerticalFovAndAspectRatio((new_fov * DEGTORAD), camera_frustum.AspectRatio());
}

void ComponentCamera::SetAspectRatio(float new_aspect_ratio)
{
	//Should add some limits?
	camera_frustum.SetHorizontalFovAndAspectRatio(camera_frustum.HorizontalFov(), new_aspect_ratio);
}

void ComponentCamera::SetPos(vec new_position)
{
	camera_frustum.SetPos(new_position);
}

//Could add a SetFrame to optimize setpos setfront and setup
void ComponentCamera::SetPos(float4x4* transform)
{
	vec new_pos = vec::zero;
	new_pos = transform->TranslatePart();
	camera_frustum.SetPos(new_pos);
}

void ComponentCamera::SetFront(float4x4* transform)
{
	camera_frustum.SetFront(transform->WorldZ());
}

void ComponentCamera::SetUp(float4x4* transform)
{
	camera_frustum.SetUp(transform->WorldY());
}

void ComponentCamera::SetFrame(float4x4* transform)
{
	camera_frustum.SetFrame(transform->TranslatePart(), transform->WorldZ(), transform->WorldY());
}

void ComponentCamera::SetFrame(vec new_pos, vec front_dir, vec up_dir)
{
	camera_frustum.SetFrame(new_pos, front_dir, up_dir);
}

//Resource used: http://www.flipcode.com/archives/Frustum_Culling.shtml 
bool ComponentCamera::Intersects(const AABB &aabb)
{	
	return camera_frustum.Intersects(aabb);
}
