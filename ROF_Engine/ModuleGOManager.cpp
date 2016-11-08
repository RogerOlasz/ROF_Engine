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

#include "OctTree.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("GOManager");

	root = new GameObject("Root");
}

ModuleGOManager::~ModuleGOManager()
{

}

// Called before render is available
bool ModuleGOManager::Init()
{

	return true;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	CameraCulling();

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		LoadFBX("Assets/Models/Street environment_V01.fbx");
	}

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		DoOctTree();
	}

	root->Update();

	std::vector<GameObject*>::iterator tmp = gos_array.begin();
	while (tmp != gos_array.end())
	{
		(*tmp)->Update();
		
		tmp++;
	}

	if (go_tree != nullptr && show_tree == true)
	{
		std::vector<OctTreeNode*>::iterator tmp_t = go_tree->tree_struct.begin();
		while (tmp_t != go_tree->tree_struct.end())
		{
			(*tmp_t)->DebugUpdate();

			tmp_t++;
		}
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
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

void ModuleGOManager::LoadGameObjectFromFBX(const aiNode* node_to_load, const aiScene* scene, GameObject* parent)
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
		//Still being bad, ComponentMaterial just loads and save textures (id) but i'm charging textures from struct Mesh
		ComponentMaterial* material = (ComponentMaterial*)ret->CreateComponent(Component::Type::Material);
#pragma endregion

#pragma region SetMesh
		for (uint i = 0; i < node_to_load->mNumMeshes; ++i)
		{
			Mesh* tmp = App->geometry->LoadGeometry(scene->mMeshes[node_to_load->mMeshes[i]], scene, material);

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

void ModuleGOManager::DoOctTree() 
{
	if (go_tree == nullptr)
	{
		go_tree = new OctTree(AABB(vec(-50, -50, -50), vec(50, 50, 50)));

		std::vector<GameObject*>::iterator tmp = gos_array.begin();
		while (tmp != gos_array.end())
		{
			go_tree->Insert((*tmp));
			tmp++;
		}
	}
	else
	{
		go_tree->Clear();
		go_tree = nullptr;
	}	
}

void ModuleGOManager::ShowAABB(bool showing)
{
	std::vector<GameObject*>::iterator tmp = gos_array.begin();
	while (tmp != gos_array.end())
	{
		(*tmp)->aabb_debug = showing;
		tmp++;
	}
}

void ModuleGOManager::CameraCulling()
{
	//It works with preferences so if there are more than one camera the first one added will determine the culling
	//TODO please, delete it... most dirty method to do camera culling...
	std::vector<GameObject*>::iterator tmp = gos_array.begin();
	ComponentCamera* tmp_cam = nullptr;
	while (tmp != gos_array.end())
	{
		std::vector<GameObject*>::iterator last_tmp = tmp;
		if ((*tmp)->have_camera)
		{
			if (((ComponentCamera*)(*tmp)->GetComponentByType(Component::Type::Camera))->frustum_culling)
			{
				tmp_cam = (ComponentCamera*)(*tmp)->GetComponentByType(Component::Type::Camera);

				tmp = gos_array.begin();
				while (tmp != gos_array.end())
				{
					if (tmp_cam->Intersects(*(*tmp)->GetBoundingBox()) == false)
					{
						if ((ComponentMesh*)(*tmp)->GetComponentByType(Component::Type::Geometry))
						{
							((ComponentMesh*)(*tmp)->GetComponentByType(Component::Type::Geometry))->active = false;
						}
					}
					else
					{
						if ((ComponentMesh*)(*tmp)->GetComponentByType(Component::Type::Geometry))
						{
							((ComponentMesh*)(*tmp)->GetComponentByType(Component::Type::Geometry))->active = true;
						}
					}
					tmp++;
				}
				break;
			}
			else
			{
				tmp = gos_array.begin();
				while (tmp != gos_array.end())
				{
					if ((ComponentMesh*)(*tmp)->GetComponentByType(Component::Type::Geometry))
					{
						((ComponentMesh*)(*tmp)->GetComponentByType(Component::Type::Geometry))->active = true;
					}
					tmp++;
				}
				tmp = last_tmp;
			}
		}
		tmp++;
	}
}

void ModuleGOManager::LoadFBX(const char* file_path, bool file_system)
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
		LoadGameObjectFromFBX(scene->mRootNode, scene, root);

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

