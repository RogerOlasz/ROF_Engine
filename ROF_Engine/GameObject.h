#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class ComponentTransformation;

class GameObject
{
public:
	GameObject(const char* name);
	virtual ~GameObject();

	Component* CreateComponent(Component::Types type);
	void RemoveComponent(Component* to_delete);

	bool IsActive() const;
	void SwitchActive(bool active);

	void Update();

	GameObject* GetParent();
	void SwitchParent(GameObject* new_parent);

	const char* GetName();
	std::string* GetNameString();
	void SetName(const char* new_name);
	bool GetHierarchyState();

	void UpdateGlobalMatrix();

	const Component* GetComponentByType(Component::Types type);
	bool RemoveGameObject(GameObject* to_delete);

private:
	bool active = true;
	GameObject* parent = nullptr;
	std::string name;

	AABB bounding_box;
	OBB o_bb;

	bool is_on_hierarchy = true;

public:
	ComponentTransformation* transform = nullptr;

	std::vector<GameObject*> children;
	std::vector<Component*> components;

};

#endif // !__GAMEOBJECT_H__

