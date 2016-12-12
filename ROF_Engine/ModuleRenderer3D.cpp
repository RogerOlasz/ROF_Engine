#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"

#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <vector>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "DebugPainter.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Glew (ImGui) lib */

using namespace std;
using namespace math;

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Renderer3D");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("[error] OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	//Glew init
	GLenum gl_enum = glewInit();
	if (GLEW_OK != gl_enum)
	{
		LOG("[error] Glew load filed.");
	}

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("[warning]: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("[error] Error Initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		//Create lights
		lights[0].ref = GL_LIGHT0;

		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		lights[1].ref = GL_LIGHT0;

		lights[1].ambient.Set(0.5f, 0.5f, 0.5f, 1.0f);
		lights[1].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[1].SetPos(0.0f, 100.0f, 0.0f);
		lights[1].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		lights[0].Active(true);
		lights[1].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	camera = App->camera->GetCamera();

	// Projection matrix for
	OnResize((int)App->window->GetWindowSize().x, (int)App->window->GetWindowSize().y);

	//CreateDebugTexture();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	if (camera_changed)
	{
		OnResize(App->window->window_size.x, App->window->window_size.y);
		update_proj_matrix = true;
	}

	if (update_proj_matrix)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf((GLfloat*)camera->GetProjectionMatrix());
		//How it works http://www.songho.ca/opengl/gl_transform.html#example1
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		update_proj_matrix = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->GetCamera()->GetPos().x, App->camera->GetCamera()->GetPos().y, App->camera->GetCamera()->GetPos().z);
	lights[0].Render();
	lights[1].Render();

	return UPDATE_CONTINUE;
}

//Update: game core cicle
update_status ModuleRenderer3D::Update(float dt)
{
	for (uint i = 0; i < to_render.size(); i++)
	{
		glPushMatrix();
		glMultMatrixf((GLfloat*)to_render[i]->c_transform->GetGlobalMatrixT());
		
		if (to_render[i]->c_mesh)
		{
			DrawMesh((ResourceMesh*)to_render[i]->c_mesh->GetResource(), (ResourceMaterial*)to_render[i]->c_material->GetResource(), ((ResourceMaterial*)to_render[i]->c_material->GetResource())->texture,to_render[i]->c_mesh->wirefr);
		}

		glPopMatrix();
	}

	return UPDATE_CONTINUE;
}

// PostUpdate: present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	camera = nullptr;
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	camera->SetAspectRatio((float)width / (float)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)camera->GetProjectionMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

ComponentCamera* ModuleRenderer3D::GetRenderingCamera()
{
	return camera;
}

void ModuleRenderer3D::AddGOToRender(RenderComp* go_to_render)
{
	to_render.push_back(go_to_render);
}

void ModuleRenderer3D::DeleteGOToRender(RenderComp* go_to_delete)
{
	std::vector<RenderComp*>::iterator tmp = to_render.begin();
	while (tmp != to_render.end())
	{
		if ((*tmp) == go_to_delete)
		{
			to_render.erase(tmp);
			return;
		}

		tmp++;
	}
}

void ModuleRenderer3D::CleanToRender()
{
	to_render.clear();
}

//Create Black/White debug texture
void ModuleRenderer3D::CreateDebugTexture() 
{
	GLubyte checkImage[40][40][4];
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 40, 40, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleRenderer3D::DrawMesh(const ResourceMesh* mesh, ResourceMaterial* material, ResourceTexture* texture, bool wireframe)
{	
	if (mesh != nullptr)
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_tex_coord);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (wireframe)
		{
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3f(0.8f, 0.7f, 0.0f);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

			glEnable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(1.0f, 1.0f, 1.0f);
		}	
		
		if (material)
		{	
			if (mesh->num_tex_coord > 0)
			{
				if (texture)
				{
					if (texture->id_texture != 0)
					{
						if (material->alpha_test)
						{
							glEnable(GL_ALPHA_TEST);
							glAlphaFunc(GL_GREATER, material->alpha_float);
						}
						glBindTexture(GL_TEXTURE_2D, texture->id_texture);
					}
				}
			}
			glColor4f(material->diffuse_color.r, material->diffuse_color.g, material->diffuse_color.b, material->diffuse_color.a);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (material->alpha_test)
		{
			glDisable(GL_ALPHA_TEST);
		}
	}
}