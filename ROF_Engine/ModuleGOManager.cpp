#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "OctTree.h"

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
	LoadScene("SceneSerialitzation.xml");
	LoadSceneNow();

	return true;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	if (go_tree)
	{		
		OctTreeCulling();
	}
	else
	{
		CameraCulling();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::Update(float dt)
{
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
	SaveScene("SceneSerialitzation.xml");
	SaveSceneNow();

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
	new_go->UUID = random.Int();
	App->renderer3D->AddGOToRender(new_go->render_c);

	gos_array.push_back(new_go);

	return new_go;
}

void ModuleGOManager::DoOctTree() 
{
	if (go_tree == nullptr)
	{
		go_tree = new OctTree(AABB(vec(-50, -50, -50), vec(50, 50, 50)));

		std::vector<GameObject*>::iterator tmp = gos_array.begin();
		while (tmp != gos_array.end())
		{
			if ((*tmp)->static_go)
			{
				go_tree->Insert((*tmp));
			}			
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

void ModuleGOManager::OctTreeCulling()
{
	std::vector<GameObject*>::iterator tmp = gos_array.begin();
	ComponentCamera* tmp_cam = nullptr;
	while (tmp != gos_array.end())
	{
		std::vector<GameObject*>::iterator last_tmp = tmp;
		if ((*tmp)->have_camera)
		{
			if (((ComponentCamera*)(*tmp)->GetComponentByType(Component::Type::Camera))->frustum_culling)
			{
				//TODO must change all to false before start with octtree selection
				tmp_cam = (ComponentCamera*)(*tmp)->GetComponentByType(Component::Type::Camera);
				
				std::vector<GameObject*> candidates;
				go_tree->CollectCandidates(candidates, tmp_cam->GetFrustum());

				std::vector<GameObject*>::iterator tmp_cand = candidates.begin();
				//LOG("Candidates size: %d", candidates.size());
				while (tmp_cand != candidates.end())
				{
					if (tmp_cam->Intersects(*(*tmp_cand)->GetBoundingBox()) == false)
					{
						if ((ComponentMesh*)(*tmp_cand)->GetComponentByType(Component::Type::Geometry))
						{
							((ComponentMesh*)(*tmp_cand)->GetComponentByType(Component::Type::Geometry))->active = false;
						}
					}
					else
					{
						if ((ComponentMesh*)(*tmp_cand)->GetComponentByType(Component::Type::Geometry))
						{
							((ComponentMesh*)(*tmp_cand)->GetComponentByType(Component::Type::Geometry))->active = true;
						}
					}
					tmp_cand++;
				}
				break; //To works just with the upper hierarchy camera
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

void ModuleGOManager::AddCameraCulling(ComponentCamera* cam, bool culling)
{
	if (culling)
	{
		camera_cullings.push_back(cam);
	}
	else
	{
		for (std::vector<ComponentCamera*>::iterator it = camera_cullings.begin(); it != camera_cullings.end(); it++)
		{
			if (cam == (*it))
			{
				camera_cullings.erase(it);
				break;
			}			
		}
	}
}

void ModuleGOManager::FindCandidates(const LineSegment &to_test, std::vector<GameObject*> &candidates)
{
	std::vector<GameObject*>::iterator tmp = gos_array.begin();
	while (tmp != gos_array.end())
	{
		if (to_test.Intersects(*(*tmp)->GetBoundingBox()))
		{
			candidates.push_back((*tmp));
		}
		tmp++;
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

void ModuleGOManager::LoadScene(const char* file)
{
	char tmp[SHORT_STRING];
	sprintf(tmp, "%s%s", App->physfs->GetSaveDirectory(), file);
	//load_scene.assign(tmp);
	load_scene.assign(file);
}

void ModuleGOManager::SaveScene(const char* file)
{
	save_scene.assign(file);
}

bool ModuleGOManager::LoadSceneNow()
{
	bool ret = true;

	char* buffer;
	uint size = App->physfs->Load(load_scene.c_str(), &buffer);

	if (size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if (result != 0)
		{
			root = data.child("GameObjects");
		}
	}

	return ret;
}

bool ModuleGOManager::SaveSceneNow()
{
	bool ret = true;

	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("GameObjects");

	std::vector<GameObject*>::iterator item = gos_array.begin();
	while (item != gos_array.end() && ret == true)
	{
		ret = (*item)->Save(root.append_child((*item)->GetName()));

		item++;
	}

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		App->physfs->Save(load_scene.c_str(), stream.str().c_str(), stream.str().length());
	}

	return ret;
}

