#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ImGui\imgui.h"
#include "PanelConsole.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleEditor::~ModuleEditor()
{}

// Called before render is available
bool ModuleEditor::Init()
{
	Console = new PanelConsole;

	return true;
}

update_status ModuleEditor::Update(float dt)
{
	//Creating an ImGui Simple UI
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ESC"))
			{
				ImGui::EndMenu();
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	Console->Draw(&Console->c_open);

	ImGui::ShowTestWindow();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{	
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

