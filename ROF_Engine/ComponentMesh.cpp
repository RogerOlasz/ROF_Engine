#include "ComponentMesh.h"
#include "Application.h"
#include "Mesh.h"
#include "ModuleRenderer3D.h"

ComponentMesh::ComponentMesh(GameObject* bearer, int id) : Component(bearer, Types::Geometry, id)
{
	char tmp[SHORT_STRING];
	sprintf(tmp, "Mesh##%i", id);
	name = tmp;
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

Mesh* ComponentMesh::GetMesh()
{
	return mesh;
}


