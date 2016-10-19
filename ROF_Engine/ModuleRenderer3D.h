#ifndef __MODULERENDERER3D_H__
#define __MODULEPHYSICS3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Light.h"
#include <vector>

struct Mesh;

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawDebug();

	//Texture debug mode
	uint image_texture;
	void CreateDebugTexture();

	void OnResize(int width, int height);

	bool LoadMeshBuffer(const Mesh* mesh);
	void RemoveMeshBuffers(Mesh* mesh);
	void DrawMesh(const Mesh* mesh, bool wireframe = false);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	float4x4 transform = float4x4::identity;
	//Understanding frustrum attributes
	Frustum camera_frustum;
};

#endif // !__MODULEPHYSICS3D_H__