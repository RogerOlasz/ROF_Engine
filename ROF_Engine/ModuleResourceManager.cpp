#include "ModuleResourceManager.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneImporter.h"
#include "Globals.h"
#include "XMLUtilities.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceMaterial.h"

#include "MeshLoader.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("ResourceManager");
}

ModuleResourceManager::~ModuleResourceManager()
{

}

bool ModuleResourceManager::Init()
{
	LoadResourcesData();

	return true;
}

bool ModuleResourceManager::CleanUp()
{
	SaveResourcesData();

	std::map<Uint32, Resource*>::iterator it = resources.begin(); 
	while(it != resources.end())
	{
		if (it->second->IsOnMemory())
		{
			it->second->UnloadFromMemory();
		}
		RELEASE(it->second);
		it = resources.erase(it);
	}

	return true;
}

Resource* ModuleResourceManager::LoadResource(Uint32 ID, Resource::ResType type)
{
	Resource* ret = nullptr;

	std::map<Uint32, Resource*>::iterator it = resources.find(ID);
	if (it != resources.end())
	{
		ret = it->second;
		if (!it->second->IsOnMemory())
		{			
			ret->LoadOnMemory();
		}	
		it->second->on_use++;
		if (type == Resource::ResType::Material)
		{
			if (((ResourceMaterial*)it->second)->resource_texture_id != 0)
			{
				((ResourceMaterial*)it->second)->texture = (ResourceTexture*)GetResource(((ResourceMaterial*)it->second)->resource_texture_id);
				((ResourceMaterial*)it->second)->texture->on_use++;
				if (!((ResourceMaterial*)it->second)->texture->IsOnMemory())
				{
					((ResourceMaterial*)it->second)->texture->LoadOnMemory();
				}
			}
		}
	}
	else
	{
		LOG("Resource must have been imported before load on memory.");
	}
	
	return ret;
}

Resource* ModuleResourceManager::CreateAndLoad(Uint32 ID, Resource::ResType type)
{
	Resource* ret = nullptr;

	std::map<Uint32, Resource*>::iterator it = resources.find(ID);
	if (it != resources.end())
	{
		LOG("Resource is already imported.");

		ret = it->second;
	}
	else
	{
		switch (type)
		{
			case (Resource::ResType::Mesh) :
			{
				ret = App->importer->mesh_loader->MeshLoad(ID);
				break;
			}
			case (Resource::ResType::Texture) :
			{
				ret = App->importer->tex_loader->TextureLoad(ID);
				break;
			}
			case (Resource::ResType::Material) :
			{
				ret = App->importer->mat_loader->MaterialLoad(ID);
				break;
			}
		}

		if (ret != nullptr)
		{
			resources[ID] = ret;
		}
	}

	return ret;
}

void ModuleResourceManager::SaveResourcesData()
{
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("Resources");

	std::map<Uint32, Resource*>::iterator it = resources.begin(); 
	while (it != resources.end())
	{
		root = root.append_child("Resource");
		root.append_child("ID").append_attribute("Value") = it->second->ID;
		root.append_child("Name").text().set(it->second->name.c_str());
		root.append_child("Type").append_attribute("Value") = it->second->GetType();
		root.append_child("OriginalFile").text().set(it->second->origin_file.c_str());
		root.append_child("ResourceFile").text().set(it->second->resource_file.c_str());
		root = root.parent();
		it++;
	}

	root.append_child("NextID").append_attribute("Value") = next_id;

	std::stringstream stream;
	data.save(stream);
	App->physfs->Save("Library/Resources.xml", stream.str().c_str(), stream.str().length());

}

void ModuleResourceManager::LoadResourcesData()
{
	char* buffer;
	uint size = App->physfs->Load("Library/Resources.xml", &buffer);

	if (size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE_ARRAY(buffer);

		if (result != 0)
		{
			root = data.child("Resources");
			for (pugi::xml_node node = root.child("Resource"); node != nullptr; node = node.next_sibling("Resource"))
			{
				//Create resources with recived data
				Resource* tmp = CreateAndLoad(node.child("ID").attribute("Value").as_ullong(), (Resource::ResType)node.child("Type").attribute("Value").as_int());
				tmp->name = node.child("Name").text().get();
				tmp->origin_file = node.child("OriginalFile").text().get();
			}
			next_id = root.child("NextID").attribute("Value").as_ullong();
		}
	}
}

ResourceMesh* ModuleResourceManager::ImportMeshResource(const aiMesh* ai_mesh, const char* origin_file, const char* resource_name)
{
	ResourceMesh* r_mesh = nullptr;

	r_mesh = (ResourceMesh*)SearchResource(origin_file, resource_name, Resource::ResType::Mesh);
	if (r_mesh != nullptr)
	{
		return r_mesh;
	}

	//If doesn't exist, import it -> it should be a method from scene importer
	r_mesh = App->importer->mesh_loader->MeshImport(ai_mesh, next_id++, origin_file, resource_name);
	if (r_mesh)
	{
		resources[r_mesh->ID] = r_mesh;
	}

	return r_mesh;
}

ResourceMaterial* ModuleResourceManager::ImportMaterialResource(const aiMaterial* ai_material, const char* origin_file, const char* resource_name)
{
	ResourceMaterial* r_mat = nullptr;

	r_mat = (ResourceMaterial*)SearchResource(origin_file, resource_name, Resource::ResType::Material);
	if (r_mat != nullptr)
	{
		return r_mat;
	}

	//If doesn't exist, import it -> it should be a method from scene importer
	r_mat = App->importer->mat_loader->MaterialImport(ai_material, next_id++, origin_file, resource_name);
	if (r_mat)
	{
		resources[r_mat->ID] = r_mat;
	}

	return r_mat;
}

ResourceTexture* ModuleResourceManager::ImportTextureResource(const aiMaterial* ai_material, const char* origin_file, const char* resource_name)
{
	ResourceTexture* r_tex = nullptr;

	r_tex = (ResourceTexture*)SearchResource(origin_file, resource_name, Resource::ResType::Texture);
	if (r_tex != nullptr)
	{
		return r_tex;
	}

	//If doesn't exist, import it -> it should be a method from scene importer
	r_tex = App->importer->tex_loader->TextureImport(ai_material, next_id++, resource_name);
	if (r_tex)
	{
		resources[r_tex->ID] = r_tex;
	}

	return r_tex;
}

bool ModuleResourceManager::CompareResource(Resource* res, const char* o_file, const char* r_name)
{
	return (res->origin_file == o_file && res->name == r_name);
}

bool ModuleResourceManager::CompareResource(Resource* res, Resource::ResType type)
{
	return (res->GetType() == type);
}

Resource* ModuleResourceManager::GetResource(Uint32 ID)
{
	std::map<Uint32, Resource*>::iterator tmp = resources.find(ID);
	if (tmp != resources.end())
	{
		return tmp->second;
	}

	return nullptr;
}

bool ModuleResourceManager::SearchForOriginFile(const char* origin_file)
{
	for (std::map<Uint32, Resource*>::iterator tmp = resources.begin(); tmp != resources.end(); tmp++)
	{
		if (tmp->second->origin_file == origin_file)
		{
			return true;
		}
	}
	return false;
}

Resource* ModuleResourceManager::SearchResource(const char* origin_file, const char* resource_name, Resource::ResType type)
{
	for (std::map<Uint32, Resource*>::iterator tmp = resources.begin(); tmp != resources.end(); tmp++)
	{
		if (CompareResource(tmp->second, origin_file, resource_name) && CompareResource(tmp->second, type))
		{
			return tmp->second;
		}
	}

	return nullptr;
}
