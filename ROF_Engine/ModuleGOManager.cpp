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
	root = new GameObject("Root");

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
		//LoadFBX("Assets/Models/Tank.fbx");
		//LoadFBX("Assets/Models/SimpleHierarchy.fbx");
		LoadFBX("Assets/Models/City.fbx");
		//LoadFBX("Assets/Models/SimpleH2.fbx");
		//LoadFBX("Assets/Models/SimpleH3.fbx");
		//LoadFBX("Assets/Models/SimpleH4.fbx");
		//LoadFBX("Assets/Models/SimpleH5.fbx");
	}

	root->Draw();

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
	GameObject* new_go = new GameObject(name);
	
	SetParent(new_go, parent);

	debug_go_counter++;

	return new_go;
}

void ModuleGOManager::RemoveGameObjects(GameObject* go_to_delete)
{
	go_to_delete->to_delete = true;
}

void ModuleGOManager::LoadGameObjectMesh(const aiNode* node_to_load, const aiScene* scene, GameObject* parent)
{
	//Setting node names
	//MAXLEN stores 1024u
	char tmp_name[MAXLEN];
	memcpy(tmp_name, node_to_load->mName.data, node_to_load->mName.length + 1);

	GameObject* ret = nullptr;

	if (node_to_load->mParent)
	{
		ret = CreateGameObject(tmp_name, parent);
	}

	if (ret != nullptr)
	{
#pragma region SetTransforms
		//Setting transformation
		ComponentTransformation* trans = (ComponentTransformation*)ret->CreateComponent(Component::Types::Transformation);

		aiVector3D position;
		aiQuaternion rotation;
		aiVector3D scale;

		node_to_load->mTransformation.Decompose(scale, rotation, position);

		trans->SetPos(position.x, position.y, position.z);
		trans->SetScale(scale.x, scale.y, scale.z);
		trans->SetRot(rotation.x, rotation.y, rotation.z, rotation.w);
#pragma endregion

#pragma region SetMeshes
		for (uint i = 0; i < node_to_load->mNumMeshes; ++i)
		{
			Mesh* tmp = App->geometry->LoadGeometry(scene->mMeshes[node_to_load->mMeshes[i]]);
						
			((ComponentMesh*)ret->CreateComponent(Component::Types::Geometry))->LoadMesh(tmp);
		}
#pragma endregion
	}

	//Loading children nodes (do this in recursive to load all tree node)
	for (int j = 0; j < node_to_load->mNumChildren; j++)
	{
		LoadGameObjectMesh(node_to_load->mChildren[j], scene, ret);
	}

	if (ret != nullptr)
	{
		LOG("I'm %s and i have %d children.", ret->GetName(), ret->children.size());
		LOG("I'm %s and i have %d components.", ret->GetName(), ret->components.size());
		LOG("I'm %s and my parent is %s", ret->GetName(), ret->GetParent()->GetName());
	}
}

void ModuleGOManager::LoadFBX(const char* file_path)
{
	const aiScene* scene = aiImportFileEx(file_path, aiProcessPreset_TargetRealtime_MaxQuality, App->physfs->GetAssimpIO());

	if (scene != nullptr && scene->HasMeshes())
	{
		LoadGameObjectMesh(scene->mRootNode, scene, root);

		if (scene)
		{
			aiReleaseImport(scene);
		}
	}
	else
	{
		LOG("[error] Error loading scene %s %s", file_path, aiGetErrorString());
	}

	LOG("[warning] Now i have %d GameObjects...", debug_go_counter);
}

void ModuleGOManager::SetParent(GameObject* me, GameObject* new_parent)
{
	if (new_parent == nullptr)
	{
		me->SwitchParent(root);
	}
	else
	{
		me->SwitchParent(new_parent);
	}
}

