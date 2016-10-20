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
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("GOManager");
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
		LoadFBX("Assets/Models/Street environment_V01.fbx");
	}

	root->Update();

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

void ModuleGOManager::DrawDebug()
{

}

// Called before quitting
bool ModuleGOManager::CleanUp()
{
	aiDetachAllLogStreams();

	std::vector<GameObject*>::reverse_iterator tmp = gos_array.rbegin();
	while (tmp != gos_array.rend())
	{
		RELEASE((*tmp));
		tmp++;
	}
	gos_array.clear();

	if (root)
	{
		RELEASE(root);
	}

	return true;
}

GameObject* ModuleGOManager::GetRootNode() const
{
	return root;
}

GameObject* ModuleGOManager::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* new_go = new GameObject(name);
	SetParent(new_go, parent);

	gos_array.push_back(new_go);

	return new_go;
}

void ModuleGOManager::LoadGameObjectMesh(const aiNode* node_to_load, const aiScene* scene, GameObject* parent)
{

	GameObject* ret = nullptr;

	//Creating GameObject from aiNode
	if (node_to_load->mParent)
	{
		ret = CreateGameObject(node_to_load->mName.C_Str(), parent);
	}

	if (ret != nullptr)
	{
#pragma region SetTransform
		//Setting transformation
		aiVector3D position;
		aiQuaternion rotation;
		aiVector3D scale;

		node_to_load->mTransformation.Decompose(scale, rotation, position);

		vec pos(position.x, position.y, position.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
		vec sca(scale.x, scale.y, scale.z);

		//Add transforms from assimp dummies nodes to the right node
		while (ret->GetNameString()->find("$AssimpFbx$") != std::string::npos)
		{
			node_to_load = node_to_load->mChildren[0];

			node_to_load->mTransformation.Decompose(scale, rotation, position);

			pos += vec(position.x, position.y, position.z);
			rot = rot * Quat(rotation.x, rotation.y, rotation.z, rotation.w);
			sca = vec(scale.x * sca.x, scale.y * sca.y, scale.z * sca.z);

			ret->SetName(node_to_load->mName.C_Str());
		}

		ret->transform->SetPos(pos.x, pos.y, pos.z);
		ret->transform->SetScale(sca.x, sca.y, sca.z);
		ret->transform->SetRot(rot.x, rot.y, rot.z, rot.w);
#pragma endregion

#pragma region SetMaterial
		ComponentMaterial* material = (ComponentMaterial*)ret->CreateComponent(Component::Types::Material);
#pragma endregion

#pragma region SetMeshe
		for (uint i = 0; i < node_to_load->mNumMeshes; ++i)
		{
			Mesh* tmp = App->geometry->LoadGeometry(scene->mMeshes[node_to_load->mMeshes[i]], scene, material);

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

void ModuleGOManager::LoadFBX(const char* file_path, bool file_system)
{
	const aiScene* scene;

	if (file_system)
	{
		scene = aiImportFileEx(file_path, aiProcessPreset_TargetRealtime_MaxQuality, App->physfs->GetAssimpIO());
	}
	else
	{
		scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	}

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

