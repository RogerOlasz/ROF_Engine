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
class MeshImporter;
class MaterialImporter;

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
	void LoadTexture(ComponentMaterial* material, aiMaterial* ai_material);
	void LoadTextureBuffer(const char* path, uint &buffer_id);

	//TODO
public:
	MeshImporter* mesh_importer = nullptr;
	MaterialImporter* material_importer = nullptr;

	//http://clb.demon.fi/MathGeoLib/nightly/docs/LCG_summary.php Awesome joke...
	LCG random;
	Uint32 UUID;

};

#endif // !__MODULESCENEIMPORTER_H__