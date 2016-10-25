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

	Component* CreateComponent(Component::Type type);
	void RemoveComponent(Component* to_delete);

	bool IsActive() const;
	void SwitchActive(bool active);

	void Update();

	GameObject* GetParent();
	void SwitchParent(GameObject* new_parent);

	const char* GetName();
	std::string* GetNameString();
	const Component* GetComponentByType(Component::Type type);
	const AABB GetBoundingBox() const;

	void SetName(const char* new_name);

	void UpdateGlobalMatrix();
	void UpdateAABB();

	bool RemoveGameObject(GameObject* to_delete);

private:
	bool active = false;
	GameObject* parent = nullptr;
	std::string name;

	AABB bounding_box;
	OBB go_obb;	

public:
	ComponentTransformation* transform = nullptr;

	bool obb_debug = false;
	bool aabb_debug = false;
	bool static_go = false;

	std::vector<GameObject*> children;
	std::vector<Component*> components;

};

#endif // !__GAMEOBJECT_H__

