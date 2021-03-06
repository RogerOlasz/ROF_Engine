#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "RenderComp.h"
#include "Light.h"
#include <vector>

#define MAX_LIGHTS 8

class ResourceMesh;
class ResourceMaterial;
class ResourceTexture;
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

	ComponentCamera* GetRenderingCamera();

	void AddGOToRender(RenderComp* go_to_render);
	void DeleteGOToRender(RenderComp* go_to_delete);
	void CleanToRender();
	void DrawMesh(const ResourceMesh* mesh, ResourceMaterial* material, ResourceTexture* texture, bool wireframe = false);

private:
	std::vector<RenderComp*> to_render;

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;

	ComponentCamera* camera = nullptr;
	bool update_proj_matrix = true;
	bool camera_changed = false;
};

#endif // !__MODULERENDERER3D_H__