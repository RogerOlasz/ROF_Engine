#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"

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
	GameObject* LoadGameObjectMesh(const aiNode* hierarchy_to_load, const aiScene* scene, GameObject* parent = nullptr);
	void RemoveGameObjects(GameObject* go_to_delete);

	GameObject* LoadFBX(const char* file_path);

private:
	GameObject* root;
	
};

#endif // __MODULEGOMANAGER_H__
