#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "PugiXML/src/pugixml.hpp"
#include "SDL/include/SDL.h"
#include <vector>

class ComponentTransformation;

class GameObject
{
public:
	GameObject(const char* name);
	~GameObject();

	Component* CreateComponent(Component::Type type);
	void RemoveComponent(Component* to_delete);

	void Update();

	GameObject* GetParent();
	void SwitchParent(GameObject* new_parent);

	const char* GetName();
	std::string* GetNameString();
	const Component* GetComponentByType(Component::Type type);
	const AABB* GetBoundingBox() const;
	const float4x4* GetGlobalMatrixT() const;

	void SetName(const char* new_name);

	void UpdateGlobalMatrix();
	void UpdateAABB();

	void SetChildrenStatic(bool static_go);
	bool RemoveGameObject(GameObject* to_delete);

	bool Save(pugi::xml_node&);

private:
	GameObject* parent = nullptr;
	std::string name;

	AABB bounding_box;
	OBB go_obb;		

public:
	ComponentTransformation* transform = nullptr;

	bool obb_debug = false;
	bool aabb_debug = false;
	bool static_go = false;
	bool static_children = false;

	bool have_camera = false;

	std::vector<GameObject*> children;
	std::vector<Component*> components;
	
	Uint32 UUID = 0;
};

#endif // !__GAMEOBJECT_H__

