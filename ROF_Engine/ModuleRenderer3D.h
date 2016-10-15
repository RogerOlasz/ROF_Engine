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

	//Texture debug mode
	uint image_texture;
	void CreateDebugTexture();
	//--------------------------------

	void OnResize(int width, int height);

	bool LoadMeshBuffer(const Mesh* mesh);
	void RemoveMeshBuffers(Mesh* mesh);
	void DrawMesh(const Mesh* mesh, bool wireframe = false);
	void DrawMeshWireframe(const Mesh* mesh);

public:
	//Save buffer ids into an array to do a faster cleanup
	std::vector<unsigned int> id_vertices_r;
	std::vector<unsigned int> id_indices_r;
	std::vector<unsigned int> id_normals_r;
	std::vector<unsigned int> id_tex_coords_r;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};

#endif // !__MODULEPHYSICS3D_H__