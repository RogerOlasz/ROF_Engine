#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Component.h"
#include "Globals.h"
#include <list>
#include <vector>

class GameObject
{
public:
	GameObject(const char* name, GameObject* parent);
	virtual ~GameObject();

	Component* CreateComponent(Component::Types type);
	void RemoveComponent(Component* to_delete);

	bool IsActive() const;
	void SwitchActive(bool active);
	void EnableComponent();
	void DisableComponent();

	void Draw();

	GameObject* GetParent();
	void SetParent(GameObject* new_parent);

	const char* GetName();

	bool Remove();

private:
	bool active = true;
	GameObject* parent = nullptr;
	std::string name;

public:
	bool to_delete = false;
	std::list<GameObject*> children;
	std::list<Component*> components;

};

#endif // !__GAMEOBJECT_H__

