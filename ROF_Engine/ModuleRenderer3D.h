#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Light.h"
#include <vector>

#define MAX_LIGHTS 8

struct Mesh;
class ComponentCamera;

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

	void AddMeshToDraw(const Mesh* mesh, bool wireframed, const float4x4* trans_matrix);
	void LoadMeshBuffers(const Mesh* mesh);
	void RemoveMeshBuffers(Mesh* mesh);
	void DrawMesh(const Mesh* mesh, bool wireframe = false);

private:
	std::vector<const Mesh*> mesh_to_draw;
	std::vector<bool> is_wireframe;
	std::vector<const float4x4*> mesh_trans_matrix;

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;

	ComponentCamera* camera = nullptr;
	bool update_proj_matrix = true;
	bool camera_changed = false;
};

#endif // !__MODULERENDERER3D_H__