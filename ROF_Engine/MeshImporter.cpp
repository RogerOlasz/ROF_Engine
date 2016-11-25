#include "MeshImporter.h"
#include "Mesh.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

MeshImporter::MeshImporter()
{

}

MeshImporter::~MeshImporter()
{

}

bool MeshImporter::Import(aiMesh* ai_mesh, std::string& output_file)
{
	bool ret = true;

	if (ai_mesh == nullptr)
	{
		ret = false;
		return ret;
	}

	Mesh mesh;

	//Vertices
	mesh.num_vertices = ai_mesh->mNumVertices;
	mesh.vertices = new vec[mesh.num_vertices];
	memcpy(mesh.vertices, ai_mesh->mVertices, sizeof(vec) * mesh.num_vertices);

	//Normals
	if (ai_mesh->HasNormals())
	{
		mesh.num_normals = ai_mesh->mNumVertices;
		mesh.normals = new vec[mesh.num_normals];
		memcpy(mesh.normals, ai_mesh->mNormals, sizeof(vec) * mesh.num_vertices);
	}

	//Texture coords
	if (ai_mesh->HasTextureCoords(0))
	{
		mesh.num_tex_coord = ai_mesh->mNumVertices;
		mesh.tex_coord = new float2[mesh.num_tex_coord];
		memcpy(mesh.tex_coord, ai_mesh->mTextureCoords[0], sizeof(float2) * mesh.num_vertices);
	}

	//Colors
	if (ai_mesh->HasVertexColors(0))
	{
		mesh.colors = new vec[mesh.num_vertices];
		memcpy(mesh.colors, ai_mesh->mColors, sizeof(vec) * mesh.num_vertices);
	}

	//Indices (faces)
	if (ai_mesh->HasFaces())
	{
		mesh.num_indices = ai_mesh->mNumFaces * 3;
		mesh.indices = new uint[mesh.num_indices];

		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			if (ai_mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("[warning] Geometry face without 3 indices, it will be deleted.");
				ret = false;
				return ret;
			}
			else
			{
				memcpy(&mesh.indices[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	ret = ToOwnFormat(&mesh, output_file);

	return ret;
}

bool MeshImporter::ToOwnFormat(Mesh* mesh, std::string& output_file)
{
	bool ret = false;

	// Amount of indices / vertices / colors / normals / texture_coords 
	uint ranges[5] = { mesh->num_indices, mesh->num_vertices, (mesh->colors) ? mesh->num_vertices : 0, (mesh->normals) ? mesh->num_vertices : 0, (mesh->tex_coord) ? mesh->num_vertices : 0 };

	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_indices + sizeof(vec) * mesh->num_vertices;
	if (mesh->colors != nullptr)
	{
		size += sizeof(vec) * mesh->num_vertices;
	}
	if (mesh->normals != nullptr)
	{
		size += sizeof(vec) * mesh->num_vertices;
	}
	if (mesh->tex_coord != nullptr)
	{
		size += sizeof(vec) * mesh->num_vertices;
	}

	// Allocate
	char* data = new char[size];
	char* pointer = data;

	// Store ranges
	uint bytes = sizeof(ranges);
	memcpy(pointer, ranges, bytes);

	// Store indices
	pointer += bytes;
	bytes = sizeof(uint) * mesh->num_indices;
	memcpy(pointer, mesh->indices, bytes);

	// Store vertices
	pointer += bytes;
	bytes = sizeof(vec) * mesh->num_vertices;
	memcpy(pointer, mesh->vertices, bytes);

	if (mesh->colors != nullptr)
	{
		// Because num color size is the same as vertices.
		pointer += bytes;
		memcpy(pointer, mesh->colors, bytes);
	}
	if (mesh->normals != nullptr)
	{
		// Because num color size is the same as vertices.
		pointer += bytes;
		memcpy(pointer, mesh->normals, bytes);
	}
	if (mesh->tex_coord != nullptr)
	{
		// Because num color size is the same as vertices.
		pointer += bytes;
		bytes = sizeof(float2) * mesh->num_tex_coord;
		memcpy(pointer, mesh->tex_coord, bytes);
	}

	// Now save as new file with my own format
	if (App->physfs->Save(output_file.c_str(), data, size) != 0)
	{
		ret = true;
	}

	RELEASE(data);

	return ret;
}

Mesh* MeshImporter::Load(const char* path)
{
	if (path == nullptr)
	{
		return nullptr;
	}
	else
	{
		char* buffer = nullptr;
		uint size = App->physfs->Load(path, &buffer);

		if (buffer != nullptr && size > 0)
		{
			Mesh* mesh = new Mesh();

			// Amount of indices / vertices / colors / normals / texture_coords 
			uint ranges[5];

			uint bytes = sizeof(ranges);

			char* pointer = buffer;
			memcpy(ranges, buffer, bytes);
			pointer += bytes;

			//Indices (faces)
			mesh->num_indices = ranges[0];
			bytes = sizeof(uint) * ranges[0];
			mesh->indices = new uint[ranges[0]];
			memcpy(mesh->indices, pointer, bytes);
			pointer += bytes;

			//Vertices
			mesh->num_vertices = ranges[1];
			bytes = sizeof(vec) * ranges[1];
			mesh->vertices = new vec[ranges[1]];
			memcpy(mesh->vertices, pointer, bytes);
			pointer += bytes;

			//Colors
			bytes = sizeof(vec) * ranges[2];
			mesh->colors = new vec[ranges[2]];
			memcpy(mesh->colors, pointer, bytes);
			pointer += bytes;

			//Normals
			mesh->num_normals = ranges[3];
			bytes = sizeof(vec) * ranges[3];
			mesh->normals = new vec[ranges[3]];
			memcpy(mesh->normals, pointer, bytes);
			pointer += bytes;

			//Texture coords
			mesh->num_tex_coord = ranges[4];
			bytes = sizeof(float2) * ranges[4];
			mesh->tex_coord = new float2[ranges[4]];
			memcpy(mesh->tex_coord, pointer, bytes);
			pointer += bytes;

			return mesh;
		}
		else
		{
			return nullptr;
		}
	}
}
