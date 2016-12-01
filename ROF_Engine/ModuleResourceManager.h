#ifndef __MOUDLERESOURCEMANAGER_H__
#define __MOUDLERESOURCEMANAGER_H__

#include "Module.h"
#include "Resource.h"
#include <string>
#include <map>

class ResourceMesh;

class MeshLoader;

struct aiMesh;
struct aiMaterial;

struct ResourceData
{
	Resource::ResType type;
	std::string resource_name;
	std::string resource_file;
	std::string origin_file;	

	bool CompareResource(const char* file, const char* name)
	{
		return (origin_file == file && resource_name == name);
	}
};

class ModuleResourceManager : public Module
{
public: 
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Init();
	bool CleanUp();

	//Load resource if it is not loaded on memory
	Resource* LoadResource(Uint32 ID, Resource::ResType type);

	//Should have an unique import for all res types?
	//UID ImportFile(const char* new_file_in_assets, bool force = false);
	ResourceMesh* ImportMeshResource(const aiMesh* ai_mesh, const char* origin_file, const char* resource_name);

private:
	//Uint32 Find(const char* file_in_assets) const;
	Resource* SearchResource(const char* origin_file, const char* resource_name, Resource::ResType type);
	ResourceData CreateResourceData(Resource* resource);
	const ResourceData* GetResourceData(const char* origin_file, const char* resource_name, Resource::ResType type);
	const ResourceData* GetResourceData(Uint32 ID);

private:
	std::map<Uint32, Resource*> resources_on_memory;
	std::map<Uint32, ResourceData> imported_resources;

	//Must take it from sceneimporter
	MeshLoader* mesh_loader = nullptr;

	Uint32 last_id = 0;
};

#endif // !__MOUDLERESOURCEMANAGER_H__
