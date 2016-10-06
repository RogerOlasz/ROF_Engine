#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "Globals.h"
#include <list>
#include <vector>

class Component;

class GameObject
{
public:
	GameObject(GameObject* parent, const char* name);
	GameObject(GameObject* parent, const char* name, const float3& translation, const float3& scale, const Quat& rotation);
	virtual ~GameObject();

	//Component* CreateComponent(Component::Types type);
	void RemoveComponent(Component* to_delete);

	bool IsActive() const;
	void SwitchActive(bool active);

	void Draw();

private:
	bool active = true;
	GameObject* parent = nullptr;

public:
	std::string name;
	std::vector<GameObject*> childs;
	std::list<Component*> components;

};


#endif // !__GAMEOBJECT_H__

