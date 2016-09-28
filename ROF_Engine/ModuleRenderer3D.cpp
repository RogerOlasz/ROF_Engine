#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <vector>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Glew (ImGui) lib */

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum gl_enum = glewInit();

	if (GLEW_OK != gl_enum)
	{
		LOG("Glew load filed.");
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
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
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
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
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	ImGui_ImplSdlGL3_Init(App->window->window);

	//Load Vertex OpenGL
	//vertex_size = CubeVertexArray();
	indices_size = CubeIndices();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	//OpenGL cubepaint
	//CubePaintDirectMode();
	//DrawCubeVertexArray(vertex_size);
	DrawCubeIndices(indices_size);

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	ImGui::Render();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::CubePaintDirectMode()
{
	//Painting a cube
	glBegin(GL_TRIANGLES);

	glColor3f(255.f, 0.f, 0.f);

	glVertex3f(0.f, 0.f, 0.f);//A
	glVertex3f(5.f, 0.f, 0.f);//B
	glVertex3f(5.f, 5.f, 0.f);//C

	glVertex3f(5.f, 5.f, 0.f);//C
	glVertex3f(0.f, 5.f, 0.f);//D
	glVertex3f(0.f, 0.f, 0.f);//A

	glColor3f(0.f, 255.f, 0.f);

	glVertex3f(5.f, 0.f, 0.f);//B
	glVertex3f(5.f, 0.f, -5.f);//E
	glVertex3f(5.f, 5.f, -5.f);//F

	glVertex3f(5.f, 5.f, -5.f);//F
	glVertex3f(5.f, 5.f, 0.f);//C
	glVertex3f(5.f, 0.f, 0.f);//B

	glColor3f(0.f, 0.f, 255.f);

	glVertex3f(5.f, 0.f, -5.f);//E
	glVertex3f(0.f, 0.f, -5.f);//G
	glVertex3f(5.f, 5.f, -5.f);//F

	glVertex3f(5.f, 5.f, -5.f);//F
	glVertex3f(0.f, 0.f, -5.f);//G
	glVertex3f(0.f, 5.f, -5.f);//H

	glColor3f(125.f, 125.f, 0.f);

	glVertex3f(0.f, 5.f, -5.f);//F
	glVertex3f(0.f, 0.f, -5.f);//E
	glVertex3f(0.f, 0.f, 0.f);//B

	glVertex3f(0.f, 0.f, 0.f);//B
	glVertex3f(0.f, 5.f, 0.f);//C
	glVertex3f(0.f, 5.f, -5.f);//F

	glColor3f(0.f, 125.f, 125.f);

	glVertex3f(0.f, 5.f, 0.f);//D
	glVertex3f(5.f, 5.f, 0.f);//C
	glVertex3f(5.f, 5.f, -5.f);//F

	glVertex3f(0.f, 5.f, 0.f);//D
	glVertex3f(5.f, 5.f, -5.f);//F
	glVertex3f(0.f, 5.f, -5.f);//H

	glColor3f(125.f, 0.f, 125.f);

	glVertex3f(0.f, 0.f, 0.f);//D
	glVertex3f(5.f, 0.f, -5.f);//F
	glVertex3f(5.f, 0.f, 0.f);//C

	glVertex3f(0.f, 0.f, 0.f);//D
	glVertex3f(0.f, 0.f, -5.f);//H
	glVertex3f(5.f, 0.f, -5.f);//F
	glEnd();
}

uint ModuleRenderer3D::CubeVertexArray()
{
	vector<vec> vertices;

	//FRONT
	vertices.push_back(vec(0.f, 0.f, 0.f));//A
	vertices.push_back(vec(5.f, 0.f, 0.f));//B
	vertices.push_back(vec(5.f, 5.f, 0.f));//C

	vertices.push_back(vec(5.f, 5.f, 0.f));//C
	vertices.push_back(vec(0.f, 5.f, 0.f));//D
	vertices.push_back(vec(0.f, 0.f, 0.f));//A

	//RIGHT SIDE									   
	vertices.push_back(vec(5.f, 0.f, 0.f));//B
	vertices.push_back(vec(5.f, 0.f, -5.f));//E
	vertices.push_back(vec(5.f, 5.f, -5.f));//F

	vertices.push_back(vec(5.f, 5.f, -5.f));//F
	vertices.push_back(vec(5.f, 5.f, 0.f));//C
	vertices.push_back(vec(5.f, 0.f, 0.f));//B
										   
	//BACK									   
	vertices.push_back(vec(5.f, 0.f, -5.f));//E
	vertices.push_back(vec(0.f, 0.f, -5.f));//G
	vertices.push_back(vec(5.f, 5.f, -5.f));//F

	vertices.push_back(vec(5.f, 5.f, -5.f));//F
	vertices.push_back(vec(0.f, 0.f, -5.f));//G
	vertices.push_back(vec(0.f, 5.f, -5.f));//H

	//LEFT SIDE										
	vertices.push_back(vec(0.f, 5.f, -5.f));//H
	vertices.push_back(vec(0.f, 0.f, -5.f));//G
	vertices.push_back(vec(0.f, 0.f, 0.f));//A

	vertices.push_back(vec(0.f, 0.f, 0.f));//A
	vertices.push_back(vec(0.f, 5.f, 0.f));//D
	vertices.push_back(vec(0.f, 5.f, -5.f));//H

	//TOP										
	vertices.push_back(vec(0.f, 5.f, 0.f));//D
	vertices.push_back(vec(5.f, 5.f, 0.f));//C
	vertices.push_back(vec(5.f, 5.f, -5.f));//F

	vertices.push_back(vec(0.f, 5.f, 0.f));//D
	vertices.push_back(vec(5.f, 5.f, -5.f));//F
	vertices.push_back(vec(0.f, 5.f, -5.f));//H

	//BOTTOM										
	vertices.push_back(vec(0.f, 0.f, 0.f));//A
	vertices.push_back(vec(5.f, 0.f, -5.f));//E
	vertices.push_back(vec(5.f, 0.f, 0.f));//B

	vertices.push_back(vec(0.f, 0.f, 0.f));//A
	vertices.push_back(vec(0.f, 0.f, -5.f));//G
	vertices.push_back(vec(5.f, 0.f, -5.f));//E

	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);

	return vertices.size();
}

void ModuleRenderer3D::DrawCubeVertexArray(uint size)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// ... draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, size * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}

uint ModuleRenderer3D::CubeIndices()
{
	vector<vec> vertex;

	vertex.push_back(vec(5.f, 5.f, 0.f));//C 0
	vertex.push_back(vec(0.f, 5.f, 0.f));//D 1
	vertex.push_back(vec(0.f, 0.f, 0.f));//A 2
	vertex.push_back(vec(5.f, 0.f, 0.f));//B 3
	vertex.push_back(vec(5.f, 0.f, -5.f));//E 4
	vertex.push_back(vec(5.f, 5.f, -5.f));//F 5
	vertex.push_back(vec(0.f, 5.f, -5.f));//H 6
	vertex.push_back(vec(0.f, 0.f, -5.f));//G 7

	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex.size() * 3, vertex.data(), GL_STATIC_DRAW);

	vector<uint> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(0);

	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(1);
	indices.push_back(0);

	indices.push_back(1);
	indices.push_back(6);
	indices.push_back(7);

	indices.push_back(7);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(7);
	indices.push_back(4);
	indices.push_back(3);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(7);

	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(4);

	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indices.size(), indices.data(), GL_STATIC_DRAW);

	return indices.size();
}

void ModuleRenderer3D::DrawCubeIndices(uint size)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Perspective
	float4x4 perspective;
	perspective.SetIdentity();

	float n = 0.125f;
	float f = 512.0f;

	float coty = 1.0f / tan(60.0f * pi / 360.0f);

	//Matrix
	perspective[0][0] = coty / ((float)width / (float)height);
	perspective[1][1] = coty;
	perspective[2][2] = (n + f) / (n - f);
	perspective[2][3] = -1.0f;
	perspective[3][2] = 2.0f * n * f / (n - f);
	perspective[3][3] = 0.0f;

	ProjectionMatrix = perspective;
	glLoadMatrixf(*ProjectionMatrix.v);	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
