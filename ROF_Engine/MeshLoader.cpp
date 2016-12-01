#include "MeshLoader.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include "ResourceMesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

MeshLoader::MeshLoader()
{

}

MeshLoader::~MeshLoader()
{

}

ResourceMesh* MeshLoader::MeshImport(const aiMesh* ai_mesh, Uint32 ID, const char* resource_name, const char* origin_file)
{
	if (ai_mesh == nullptr)
	{
		return nullptr;
	}

	ResourceMesh* r_mesh;

	//Vertices
	r_mesh->num_vertices = ai_mesh->mNumVertices;
	r_mesh->vertices = new vec[r_mesh->num_vertices];
	memcpy(r_mesh->vertices, ai_mesh->mVertices, sizeof(vec) * r_mesh->num_vertices);

	//Normals
	if (ai_mesh->HasNormals())
	{
		r_mesh->num_normals = ai_mesh->mNumVertices;
		r_mesh->normals = new vec[r_mesh->num_normals];
		memcpy(r_mesh->normals, ai_mesh->mNormals, sizeof(vec) * r_mesh->num_vertices);
	}

	//Texture coords
	if (ai_mesh->HasTextureCoords(0))
	{
		r_mesh->num_tex_coord = ai_mesh->mNumVertices;
		r_mesh->tex_coord = new float2[r_mesh->num_tex_coord];

		for (int i = 0; i < r_mesh->num_tex_coord; i++)
		{
			r_mesh->tex_coord[i].x = ai_mesh->mTextureCoords[0][i].x;
			r_mesh->tex_coord[i].y = ai_mesh->mTextureCoords[0][i].y;
		}
	}

	//Indices (faces)
	if (ai_mesh->HasFaces())
	{
		r_mesh->num_indices = ai_mesh->mNumFaces * 3;
		r_mesh->indices = new uint[r_mesh->num_indices];

		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			if (ai_mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("[warning] Geometry face without 3 indices, it will be deleted.");
				RELEASE(r_mesh);
				return nullptr;
			}
			else
			{
				memcpy(&r_mesh->indices[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	r_mesh->CreateAABB();
	r_mesh->ID = ID;
	r_mesh->name = resource_name;

	std::string path("Library/Meshes/");
	path.append(std::to_string(ID)).append(".rof");

	r_mesh->resource_file = path;
	r_mesh->origin_file = origin_file;

	MeshToOwnFormat(r_mesh);

	return r_mesh;
}

bool MeshLoader::MeshToOwnFormat(ResourceMesh* r_mesh)
{
	bool ret = false;

	// Amount of indices / vertices / normals / texture_coords 
	uint ranges[4] = { r_mesh->num_indices, r_mesh->num_vertices, r_mesh->num_normals, r_mesh->num_tex_coord };

	uint size = sizeof(ranges) + sizeof(uint) * r_mesh->num_indices + sizeof(vec) * r_mesh->num_vertices + sizeof(vec) * r_mesh->num_normals + sizeof(float2) * r_mesh->num_tex_coord;

	// Allocate
	char* data = new char[size];
	char* pointer = data;

	// Store ranges
	uint bytes = sizeof(ranges);
	memcpy(pointer, ranges, bytes);

	// Store indices
	pointer += bytes;
	bytes = sizeof(uint) * r_mesh->num_indices;
	memcpy(pointer, r_mesh->indices, bytes);

	// Store vertices
	pointer += bytes;
	bytes = sizeof(vec) * r_mesh->num_vertices;
	memcpy(pointer, r_mesh->vertices, bytes);

	if (r_mesh->normals != nullptr)
	{
		pointer += bytes;
		memcpy(pointer, r_mesh->normals, bytes);
	}
	if (r_mesh->tex_coord != nullptr)
	{
		pointer += bytes;
		bytes = sizeof(float2) * r_mesh->num_tex_coord;
		memcpy(pointer, r_mesh->tex_coord, bytes);
	}

	// Now save as new file with my own format
	if (App->physfs->Save(r_mesh->resource_file.c_str(), data, size) != 0)
	{
		ret = true;
	}

	RELEASE(data);

	return ret;
}

ResourceMesh* MeshLoader::MeshLoad(Uint32 ID)
{
	std::string path = "Library/Meshes/";
	path.append(std::to_string(ID)); //Unique UUID for each resource file

	char* buffer = nullptr;
	uint size = App->physfs->Load(path.c_str(), &buffer);

	if (buffer != nullptr && size > 0)
	{
		ResourceMesh* r_mesh = new ResourceMesh();

		// Amount of indices / vertices / normals / texture_coords 
		uint ranges[4];

		uint bytes = sizeof(ranges);

		char* pointer = buffer;
		memcpy(ranges, buffer, bytes);
		pointer += bytes;

		//Indices (faces)
		r_mesh->num_indices = ranges[0];
		bytes = sizeof(uint) * ranges[0];
		r_mesh->indices = new uint[ranges[0]];
		memcpy(r_mesh->indices, pointer, bytes);
		pointer += bytes;

		//Vertices
		r_mesh->num_vertices = ranges[1];
		bytes = sizeof(vec) * ranges[1];
		r_mesh->vertices = new vec[ranges[1]];
		memcpy(r_mesh->vertices, pointer, bytes);
		pointer += bytes;

		//Normals
		r_mesh->num_normals = ranges[2];
		bytes = sizeof(vec) * ranges[2];
		r_mesh->normals = new vec[ranges[2]];
		memcpy(r_mesh->normals, pointer, bytes);
		pointer += bytes;

		//Texture coords
		r_mesh->num_tex_coord = ranges[3];
		bytes = sizeof(float2) * ranges[3];
		r_mesh->tex_coord = new float2[ranges[3]];
		memcpy(r_mesh->tex_coord, pointer, bytes);
		pointer += bytes;

		r_mesh->ID = ID;
		r_mesh->CreateAABB();
		r_mesh->resource_file = path;

		RELEASE(buffer);

		return r_mesh;
	}
	else
	{
		return nullptr;
	}

}