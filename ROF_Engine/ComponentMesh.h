#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <string>

struct Mesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* bearer, int id);
	~ComponentMesh();

	void LoadMesh(Mesh* recived_mesh, const char* path = nullptr);
	void Update();
	void CleanUp();

	void OnSave(pugi::xml_node&);
	void OnLoad(pugi::xml_node&);

	Mesh* GetMesh();
	const AABB* GetBoundingBox() const;
	const char* GetPath() const;

public:
	bool wirefr = false;
	std::string wire;
	std::string is_active;

public:
	Mesh* mesh;
	// Linear bounding box / Take a look: http://clb.demon.fi/MathGeoLib/nightly/docs/AABB_summary.php
	AABB bounding_box;
	std::string path;
};

#endif // !__COMPONENTMESH_H__