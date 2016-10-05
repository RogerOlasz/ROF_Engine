#include "Globals.h"
#include "Application.h"
#include "ModuleGeometry.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"
#include "Mesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleGeometry::~ModuleGeometry()
{}

// Called before render is available
bool ModuleGeometry::Init()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
	return true;
}

// PreUpdate: clear buffer
update_status ModuleGeometry::PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		//LoadGeometry("Assets/Models/LamboMurcielago.fbx");
		LoadGeometry("Assets/Models/Tank.fbx");
	}
	
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGeometry::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGeometry::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGeometry::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleGeometry::LoadGeometry(const char* file_path)
{
	//const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	const aiScene* scene = aiImportFileEx(file_path, aiProcessPreset_TargetRealtime_MaxQuality, App->physfs->GetAssimpIO());

	if (scene != nullptr && scene->HasMeshes())
	{
		// For each mesh
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh* mesh = new Mesh();
			aiMesh* ai_mesh = scene->mMeshes[i];

			// Copying vertices
			mesh->num_vertices = ai_mesh->mNumVertices;
			mesh->vertices = new vec[mesh->num_vertices];
			memcpy(mesh->vertices, ai_mesh->mVertices, sizeof(vec) * mesh->num_vertices);
			LOG("New mesh with %d vertices", mesh->num_vertices);

			// Copying normals
			mesh->num_normals = ai_mesh->mNumVertices;
			mesh->normals = new vec[mesh->num_normals];
			memcpy(mesh->normals, ai_mesh->mNormals, sizeof(vec) * mesh->num_vertices);
			LOG("New mesh with %d normals", mesh->num_vertices);

			// Copying texture coords
			uint UV_index = 0;
			if (ai_mesh->HasTextureCoords(UV_index))
			{
				mesh->num_tex_coord = ai_mesh->mNumVertices;
				mesh->tex_coord = new float2[mesh->num_tex_coord];
				for (int k = 0; k < mesh->num_tex_coord; ++k)
				{
					mesh->tex_coord[k].x = ai_mesh->mTextureCoords[UV_index][k].x;
					mesh->tex_coord[k].y = ai_mesh->mTextureCoords[UV_index][k].y;
				}
				LOG("New mesh with %d texture coords", mesh->num_tex_coord);							
			}

			// Copying indicies (faces on Assimp)
			if (ai_mesh->HasFaces())
			{
				mesh->num_indices = ai_mesh->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; //Each face is a triangle
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("[warning], geometry face without 3 indices!");
					}
					else
					{
						memcpy(&mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}						
				}
			}
			App->renderer3D->LoadMeshBuffer(mesh);
			meshes.push_back(mesh);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("[error] Error loading scene %s %s", file_path, aiGetErrorString());
	}
}