#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Light.h"

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

	//Texture debug mode
	uint image_texture;
	void CreateDebugTexture();
	//--------------------------------

	void OnResize(int width, int height);

	bool LoadMeshBuffer(const Mesh* mesh);
	void DrawMesh(const Mesh* mesh);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};