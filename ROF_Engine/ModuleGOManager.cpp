#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "ModuleGeometry.h"
#include "ModuleFileSystem.h"
#include "ModuleInput.h"

#include "Mesh.h"
#include "GameObject.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGOManager::~ModuleGOManager()
{

}

// Called before render is available
bool ModuleGOManager::Init()
{
	root = new GameObject("Root", nullptr);

	return true;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		//LoadFBX("Assets/Models/LamboMurcielago.fbx");
		//LoadFBX("Assets/Models/HierarchyScene.fbx");
		//LoadFBX("Assets/Models/Tank.fbx");
		LoadFBX("Assets/Models/HierarchyScene.fbx");
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::PostUpdate(float dt)
{
	//Here i have to delete all checked GOs with to_delete

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGOManager::CleanUp()
{
	return true;
}

GameObject* ModuleGOManager::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* new_go = new GameObject(name, parent);
	
	if (parent == nullptr)
	{
		parent = root;
	}

	return new_go;
}

void ModuleGOManager::RemoveGameObjects(GameObject* go_to_delete)
{
	go_to_delete->to_delete = true;
}

GameObject* ModuleGOManager::LoadGameObjectMesh(const aiNode* node_to_load, const aiScene* scene, GameObject* parent)
{
	//Setting name of node
	char tmp_name[LONG_STRING];
	memcpy(tmp_name, node_to_load->mName.data, node_to_load->mName.length + 1);

	GameObject* ret = CreateGameObject(tmp_name, parent);

	ComponentTransformation* trans = (ComponentTransformation*)ret->CreateComponent(Component::Types::Transformation);

	//Setting transformation
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	node_to_load->mTransformation.Decompose(scale, rotation, position);

	trans->SetPos(position.x, position.y, position.z);
	trans->SetScale(scale.x, scale.y, scale.z);
	trans->SetRot(rotation.x, rotation.y, rotation.z, rotation.w);
		
	for (uint i = 0; i < node_to_load->mNumMeshes; ++i)
	{
		Mesh* tmp = App->geometry->LoadGeometry(scene->mMeshes[node_to_load->mMeshes[i]]);
		ret->CreateComponent(Component::Types::Geometry);
		if (ret->components.back()->GetType() == Component::Types::Geometry)
		{
			((ComponentMesh*)ret->components.back())->LoadMesh(tmp);
		}
	}

	//Loading children nodes (do this in recursive to load all node tree)
	for (int j = 0; j < node_to_load->mNumChildren; j++)
	{
		ret->children.push_back(LoadGameObjectMesh(node_to_load->mChildren[j], scene, ret));
	}

	return ret;
}

GameObject* ModuleGOManager::LoadFBX(const char* file_path)
{
	GameObject* ret = nullptr;

	const aiScene* scene = aiImportFileEx(file_path, aiProcessPreset_TargetRealtime_MaxQuality, App->physfs->GetAssimpIO());

	if (scene != nullptr && scene->HasMeshes())
	{
		ret = LoadGameObjectMesh(scene->mRootNode, scene, root);

		if (scene)
		{
			aiReleaseImport(scene);
		}
	}
	else
	{
		LOG("[error] Error loading scene %s %s", file_path, aiGetErrorString());
	}

	return ret;
}

