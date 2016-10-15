#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include <string>

struct Mesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* bearer, int id);
	~ComponentMesh();

	void LoadMesh(Mesh* recived_mesh);
	void Draw();

	void CleanUp();

	Mesh* GetMesh();

public:
	bool wirefr = false;
	std::string wire;
	std::string is_active;

private:
	Mesh* mesh;

};

#endif // !__COMPONENTMESH_H__