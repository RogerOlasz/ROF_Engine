#include "PanelConfiguration.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"
#include "ImGui/imgui.h"
#include "SDL/include/SDL_opengl.h"

PanelConfiguration::PanelConfiguration() : Panel("Configuration")
{
	while (fps_log.size() <= 49)
	{
		fps_log.push_back(0);
	}

	while (ms_log.size() <= 49)
	{
		ms_log.push_back(0);
	}

	active = true;
	set_size = true;

	brightness = SDL_GetWindowBrightness(App->window->window);
	gamma = SDL_GetWindowGammaRamp(App->window->window, nullptr, nullptr, nullptr);
}

PanelConfiguration::~PanelConfiguration()
{
	fps_log.clear();
	ms_log.clear();
}

void PanelConfiguration::Draw()
{
	ImGui::SetNextWindowPos(ImVec2((App->window->GetWindowSize().x - 335), (App->window->GetWindowSize().y - 380)));

	if (set_size == true)
	{
		ImGui::SetNextWindowSize(ImVec2(330, 375));
		set_size = false;
	}
	
	ImGui::Begin("Configuration", &active);
	
	DrawApplication();
	DrawWindow();
	DrawIlumination();
	DrawTecnology();
	DrawEditorCamera();

	ImGui::End();
}

void PanelConfiguration::DrawWindow()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::DragFloat("Brightness", &brightness, 0.01f, 0.1f))
		{
			SDL_SetWindowBrightness(App->window->window, brightness);
		}		
		
		if (ImGui::DragFloat("Gamma", &gamma, 0.01f, 0.1f))
		{
			SDL_SetWindowGammaRamp(App->window->window, nullptr, nullptr, nullptr);
		}
		//TODO Must add a dragfloat2 to change window resolution SDL_SetWindowResolution(w, h);

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
		{
			SDL_SetWindowFullscreen(App->window->window, fullscreen);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
		{

		}

		if (ImGui::Checkbox("Borderless", &borderless))
		{
			SDL_SetWindowBordered(App->window->window, (SDL_bool)!borderless);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Full desktop", &fullscreen_desktop))
		{

		}
	}
}

void PanelConfiguration::DrawIlumination()
{
	if (ImGui::CollapsingHeader("Scene Lights"))
	{
		for (int num_light = 0; num_light < MAX_LIGHTS; num_light++)
		{
			char light_name[SHORT_STRING];
			sprintf(light_name, "Light %d", num_light);

			bool light_on = App->renderer3D->lights[num_light].on;

			ImGui::Checkbox(light_name, &light_on);

			if (light_on != App->renderer3D->lights[num_light].on)
			{
				App->renderer3D->lights[num_light].Active(light_on);
			}

			if (App->renderer3D->lights[num_light].on == true)
			{
				sprintf(light_name, "Light position##Light_%d", num_light);
				ImGui::SameLine();

				if (ImGui::TreeNode(light_name))
				{
					char tmp[SHORT_STRING];
					sprintf(tmp, "X##light_%d", num_light);
					ImGui::DragFloat(tmp, &App->renderer3D->lights[num_light].position.x, 1.0f);

					sprintf(tmp, "Y##light_%d", num_light);
					ImGui::DragFloat(tmp, &App->renderer3D->lights[num_light].position.y, 1.0f);

					sprintf(tmp, "Z##light_%d", num_light);
					ImGui::DragFloat(tmp, &App->renderer3D->lights[num_light].position.z, 1.0f);

					ImGui::TreePop();
				}
			}
		}
	}
}

void PanelConfiguration::DrawTecnology()
{
	if (ImGui::CollapsingHeader("Tecnology"))
	{
		ImGui::Separator();
		ImGui::LabelText("", "%s", "Hardware");
		ImGui::Separator();

		//CPU count + cache
		ImGui::Text("CPUs:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.00f, 1.00f), "%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

		//System RAM
		ImGui::Text("System RAM:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.00f, 1.00f), "%dGb", (SDL_GetSystemRAM()/1000));

		////Caps
		//ImGui::Text("Caps:"); ImGui::SameLine();
		//ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.00f, 1.00f), "%s, %s, %s, %s, %s, %s.", SDL_HasRDTSC(), SDL_HasMMX(), SDL_HasSSE(), SDL_HasSSE2(), SDL_HasSSE3(), SDL_HasSSE41(), SDL_HasSSE42(), SDL_HasAVX());

		//GPU
		ImGui::Text("\nGPU:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.00f, 1.00f), "\n%s", glGetString(GL_VENDOR));

		//Brand
		ImGui::Text("Brand:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.00f, 1.00f), "%s", glGetString(GL_RENDERER));

		ImGui::Separator();
		ImGui::LabelText("", "%s", "Software");
		ImGui::Separator();

		//SDL version
		SDL_version ver; 
		SDL_GetVersion(&ver);
		ImGui::Text("SDL Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%u.%u.%u", ver.major, ver.minor, ver.patch);

		//OpenGL version
		ImGui::Text("OpenGL Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", glGetString(GL_VERSION));
		
		//ImGUI version
		ImGui::Text("ImGUI Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", ImGui::GetVersion());

	}
}

void PanelConfiguration::DrawEditorCamera()
{
	if (ImGui::CollapsingHeader("Editor's camera"))
	{
		near_plane = App->camera->GetCamera()->GetNearPlane();
		far_plane = App->camera->GetCamera()->GetFarPlane();
		field_of_view = App->camera->GetCamera()->GetFOV();
		aspect_ratio = App->camera->GetCamera()->GetAspectRatio();

		if (ImGui::DragFloat("Near plane", &near_plane, 0.1f))
		{
			App->camera->GetCamera()->SetNearPlane(near_plane);
		}
		if (ImGui::DragFloat("Far plane", &far_plane, 0.1f))
		{
			App->camera->GetCamera()->SetFarPlane(far_plane);
		}
		if (ImGui::DragFloat("Field of view", &field_of_view, 0.1f))
		{
			App->camera->GetCamera()->SetFOV(field_of_view);
		}
		if (ImGui::DragFloat("Aspect ratio", &aspect_ratio, 0.01f))
		{
			App->camera->GetCamera()->SetAspectRatio(aspect_ratio);
		}
	}
}

void PanelConfiguration::DrawApplication()
{	
	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_YELLOW, "%i", max_fps);
		ImGui::SliderInt("Max FPS", &max_fps, 0, 120);

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	}	
}

void PanelConfiguration::Log(const float* fps, const float ms)
{
	for (int i = 0; i < fps_log.size() - 1; i++)
	{
		fps_log[i] = fps_log[i + 1];
	}
	fps_log[fps_log.size() - 1] = *fps;

	for (int i = 0; i < ms_log.size() - 1; i++)
	{
		ms_log[i] = ms_log[i + 1];
	}
	ms_log[ms_log.size() - 1] = ms;
}

unsigned int PanelConfiguration::GetMaxFPS()
{
	return max_fps;
}