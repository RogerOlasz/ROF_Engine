#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleGOManager.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneImporter.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"

#include "Panel.h"
#include "PanelConsole.h"
#include "PanelConfiguration.h"
#include "PanelHierarchy.h"
#include "PanelComponents.h"
#include "PanelTimeControl.h"

#include <algorithm>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"

using namespace std;

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Editor");
	selected_file[0] = '\0';
}

// Destructor
ModuleEditor::~ModuleEditor()
{}

// Called before render is available
bool ModuleEditor::Init()
{
	ImGui_ImplSdlGL3_Init(App->window->window);

	panels.push_back(Config = new PanelConfiguration);
	panels.push_back(Console = new PanelConsole);
	panels.push_back(Hierarchy = new PanelHierarchy);
	panels.push_back(Comp = new PanelComponents);
	panels.push_back(TimeControl = new PanelTimeControl);

	camera_id = 0;

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	
	//TODO so temporal, is horrible have this here...
	char tmp[SHORT_STRING];
	sprintf(tmp, "Camera%d", camera_id);
	camera_name = tmp;

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	//Creating an ImGui UI
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ESC"))
			{
				return UPDATE_STOP;
			}
			if (ImGui::MenuItem("Load Scene...", nullptr, &file_explorer))
			{
				loading_scene = true;
				frame_name = "Load File";
			}
			if (ImGui::MenuItem("Save Scene...", nullptr, &file_explorer))
			{
				saving_scene = true;
				frame_name = "Save File";
			}
			
		 ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Create Camera"))
			{
				GameObject* tmp_go;
				tmp_go = App->go_manager->CreateGameObject(camera_name.c_str(), nullptr);
				tmp_go->CreateComponent(Component::Type::Camera);
				camera_id++;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Configuration", "C", &Config->active));
			if (ImGui::MenuItem("Console", "CTR+C", &Console->active));
			if (ImGui::MenuItem("Hierarchy", "CTR+O", &Hierarchy->active));
			if (ImGui::MenuItem("Time Controller", "CTR+T", &TimeControl->active)); 

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug Tools"))
		{
			if (ImGui::MenuItem("Show AABB", "B", &aabb_debug))
			{
				App->go_manager->ShowAABB(aabb_debug);
			}

			if (ImGui::MenuItem("Show GO OctTree", "T", &octtree_debug))
			{
				App->go_manager->show_tree = octtree_debug;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Download latest"))
			{
				App->RequestBrowser("https://github.com/RogerOlasz/ROF_Engine/releases");
			}

			if (ImGui::MenuItem("Report a bug"))
			{
				App->RequestBrowser("https://github.com/RogerOlasz/ROF_Engine/issues");
			}

			if (ImGui::MenuItem("Source code"))
			{
				App->RequestBrowser("https://github.com/RogerOlasz/ROF_Engine");
			}

			ImGui::EndMenu();
		}

	 ImGui::EndMainMenuBar();
	}	

	//ROF Discomment it to edit ImGuiC Colors
	//ImGui::ShowStyleEditor();

	//Draw all active panels
	Panel* panel;
	for (vector<Panel*>::iterator tmp = panels.begin(); tmp != panels.end(); ++tmp)
	{
		panel = (*tmp);

		if (panel->IsActive())
		{
			panel->Draw();
		}
	}
	Comp->Draw(Hierarchy->GetSelectedGO());

	if (file_explorer == true && App->editor->FileDialog(nullptr, "Assets/"))
	{
		const char* file = App->editor->CloseFileDialog();
		if (file != nullptr && loading_scene)
		{
			App->go_manager->LoadScene(file);
			App->go_manager->want_to_load_scene = true;
		}

		if (file != nullptr && saving_scene)
		{
			std::string tmp = file;
			tmp.append(".xml");
			App->go_manager->SaveScene(tmp.c_str());
			App->go_manager->want_to_save_scene = true;
		}

		loading_scene = false;
		saving_scene = false;
		file_explorer = false;
	}

	if (file_dialog == opened)
	{
		LoadFile((file_dialog_filter.length() > 0) ? file_dialog_filter.c_str() : nullptr);
	}
	else
	{
		in_modal = false;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleEditor::Load(pugi::xml_node &config)
{
	SetMaxFPS(config.child("Framerate").attribute("Cap_FPS").as_uint(60));

	return true;
}

bool ModuleEditor::Save(pugi::xml_node &config) const
{
	pugi::xml_node tmp_node = config.append_child("Framerate");

	tmp_node.append_attribute("Cap_FPS") = GetMaxFPS();

	return true;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{	
	for (vector<Panel*>::iterator tmp = panels.begin(); tmp != panels.end(); ++tmp)
	{
		RELEASE(*tmp);
	}
	panels.clear();

	ImGui_ImplSdlGL3_Shutdown();

	return true;
}

void ModuleEditor::Log(const char* log)
{
	if (Console != NULL)
	{
		Console->AddLog(log);
	}	
}

void ModuleEditor::SetMaxFPS(uint new_fps_cap)
{
	Config->SetMaxFPS(new_fps_cap);
}

void ModuleEditor::LogFPS(const float* fps, const float ms)
{
	Config->Log(fps, ms);
}

uint ModuleEditor::GetMaxFPS() const
{
	return Config->GetMaxFPS();
}

void ModuleEditor::SetSelectedGO(GameObject* go)
{
	Hierarchy->SetSelectedGO(go);
}

void ModuleEditor::LoadFile(const char* filter_extension, const char* from_dir)
{
	ImGui::OpenPopup(frame_name.c_str());
	if (ImGui::BeginPopupModal(frame_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		in_modal = true;

		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::BeginChild("File Browser", ImVec2(0, 300), true);

		DrawDirectoryRecursive(from_dir, filter_extension);

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::PushItemWidth(250.f);
		if (ImGui::InputText("##file_selector", selected_file, MEDIUM_STRING, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			file_dialog = ready_to_close;
		}

		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("Ok", ImVec2(50, 20)))
		{
			file_dialog = ready_to_close;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(50, 20)))
		{
			file_dialog = ready_to_close;
			selected_file[0] = '\0';
		}

		ImGui::EndPopup();
	}
	else
	{
		in_modal = false;
	}
}

void ModuleEditor::DrawDirectoryRecursive(const char* directory, const char* filter_extension)
{
	vector<string> files;
	vector<string> dirs;

	std::string dir((directory) ? directory : "");
	dir += "/";

	App->physfs->DiscoverFiles(dir.c_str(), files, dirs);

	for (vector<string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		if (ImGui::TreeNodeEx((dir + (*it)).c_str(), 0, "%s/", (*it).c_str()))
		{
			DrawDirectoryRecursive((dir + (*it)).c_str(), filter_extension);
			ImGui::TreePop();
		}
	}

	std::sort(files.begin(), files.end());

	for (vector<string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		const string& str = *it;

		bool ok = true;

		if (filter_extension && str.substr(str.find_last_of(".") + 1) != filter_extension)
		{
			ok = false;
		}

		if (ok && ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked()) 
			{
				sprintf_s(selected_file, MEDIUM_STRING, "%s%s", dir.substr(1).c_str(), str.c_str()); //Changed dir

				if (ImGui::IsMouseDoubleClicked(0))
				{
					file_dialog = ready_to_close;
				}
			}

			ImGui::TreePop();
		}
	}
}

bool ModuleEditor::FileDialog(const char * extension, const char* from_folder)
{
	bool ret = true;

	App->camera->controls_disabled = true;

	switch (file_dialog)
	{
	case closed:
		selected_file[0] = '\0';
		file_dialog_filter = (extension) ? extension : "";
		file_dialog_origin = (from_folder) ? from_folder : "";
		file_dialog = opened;
	case opened:
		ret = false;
		break;
	}

	return ret;
}

const char * ModuleEditor::CloseFileDialog()
{
	App->camera->controls_disabled = false;

	if (file_dialog == ready_to_close)
	{
		file_dialog = closed;
		return selected_file[0] ? selected_file : nullptr;
	}
	return nullptr;
}