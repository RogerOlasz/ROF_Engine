#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Resource.h"

#include "ResourceMesh.h"
#include "ModuleSceneImporter.h"
#include "ModuleResourceManager.h"

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
	//TODO communicate to res manager to remove this component from using a resource
	resource = nullptr;
}

void ComponentMesh::OnSave(pugi::xml_node &scene)
{
	scene = scene.append_child("Mesh");

	scene.append_child("Type").append_attribute("Value") = this->GetType();

	scene.append_child("ResourceID").append_attribute("Value") = resource->GetID();
	scene = scene.parent();
}

void ComponentMesh::OnLoad(pugi::xml_node &scene)
{
	resource = App->res_manager->LoadResource(scene.child("Mesh").child("ResourceID").attribute("Value").as_ullong(), Resource::ResType::Mesh);
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

const char* ComponentMesh::GetPath() const
{
	return path.c_str();
}

AABB* ComponentMesh::GetBoundingBox() const
{
	return &((ResourceMesh*)resource)->bounding_box;
}
