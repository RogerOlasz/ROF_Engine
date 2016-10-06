#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "MathGeoLib/include/MathGeoLib.h"
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

	void Draw();

	void SetNewParent(GameObject* new_parent);

private:
	bool active = true;
	GameObject* parent = nullptr;

public:
	std::string name;
	std::list<GameObject*> childs;
	std::list<Component*> components;

};

#endif // !__GAMEOBJECT_H__

