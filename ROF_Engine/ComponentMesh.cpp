#include "ComponentMesh.h"
#include "Application.h"
#include "Mesh.h"
#include "ModuleRenderer3D.h"

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

void ComponentMesh::Draw()
{
	App->renderer3D->DrawMesh(mesh);
}


