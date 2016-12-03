#ifndef __MOUDLERESOURCEMANAGER_H__
#define __MOUDLERESOURCEMANAGER_H__

#include "Module.h"
#include "Resource.h"
#include <map>

class ResourceMesh;

struct aiMesh;

class ModuleResourceManager : public Module
{
public: 
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Init();
	bool CleanUp();

	//Load resource if it is not loaded on memory
	Resource* LoadResource(Uint32 ID, Resource::ResType type);
	Resource* CreateAndLoad(Uint32 ID, Resource::ResType type);

	void SaveResourcesData();
	void LoadResourcesData();

	//Should have an unique import for all res types?
	ResourceMesh* ImportMeshResource(const aiMesh* ai_mesh, const char* origin_file, const char* resource_name);
	bool CompareResource(Resource* res, const char* o_file, const char* r_name);
	bool CompareResource(Resource* res, Resource::ResType type);

private:
	Resource* SearchResource(const char* origin_file, const char* resource_name, Resource::ResType type);
	Resource* SearchResource(Uint32 ID);

private:
	std::map<Uint32, Resource*> resources;

public:
	Uint32 next_id = 0;
};

#endif // !__MOUDLERESOURCEMANAGER_H__
