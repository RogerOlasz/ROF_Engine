#include "ModuleSceneImporter.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "ModuleFileSystem.h"
#include "ModuleInput.h"
#include "ModuleResourceManager.h"

#include "GameObject.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

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

	mesh_loader = new MeshLoader();
	tex_loader = new TextureLoader();
	mat_loader = new MaterialLoader();

	return ret;
}

update_status ModuleSceneImporter::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleSceneImporter::CleanUp()
{
	aiDetachAllLogStreams();

	RELEASE(mesh_loader);
	RELEASE(tex_loader);
	RELEASE(mat_loader);

	return true;
}

void ModuleSceneImporter::LoadFBX(const char* file_path, bool file_system)
{
	const aiScene* scene = nullptr;

	std::string file_path_string = file_path;
	std::size_t tmp_pos = file_path_string.rfind("Assets");
	file_path_string = file_path_string.substr(tmp_pos);

	if (App->res_manager->SearchForOriginFile(file_path_string.c_str()))
	{
		fbx_is_loaded = true;
	}

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
		LoadGameObjectFromFBX(file_path, scene->mRootNode, scene, App->go_manager->GetRootNode());

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

void ModuleSceneImporter::LoadGameObjectFromFBX(const char* file_path, const aiNode* node_to_load, const aiScene* scene, GameObject* parent)
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

#pragma region SetMeshAndMaterial
		for (uint i = 0; i < node_to_load->mNumMeshes; ++i)
		{
			std::string file_path_string = file_path;
			std::size_t tmp_pos = file_path_string.rfind("Assets");
			file_path_string = file_path_string.substr(tmp_pos);

			if (fbx_is_loaded == false)
			{
				ResourceMesh* r_mesh = App->res_manager->ImportMeshResource(scene->mMeshes[node_to_load->mMeshes[i]], file_path, node_to_load->mName.C_Str());
				if (r_mesh)
				{
					ResourceMaterial* r_mat = App->res_manager->ImportMaterialResource(scene->mMaterials[scene->mMeshes[node_to_load->mMeshes[i]]->mMaterialIndex], file_path_string.c_str(), node_to_load->mName.C_Str());

					ret->CreateComponent(Component::Type::Geometry)->SetResource(r_mesh);
					ret->CreateComponent(Component::Type::Material)->SetResource(r_mat);

					App->res_manager->LoadResource(r_mesh->GetID(), Resource::ResType::Mesh);
					App->res_manager->LoadResource(r_mat->texture->GetID(), Resource::ResType::Texture);
				}
			}
			else
			{
				ComponentMesh* cmesh = (ComponentMesh*)ret->CreateComponent(Component::Type::Geometry);
				cmesh->SetResource(App->res_manager->SearchResource(file_path_string.c_str(), node_to_load->mName.C_Str(), Resource::ResType::Mesh));

				ComponentMaterial* cmat = (ComponentMaterial*)ret->CreateComponent(Component::Type::Material);
				cmat->SetResource(App->res_manager->SearchResource(file_path_string.c_str(), node_to_load->mName.C_Str(), Resource::ResType::Material));

				App->res_manager->LoadResource(cmesh->GetResource()->GetID(), Resource::ResType::Mesh);
				App->res_manager->LoadResource(cmat->GetResource()->GetID(), Resource::ResType::Material);
			}
		}
#pragma endregion
	}

	//Loading children nodes (do this in recursive to load all tree node)
	for (int j = 0; j < node_to_load->mNumChildren; j++)
	{
		LoadGameObjectFromFBX(file_path, node_to_load->mChildren[j], scene, ret);
	}

	if (ret != nullptr)
	{
		LOG("I'm %s and i have %d children.", ret->GetName(), ret->children.size());
		LOG("I'm %s and i have %d components.", ret->GetName(), ret->components.size());
		LOG("I'm %s and my parent is %s", ret->GetName(), ret->GetParent()->GetName());
	}
}