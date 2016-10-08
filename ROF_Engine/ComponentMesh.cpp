#include "ComponentMesh.h"
#include "Mesh.h"
#include "ModuleGeometry.h"

ComponentMesh::ComponentMesh(GameObject* bearer) : Component(bearer, Types::Geometry)
{
	
}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::LoadMesh(Mesh* recived_mesh)
{
	mesh = recived_mesh;
}