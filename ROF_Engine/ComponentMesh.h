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

	void LoadMesh(Mesh* recived_mesh);
	void Update();

	void CleanUp();

	Mesh* GetMesh();
	const AABB* GetBoundingBox() const;

public:
	bool wirefr = false;
	std::string wire;
	std::string is_active;

private:
	Mesh* mesh;
	// Linear bounding box / Take a look: http://clb.demon.fi/MathGeoLib/nightly/docs/AABB_summary.php
	AABB bounding_box;

};

#endif // !__COMPONENTMESH_H__