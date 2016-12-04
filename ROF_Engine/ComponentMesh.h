#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <string>

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* bearer, int id);
	~ComponentMesh();

	void Update();

	void OnSave(pugi::xml_node&);
	void OnLoad(pugi::xml_node&);

	void UpdateResourceInfo() override;

	AABB* GetBoundingBox() const;

public:
	bool wirefr = false;
	std::string wire;
	std::string is_active;
};

#endif // !__COMPONENTMESH_H__