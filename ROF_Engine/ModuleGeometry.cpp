#include "Globals.h"
#include "Application.h"
#include "ModuleGeometry.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "ModuleGOManager.h"
#include "ModuleSceneImporter.h"
#include "Mesh.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Geometry");
}

// Destructor
ModuleGeometry::~ModuleGeometry()
{}

// Called before render is available
bool ModuleGeometry::Init()
{
	//Log messages 
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

// PreUpdate: clear buffer
update_status ModuleGeometry::PreUpdate(float dt)
{	
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
	//Detach log stream
	aiDetachAllLogStreams();

	return true;
}

Mesh* ModuleGeometry::LoadGeometry(const aiMesh* ai_mesh, const aiScene* scene, ComponentMaterial* material)
{
	LOG("[start] New mesh ----------------------------------------------------");
	Mesh* mesh = new Mesh();

	//Copying vertices to mesh
	mesh->num_vertices = ai_mesh->mNumVertices;
	mesh->vertices = new vec[mesh->num_vertices];

	memcpy(mesh->vertices, ai_mesh->mVertices, sizeof(vec) * mesh->num_vertices); 
	LOG("New mesh with %d vertices", mesh->num_vertices);

	if (ai_mesh->HasNormals())
	{
		//Copying normals to mesh
		mesh->num_normals = ai_mesh->mNumVertices;
		mesh->normals = new vec[mesh->num_normals];

		memcpy(mesh->normals, ai_mesh->mNormals, sizeof(vec) * mesh->num_vertices);
		LOG("New mesh with %d normals", mesh->num_vertices);
	}	

	//Copying texture coords to mesh
	uint tmp_UV_index = 0;
	if (ai_mesh->HasTextureCoords(tmp_UV_index))
	{
		mesh->num_tex_coord = ai_mesh->mNumVertices;
		mesh->tex_coord = new float2[mesh->num_tex_coord];

		for (int i = 0; i < mesh->num_tex_coord; i++)
		{
			mesh->tex_coord[i].x = ai_mesh->mTextureCoords[tmp_UV_index][i].x;
			mesh->tex_coord[i].y = ai_mesh->mTextureCoords[tmp_UV_index][i].y;
		}
		LOG("New mesh with %d texture coords", mesh->num_tex_coord);							
	}

	//Copying indicies to mesh (HasFaces() on Assimp)
	if (ai_mesh->HasFaces())
	{
		mesh->num_indices = ai_mesh->mNumFaces * 3;
		mesh->indices = new uint[mesh->num_indices]; 

		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			if (ai_mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("[warning] Geometry face without 3 indices, it will return null.");
				RELEASE(mesh);
				mesh = nullptr;
			}
			else
			{
				memcpy(&mesh->indices[i*3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}						
		}
	}

	if (mesh != nullptr)
	{
		//Adding texture to mesh struct (must change it to save tex only on comp material)
		App->importer->LoadTexture(material, scene->mMaterials[ai_mesh->mMaterialIndex]);
		mesh->material = material;

		App->renderer3D->LoadMeshBuffers(mesh);
	}
	LOG("[end] New mesh ------------------------------------------------------");

	return mesh;
}
