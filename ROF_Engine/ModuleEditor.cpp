#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ImGui\imgui.h"
#include "Panel.h"
#include "PanelConsole.h"
#include "PanelConfiguration.h"

using namespace std;

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleEditor::~ModuleEditor()
{}

// Called before render is available
bool ModuleEditor::Init()
{
	//panels.push_back(Console = new PanelConsole);
	panels.push_back(Config = new PanelConfiguration);
	Console = new PanelConsole;

	return true;
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
				ImGui::EndMenu();
				return UPDATE_STOP;
			}

			if (ImGui::MenuItem("Configuration", "C", &Config->active));

			if (ImGui::MenuItem("Console", "CTR+C"))
			{
				if (console != true)
				{
					console = true;
				}							
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}	

	if (console == true)
	{
		Console->Draw();
	}

	//Draw all active panels
	for (vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		Panel* panel = (*it);

		if (panel->IsActive())
		{
			panel->Draw();
		}
	}

	//ImGui::ShowTestWindow();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{	

	for (vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		RELEASE(*it);
	}

	panels.clear();

	delete Console;

	Console = NULL;

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

uint ModuleEditor::MaxFPS()
{
	return Config->GetMaxFPS();
}

