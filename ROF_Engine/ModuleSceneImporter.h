#ifndef __MODULESCENEIMPORTER_H__
#define __MODULESCENEIMPORTER_H__

#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL.h"

struct aiNode;
struct aiScene;
class GameObject;
class MeshImporter;

class ModuleSceneImporter : public Module
{
public:
	ModuleSceneImporter(Application* app, bool start_enabled = true);
	~ModuleSceneImporter();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void LoadFBX(const char* file_path, bool file_system = true);
	void LoadGameObjectFromFBX(const aiNode* hierarchy_to_load, const aiScene* scene, GameObject* parent = nullptr);

private:
	MeshImporter* mesh_importer = nullptr;

	//http://clb.demon.fi/MathGeoLib/nightly/docs/LCG_summary.php Awesome joke...
	LCG random;
	Uint32 UID;

};

#endif // !__MODULESCENEIMPORTER_H__