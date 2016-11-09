#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleGOManager.h"
#include "GameObject.h"

#include "Panel.h"
#include "PanelConsole.h"
#include "PanelConfiguration.h"
#include "PanelHierarchy.h"
#include "PanelComponents.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"

using namespace std;

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Editor");
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

	if (Hierarchy->GetSelectedGO())
	{
		Comp->Draw(Hierarchy->GetSelectedGO());
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

