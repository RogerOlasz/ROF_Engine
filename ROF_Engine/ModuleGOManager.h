#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"

class GameObject;

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

	//GameObject* GetParent();
	GameObject* CreateGameObject(const char* name, GameObject* parent);
	void RemoveGameObject(GameObject* to_delete);
	void ChangeParent(GameObject* to_move, GameObject* new_parnet); 

private:
	GameObject* root;
	
};

#endif // __MODULEGOMANAGER_H__
