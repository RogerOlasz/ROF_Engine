#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneImporter.h"
#include "ModuleResourceManager.h"

#include "Resource.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"

ComponentMaterial::ComponentMaterial(GameObject* bearer, int id) : Component(bearer, Type::Material, id)
{
	//Component names are to solve problems with ImGui same names
	char tmp[SHORT_STRING];
	sprintf(tmp, "Material##%d", id);
	name = tmp;
}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::OnSave(pugi::xml_node &scene)
{
	scene = scene.append_child("Material");

	scene.append_child("Type").append_attribute("Value") = this->GetType();

	scene.append_child("ResourceID").append_attribute("Value") = resource->GetID();

	scene = scene.parent();
}

void ComponentMaterial::OnLoad(pugi::xml_node &scene)
{
	resource = App->res_manager->LoadResource(scene.child("Material").child("ResourceID").attribute("Value").as_ullong(), Resource::ResType::Material);
	if (((ResourceMaterial*)resource)->have_texture)
	{
		((ResourceMaterial*)resource)->texture = (ResourceTexture*)App->res_manager->GetResource(((ResourceMaterial*)resource)->resource_texture_id);
		if (!((ResourceMaterial*)resource)->texture->IsOnMemory())
		{
			((ResourceMaterial*)resource)->texture->LoadOnMemory();
		}
	}
}