#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
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

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	
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

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Configuration", "C", &Config->active));
			if (ImGui::MenuItem("Console", "CTR+C", &Console->active));
			if (ImGui::MenuItem("Hierarchy", "CTR+O", &Hierarchy->active));

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
	for (vector<Panel*>::iterator tmp = panels.begin(); tmp != panels.end(); ++tmp)
	{
		Panel* panel = (*tmp);

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

void ModuleEditor::LogFPS(const float* fps, const float ms)
{
	Config->Log(fps, ms);
}

uint ModuleEditor::GetMaxFPS()
{
	return Config->GetMaxFPS();
}

