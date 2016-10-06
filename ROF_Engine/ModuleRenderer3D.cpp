#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleGeometry.h"
#include "Mesh.h"

#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <vector>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Glew (ImGui) lib */

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

using namespace std;
using namespace math;

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

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//Load Vertex OpenGL
	//vertex_size = CubeVertexArray();
	//indices_size = CubeIndices();

	CreateDebugTexture();
	//LoadTextureCube();
	
	//Initialize DevIL
	ilInit();
	iluInit();
	ilutInit();

	ilutRenderer(ILUT_OPENGL);

	lenna_texture = ilutGLLoadImage("Assets/Lenna.png");
	glBindTexture(GL_TEXTURE_2D, 0);

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
	//DrawCubeIndices(indices_size);

	//DrawCubeTexture();

	return UPDATE_CONTINUE;
}

//Update: game core cicle
update_status ModuleRenderer3D::Update(float dt)
{
	for (int i = 0; i < App->geometry->meshes.size(); ++i)
	{
		DrawMesh(App->geometry->meshes[i]);
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

//Load a cube with his UV
void ModuleRenderer3D::LoadTextureCube()
{
	//Loading vertex data
	vector<vec> vertices;

	vertices.push_back(vec(0.0f, 5.0f, 0.0f)); //0
	vertices.push_back(vec(5.0f, 5.0f, 5.0f)); //1
	vertices.push_back(vec(5.0f, 5.0f, 5.0f)); //2
	vertices.push_back(vec(0.0f, 5.0f, 5.0f)); //3
	vertices.push_back(vec(0.0f, 5.0f, 5.0f)); //4
	vertices.push_back(vec(5.0f, 5.0f, 0.0f)); //5
	vertices.push_back(vec(5.0f, 0.0f, 5.0f)); //6
	vertices.push_back(vec(5.0f, 0.0f, 5.0f)); //7
	vertices.push_back(vec(0.0f, 0.0f, 5.0f)); //8
	vertices.push_back(vec(0.0f, 0.0f, 5.0f)); //9
	vertices.push_back(vec(5.0f, 0.0f, 0.0f)); //10
	vertices.push_back(vec(5.0f, 0.0f, 0.0f)); //11
	vertices.push_back(vec(5.0f, 0.0f, 0.0f)); //12
	vertices.push_back(vec(0.0f, 0.0f, 0.0f)); //13
	vertices.push_back(vec(0.0f, 0.0f, 0.0f)); //14
	vertices.push_back(vec(0.0f, 0.0f, 0.0f)); //15

	glGenBuffers(1, (GLuint*)&texture_vertex_id);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vertex_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);

	//Loading UV data
	vector<float2> UV_texture;

	UV_texture.push_back(float2(1.0f, 1.0f)); 
	UV_texture.push_back(float2(0.0f, 0.0f));
	UV_texture.push_back(float2(1.0f, 1.0f));
	UV_texture.push_back(float2(1.0f, 0.0f));
	UV_texture.push_back(float2(0.0f, 1.0f));
	UV_texture.push_back(float2(0.0f, 1.0f));
	UV_texture.push_back(float2(1.0f, 0.0f));
	UV_texture.push_back(float2(0.0f, 0.0f));
	UV_texture.push_back(float2(0.0f, 0.0f));
	UV_texture.push_back(float2(1.0f, 0.0f));
	UV_texture.push_back(float2(0.0f, 0.0f));
	UV_texture.push_back(float2(1.0f, 1.0f));
	UV_texture.push_back(float2(0.0f, 1.0f));
	UV_texture.push_back(float2(0.0f, 1.0f));
	UV_texture.push_back(float2(1.0f, 0.0f));
	UV_texture.push_back(float2(1.0f, 1.0f));

	glGenBuffers(1, (GLuint*)&texture_UV_id);
	glBindBuffer(GL_ARRAY_BUFFER, texture_UV_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * UV_texture.size() * 2, UV_texture.data(), GL_STATIC_DRAW);

	//Loading index data
	vector<uint> indices_texture;

	//FRONT 
	indices_texture.push_back(10);
	indices_texture.push_back(14);
	indices_texture.push_back(0);

	indices_texture.push_back(10);
	indices_texture.push_back(0);
	indices_texture.push_back(5);

	//RIGHT
	indices_texture.push_back(13);
	indices_texture.push_back(8);
	indices_texture.push_back(3);

	indices_texture.push_back(13);
	indices_texture.push_back(3);
	indices_texture.push_back(0);

	//LEFT
	indices_texture.push_back(6);
	indices_texture.push_back(11);
	indices_texture.push_back(5);

	indices_texture.push_back(6);
	indices_texture.push_back(5);
	indices_texture.push_back(1);

	//BACK
	indices_texture.push_back(8);
	indices_texture.push_back(6);
	indices_texture.push_back(2);

	indices_texture.push_back(8);
	indices_texture.push_back(2);
	indices_texture.push_back(4);

	//TOP
	indices_texture.push_back(5);
	indices_texture.push_back(3);
	indices_texture.push_back(1);

	indices_texture.push_back(5);
	indices_texture.push_back(0);
	indices_texture.push_back(3);

	//BOTTOM
	indices_texture.push_back(15);
	indices_texture.push_back(12);
	indices_texture.push_back(7);

	indices_texture.push_back(15);
	indices_texture.push_back(7);
	indices_texture.push_back(9);

	glGenBuffers(1, (GLuint*)&texture_index_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture_index_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices_texture.size(), indices_texture.data(), GL_STATIC_DRAW);
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

void ModuleRenderer3D::DrawCubeTexture()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vertex_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, texture_UV_id);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture_index_id);

	glBindTexture(GL_TEXTURE_2D, lenna_texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool ModuleRenderer3D::LoadMeshBuffer(const Mesh* mesh)
{
	bool ret = true;

	// Vertices
	glGenBuffers(1, (GLuint*) &(mesh->id_vertices));
	if (mesh->id_vertices == 0)
	{
		LOG("[error] Vertices buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);
	}

	// Normals
	glGenBuffers(1, (GLuint*) &(mesh->id_normals));
	if (mesh->id_normals == 0)
	{
		LOG("[error] Normals buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);
	}

	// Texture coords
	glGenBuffers(1, (GLuint*) &(mesh->id_tex_coord));
	if (mesh->id_tex_coord == 0)
	{
		LOG("[error] Texture coordinates buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_tex_coord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_tex_coord * 2, mesh->tex_coord, GL_STATIC_DRAW);
	}

	// Indices
	glGenBuffers(1, (GLuint*) &(mesh->id_indices));
	if (mesh->id_indices == 0)
	{
		LOG("[error] Indices buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
	}

	return ret;
}

void ModuleRenderer3D::DrawMesh(const Mesh* mesh)
{	
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
	glBindTexture(GL_TEXTURE_2D, image_texture);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}