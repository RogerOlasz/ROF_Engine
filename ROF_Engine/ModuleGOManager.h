#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class GameObject;
class ComponentCamera;
class OctTree;

class ModuleGOManager : public Module
{
public:

	ModuleGOManager(Application* app, bool start_enabled = true);
	~ModuleGOManager();

	bool Start();
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

	void FindCandidates(const LineSegment &to_test, std::vector<GameObject*> &candidates);

private:
	GameObject* root = nullptr;
	std::vector<GameObject*> gos_array;

	OctTree* go_tree = nullptr;

	bool want_to_save_scene;
	bool want_to_load_scene;

	std::string load_scene;
	std::string save_scene;

	LCG random;

public:
	bool show_tree = false;
	std::vector<ComponentCamera*> camera_cullings;

private:
	void SetParent(GameObject* me, GameObject* new_parent);

	void LoadScene(const char* file);
	void SaveScene(const char* file);

	bool LoadSceneNow();
	bool SaveSceneNow();
};

#endif // __MODULEGOMANAGER_H__
