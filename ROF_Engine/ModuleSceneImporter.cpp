#include "ModuleSceneImporter.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "ModuleGeometry.h"
#include "ModuleFileSystem.h"
#include "ModuleInput.h"

#include "GameObject.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "MeshImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

ModuleSceneImporter::ModuleSceneImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("SceneImporter");
}

// Destructor
ModuleSceneImporter::~ModuleSceneImporter()
{}

// Called before render is available
bool ModuleSceneImporter::Init()
{
	bool ret = true;

	mesh_importer = new MeshImporter();
	UUID = 0;

	return ret;
}

update_status ModuleSceneImporter::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		LoadFBX("Assets/Models/Street environment_V01.fbx");
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleSceneImporter::CleanUp()
{
	aiDetachAllLogStreams();

	return true;
}

void ModuleSceneImporter::LoadFBX(const char* file_path, bool file_system)
{
	const aiScene* scene = nullptr;

	if (file_system)
	{
		scene = aiImportFileEx(file_path, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	}
	else
	{
		scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	}

	if (scene != nullptr && scene->HasMeshes())
	{
		LoadGameObjectFromFBX(scene->mRootNode, scene, App->go_manager->GetRootNode());

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

void ModuleSceneImporter::LoadGameObjectFromFBX(const aiNode* node_to_load, const aiScene* scene, GameObject* parent)
{
	GameObject* ret = nullptr;

	//Creating GameObject from aiNode
	if (node_to_load->mParent)
	{
		ret = App->go_manager->CreateGameObject(node_to_load->mName.C_Str(), parent);
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
		//TODO: Still being bad, ComponentMaterial just loads and save textures (id) but i'm charging textures from struct Mesh
		ComponentMaterial* material = (ComponentMaterial*)ret->CreateComponent(Component::Type::Material);
#pragma endregion

#pragma region SetMesh
		for (uint i = 0; i < node_to_load->mNumMeshes; ++i)
		{
			Mesh* tmp = App->geometry->LoadGeometry(scene->mMeshes[node_to_load->mMeshes[i]], scene, material);

			/*char tmp_c[LONG_STRING];
			UUID = random.Int();
			sprintf(tmp_c, "mesh%d.rof", UUID);
			std::string tmp_s = tmp_c;
			mesh_importer->Import(scene->mMeshes[node_to_load->mMeshes[i]], tmp_s);*/

			if (tmp != nullptr)
			{
				((ComponentMesh*)ret->CreateComponent(Component::Type::Geometry))->LoadMesh(tmp);
			}
		}
#pragma endregion
	}

	//Loading children nodes (do this in recursive to load all tree node)
	for (int j = 0; j < node_to_load->mNumChildren; j++)
	{
		LoadGameObjectFromFBX(node_to_load->mChildren[j], scene, ret);
	}

	if (ret != nullptr)
	{
		LOG("I'm %s and i have %d children.", ret->GetName(), ret->children.size());
		LOG("I'm %s and i have %d components.", ret->GetName(), ret->components.size());
		LOG("I'm %s and my parent is %s", ret->GetName(), ret->GetParent()->GetName());
	}
}