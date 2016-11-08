#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"
#include <vector>

#define MAX_STR_LEN 1024

class GameObject;
class ComponentCamera;
class OctTree;
struct aiNode;
struct aiScene;

class ModuleGOManager : public Module
{
public:

	ModuleGOManager(Application* app, bool start_enabled = true);
	~ModuleGOManager();

	bool Init();
	bool CleanUp();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	GameObject* GetRootNode() const;
	GameObject* CreateGameObject(const char* name, GameObject* parent);
	
	void LoadFBX(const char* file_path, bool file_system = true);
	void LoadGameObjectFromFBX(const aiNode* hierarchy_to_load, const aiScene* scene, GameObject* parent = nullptr);

	void DoOctTree();
	void ShowAABB(bool showing);

	void CameraCulling();
	void OctTreeCulling();

private:
	GameObject* root = nullptr;
	std::vector<GameObject*> gos_array;

	OctTree* go_tree = nullptr;

public:
	bool show_tree = false;

private:
	void SetParent(GameObject* me, GameObject* new_parent);
};

#endif // __MODULEGOMANAGER_H__
