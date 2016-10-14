#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Component.h"
#include "Globals.h"
#include <list>
#include <vector>

class GameObject
{
public:
	GameObject(const char* name);
	virtual ~GameObject();

	Component* CreateComponent(Component::Types type);
	void RemoveComponent(Component* to_delete);

	bool IsActive() const;
	void SwitchActive(bool active);
	void EnableComponent();
	void DisableComponent();

	void Update();

	GameObject* GetParent();
	void SwitchParent(GameObject* new_parent);

	const char* GetName();
	std::string* GetNameString();
	void SetName(const char* new_name);
	bool GetHierarchyState();

	bool Remove();

private:
	bool active = true;
	GameObject* parent = nullptr;
	std::string name;

	bool is_on_hierarchy = true;

public:
	bool to_delete = false;
	//TODO vectors pls
	std::list<GameObject*> children;
	std::list<Component*> components;

};

#endif // !__GAMEOBJECT_H__

