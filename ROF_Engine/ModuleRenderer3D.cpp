#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "Mesh.h"
#include "ComponentCamera.h"

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
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		lights[0].Active(true);
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
			DrawMesh(to_render[i]->c_mesh->GetMesh(), to_render[i]->c_mesh->wirefr);
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

void ModuleRenderer3D::LoadMeshBuffers(const Mesh* mesh)
{
	//Loading vertices
	if (mesh->num_vertices > 0)
	{
		glGenBuffers(1, (GLuint*)&mesh->id_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);
	}
	else
	{
		LOG("[warning] This mesh have no vertices, buffer couldn't be load.");
	}	

	//Loading normals
	if (mesh->num_normals > 0)
	{
		glGenBuffers(1, (GLuint*)&mesh->id_normals);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);
	}
	else
	{
		LOG("[warning] This mesh have no normals, buffer couldn't be load.");
	}

	//Loading texture coords (texture buffer load on ComponentMaterial)
	if (mesh->num_tex_coord > 0)
	{
		glGenBuffers(1, (GLuint*)&mesh->id_tex_coord);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_tex_coord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh->num_tex_coord * 2, mesh->tex_coord, GL_STATIC_DRAW);		
	}	
	else 
	{
		LOG("[warning] This mesh have no UV coords, buffer couldn't be load.");
	}

	//Loading indices
	if (mesh->num_indices > 0)
	{
		glGenBuffers(1, (GLuint*)&mesh->id_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
	}
	else
	{
		LOG("[warning] This mesh have no indices (faces), buffer couldn't be load.");
	}
}

void ModuleRenderer3D::RemoveMeshBuffers(Mesh* mesh)
{
	if (mesh->id_indices != 0)
	{
		glDeleteBuffers(1, &mesh->id_indices);
	}

	if (mesh->id_normals != 0)
	{
		glDeleteBuffers(1, &mesh->id_normals);
	}

	if (mesh->id_tex_coord != 0)
	{
		glDeleteBuffers(1, &mesh->id_tex_coord);
	}

	if (mesh->id_vertices != 0)
	{
		glDeleteBuffers(1, &mesh->id_vertices);
	}
}

void ModuleRenderer3D::DrawMesh(const Mesh* mesh, bool wireframe)
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

		//If mesh have UV coords...
		if (mesh->num_tex_coord > 0)
		{
			//If mesh have any material...
			if (mesh->material->GetTextureId() != 0)
			{
				glBindTexture(GL_TEXTURE_2D, mesh->material->GetTextureId());
			}
			glColor4f(mesh->material->GetMaterialColor().r, mesh->material->GetMaterialColor().g, mesh->material->GetMaterialColor().b, mesh->material->GetMaterialColor().a);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}