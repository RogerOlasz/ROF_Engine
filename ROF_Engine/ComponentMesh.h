#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"

struct Mesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* bearer, int id);
	~ComponentMesh();

	void LoadMesh(Mesh* recived_mesh);
	void Draw();

	Mesh* GetMesh();

private:
	Mesh* mesh;

};

#endif // !__COMPONENTMESH_H__