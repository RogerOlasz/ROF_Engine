#include "ModuleResourceManager.h"
#include "Application.h"
#include "Globals.h"

#include "MeshLoader.h"
#include "ResourceMesh.h"

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("ResourceManager");
}

ModuleResourceManager::~ModuleResourceManager()
{

}

bool ModuleResourceManager::Init()
{
	mesh_loader = new MeshLoader();

	return true;
}

bool ModuleResourceManager::CleanUp()
{
	std::map<Uint32, Resource*>::iterator it = resources_on_memory.begin(); 
	while(it != resources_on_memory.end())
	{
		it->second->UnloadFromMemory();
		RELEASE(it->second);
		it = resources_on_memory.erase(it);
	}

	RELEASE(mesh_loader);

	return true;
}

Resource* ModuleResourceManager::GetResource(Uint32 ID, Resource::ResType type)
{
	Resource* ret = nullptr;

	std::map<Uint32, Resource*>::iterator it = resources_on_memory.find(ID);
	if (it != resources_on_memory.end())
	{
		ret = it->second;
	}
	else
	{
		switch (type)
		{
			case (Resource::ResType::Mesh) :
			{
				ret = mesh_loader->MeshLoad(ID);
				break;
			}
			case (Resource::ResType::Texture) :
			{
				break;
			}
			case (Resource::ResType::Material) :
			{
				break;
			}
			case (Resource::ResType::Prefab) :
			{
				break;
			}
		}

		if (ret != nullptr)
		{
			ret->LoadOnMemory();
		}
	}

	return ret;
}

ResourceMesh* ModuleResourceManager::ImportMeshResource(const aiMesh* ai_mesh, const char* origin_file, const char* resource_name)
{
	ResourceMesh* r_mesh = nullptr;

	r_mesh = (ResourceMesh*)SearchResource(origin_file, resource_name, Resource::ResType::Mesh);
	if (r_mesh != nullptr)
	{
		return r_mesh;
	}

	//If doesn't exist, import it
	r_mesh = mesh_loader->MeshImport(ai_mesh, last_id++, origin_file, resource_name);
	if (r_mesh)
	{
		imported_resources[r_mesh->ID] = CreateResourceData(r_mesh);
	}

	return r_mesh;
}

Resource* ModuleResourceManager::SearchResource(const char* origin_file, const char* resource_name, Resource::ResType type)
{
	for (std::map<Uint32, ResourceData>::iterator tmp = imported_resources.begin(); tmp != imported_resources.end(); tmp++)
	{
		if (tmp->second.CompareResource(origin_file, resource_name))
		{
			return GetResource(tmp->first, type);
		}
	}

	return nullptr;
}

ResourceData ModuleResourceManager::CreateResourceData(Resource* resource)
{
	ResourceData ret;

	ret.origin_file = resource->origin_file;
	ret.resource_name = resource->name;
	ret.type = resource->type;

	return ret;
}