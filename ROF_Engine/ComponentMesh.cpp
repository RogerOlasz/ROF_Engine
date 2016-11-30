#include "ComponentMesh.h"
#include "Application.h"
#include "Mesh.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"

#include "MeshImporter.h"
#include "ModuleSceneImporter.h"

ComponentMesh::ComponentMesh(GameObject* bearer, int id) : Component(bearer, Type::Geometry, id)
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

void ComponentMesh::OnSave(pugi::xml_node &scene)
{
	scene = scene.append_child("Mesh");
	scene.append_child("Type").append_attribute("Value") = this->GetType();
	scene.append_child("Path").text().set(path.c_str());
	scene = scene.parent();
}

void ComponentMesh::OnLoad(pugi::xml_node &scene)
{
	//TODO
	pugi::xml_node tmp = scene.child("Mesh");
	pugi::xml_node tmp2 = tmp.child("Path");
	LoadMesh(App->importer->mesh_importer->Load(scene.child("Mesh").child("Path").text().get()), scene.child("Mesh").child_value("Path"));
}

void ComponentMesh::LoadMesh(Mesh* recived_mesh, const char* path)
{
	mesh = recived_mesh;
	if (path)
	{
		this->path = path;
	}

	//Setting bounding box
	bounding_box.SetNegativeInfinity(); //Must be called before Enclose() to ser box at null
	bounding_box.Enclose(mesh->vertices, mesh->num_vertices);
}

void ComponentMesh::Update()
{
	if (active)
	{
		game_object->render_c->c_mesh = this;
	}
	else
	{
		game_object->render_c->c_mesh = nullptr;
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

const char* ComponentMesh::GetPath() const
{
	return path.c_str();
}

