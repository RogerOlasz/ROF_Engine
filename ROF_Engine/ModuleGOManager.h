#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"

#define MAX_STR_LEN 1024

class GameObject;
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

	GameObject* CreateGameObject(const char* name, GameObject* parent);
	void LoadGameObjectMesh(const aiNode* hierarchy_to_load, const aiScene* scene, GameObject* parent = nullptr);
	void RemoveGameObjects(GameObject* go_to_delete);

	void LoadFBX(const char* file_path);

private:
	GameObject* root;
	
	void SetParent(GameObject* me, GameObject* new_parent);

	uint debug_go_counter = 0;
};

#endif // __MODULEGOMANAGER_H__
