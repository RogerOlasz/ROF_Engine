#ifndef __MODULERENDERER3D_H__
#define __MODULEPHYSICS3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Light.h"
#include <vector>

#define MAX_LIGHTS 8

struct Mesh;
class ComponentCamera;
class ComponentMaterial;
class QuadTreee;

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

	//Debug texture mode
	uint image_texture;
	void CreateDebugTexture();

	void OnResize(int width, int height);

	void LoadMeshBuffers(const Mesh* mesh);
	void RemoveMeshBuffers(Mesh* mesh);
	void DrawMesh(const Mesh* mesh, bool wireframe = false);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;

	ComponentCamera* camera = nullptr;
	bool update_proj_matrix = true;
};

#endif // !__MODULEPHYSICS3D_H__