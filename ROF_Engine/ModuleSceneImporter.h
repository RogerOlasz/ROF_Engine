#ifndef __MODULESCENEIMPORTER_H__
#define __MODULESCENEIMPORTER_H__

#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL.h"

struct aiNode;
struct aiMaterial;
struct aiScene;
class GameObject;
class ComponentMaterial;
class MeshLoader;
class TextureLoader;
class MaterialLoader;

class ModuleSceneImporter : public Module
{
public:
	ModuleSceneImporter(Application* app, bool start_enabled = true);
	~ModuleSceneImporter();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void LoadFBX(const char* file_path, bool file_system = true);
	void LoadGameObjectFromFBX(const char* file_path, const aiNode* hierarchy_to_load, const aiScene* scene, GameObject* parent = nullptr);

public:
	MeshLoader* mesh_loader = nullptr;
	TextureLoader* tex_loader = nullptr;
	MaterialLoader* mat_loader = nullptr;

	bool fbx_is_loaded = false;
};

#endif // !__MODULESCENEIMPORTER_H__