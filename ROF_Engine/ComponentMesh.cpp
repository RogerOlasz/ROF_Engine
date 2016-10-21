#include "ComponentMesh.h"
#include "Application.h"
#include "Mesh.h"
#include "ModuleRenderer3D.h"

ComponentMesh::ComponentMesh(GameObject* bearer, int id) : Component(bearer, Types::Geometry, id)
{
	//Component names are to solve problems with ImGui same names
	active = true;
	char tmp[SHORT_STRING];
	sprintf(tmp, "Mesh##%d", id);

	char tmp_2[SHORT_STRING];
	sprintf(tmp_2, "Wireframe##%d", id);

	char tmp_3[SHORT_STRING];
	sprintf(tmp_3, "##Active%d", id);

	is_active = tmp_3;
	wire = tmp_2;
	name = tmp;
}

ComponentMesh::~ComponentMesh()
{
	
}

void ComponentMesh::CleanUp()
{
	App->renderer3D->RemoveMeshBuffers(mesh);
	RELEASE(mesh);
}

void ComponentMesh::LoadMesh(Mesh* recived_mesh)
{
	mesh = recived_mesh;

	//Setting bounding box
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose(mesh->vertices, mesh->num_vertices);
}

void ComponentMesh::Update()
{
	if (active)
	{
		App->renderer3D->DrawMesh(mesh, wirefr);
	}
}

Mesh* ComponentMesh::GetMesh()
{
	return mesh;
}

const AABB* ComponentMesh::GetBoundingBox() const
{
	return &bounding_box;
}

