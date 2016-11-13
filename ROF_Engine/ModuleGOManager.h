#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"
#include <vector>

class GameObject;
class ComponentCamera;
class OctTree;

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

	void DoOctTree();
	void ShowAABB(bool showing);

	void CameraCulling();
	void OctTreeCulling();
	void AddCameraCulling(ComponentCamera* cam, bool culling);

private:
	GameObject* root = nullptr;
	std::vector<GameObject*> gos_array;

	OctTree* go_tree = nullptr;

public:
	bool show_tree = false;
	std::vector<ComponentCamera*> camera_cullings;

private:
	void SetParent(GameObject* me, GameObject* new_parent);
};

#endif // __MODULEGOMANAGER_H__
