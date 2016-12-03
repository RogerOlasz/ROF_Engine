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
#include "MeshLoader.h"

#include "MaterialImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

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

	//Initialize DevIL 
	ilInit();
	iluInit();
	ilutInit();

	mesh_loader = new MeshLoader();
	material_importer = new MaterialImporter();
	UUID = 0;

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
	RELEASE(material_importer);

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

#pragma region SetMaterial
		ComponentMaterial* material = (ComponentMaterial*)ret->CreateComponent(Component::Type::Material);		
#pragma endregion

#pragma region SetMesh
		for (uint i = 0; i < node_to_load->mNumMeshes; ++i)
		{
			ResourceMesh* r_mesh = App->res_manager->ImportMeshResource(scene->mMeshes[node_to_load->mMeshes[i]], file_path, node_to_load->mName.C_Str()); // scene->mMeshes[node_to_load->mMeshes[i]]->mName.C_Str()
			if (r_mesh)
			{
				LoadTexture(material, scene->mMaterials[scene->mMeshes[node_to_load->mMeshes[i]]->mMaterialIndex]);
				ret->CreateComponent(Component::Type::Geometry)->SetResource(r_mesh);
				r_mesh->LoadOnMemory();
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

void ModuleSceneImporter::LoadTexture(ComponentMaterial* material, aiMaterial* ai_material)
{
	uint tex_num = ai_material->GetTextureCount(aiTextureType_DIFFUSE);

	aiColor4D mat_color;
	ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, mat_color);
	material->SetMaterialColor(mat_color.r, mat_color.g, mat_color.b, mat_color.a);

	aiString path;
	ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	material->AppendTexturePath("Assets/Textures/");
	std::string tmp = material->GetTexturePath();
	material->AppendTexturePath(App->physfs->GetFileNameFromDirPath(path.data).c_str());

	std::string tmp_s = "";

	if (tex_num != 0)
	{		
		if (App->physfs->Exists(material->GetTexturePath()))
		{
			//Adapt devIL to OpenGL buffer
			ilutRenderer(ILUT_OPENGL);
			ilGenImages(1, (ILuint*)material->GetTextureId());
			ilBindImage(material->GetTextureId());
			ilLoadImage(material->GetTexturePath());
			
			material->SetTextureId(ilutGLBindTexImage());
		}			
	}
	else
	{
		material->SetTextureId(0);
		LOG("[error] aiMaterial couldn't be load.");
	}
	
	/*char tmp_c[LONG_STRING];
	UUID = random.Int();
	sprintf(tmp_c, "Library/Textures/texture%d.rof", UUID);
	tmp_s = tmp_c;
	material->path = tmp_s;

	material_importer->Import(App->physfs->GetFileNameFromDirPath(path.data).c_str(), tmp.c_str(), material->GetMaterialColor(), tmp_s);*/
}

void ModuleSceneImporter::LoadTextureBuffer(const char* path, uint &buffer_id)
{
	if (App->physfs->Exists(path))
	{
		//Adapt devIL to OpenGL buffer
		ilutRenderer(ILUT_OPENGL);
		ilGenImages(1, (ILuint*)buffer_id);
		ilBindImage(buffer_id);
		ilLoadImage(path);

		buffer_id = ilutGLBindTexImage();
	}
}