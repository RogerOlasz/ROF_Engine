#include "ResourceMesh.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ResourceMesh::ResourceMesh() : Resource(ResType::Mesh)
{

}

ResourceMesh::~ResourceMesh()
{

}

void ResourceMesh::CreateAABB()
{
	//Setting bounding box
	bounding_box.SetNegativeInfinity(); //Must be called before Enclose() to set box to null
	bounding_box.Enclose(vertices, num_vertices);
}

void ResourceMesh::LoadOnMemory()
{
	loaded_on_memory = true;

	//Loading vertices
	if (num_vertices > 0)
	{
		glGenBuffers(1, (GLuint*)&id_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	}
	else
	{
		LOG("[warning] This mesh have no vertices, buffer couldn't be load.");
	}

	//Loading normals
	if (num_normals > 0)
	{
		glGenBuffers(1, (GLuint*)&id_normals);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * num_normals * 3, normals, GL_STATIC_DRAW);
	}
	else
	{
		LOG("[warning] This mesh have no normals, buffer couldn't be load.");
	}

	//Loading texture coords (texture buffer load on ComponentMaterial)
	if (num_tex_coord > 0)
	{
		glGenBuffers(1, (GLuint*)&id_tex_coord);
		glBindBuffer(GL_ARRAY_BUFFER, id_tex_coord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * num_tex_coord * 2, tex_coord, GL_STATIC_DRAW);
	}
	else
	{
		LOG("[warning] This mesh have no UV coords, buffer couldn't be load.");
	}

	//Loading indices
	if (num_indices > 0)
	{
		glGenBuffers(1, (GLuint*)&id_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);
	}
	else
	{
		LOG("[warning] This mesh have no indices (faces), buffer couldn't be load.");
	}
}

void ResourceMesh::UnloadFromMemory()
{
	loaded_on_memory = false;

	if (id_indices != 0)
	{
		glDeleteBuffers(1, &id_indices);
	}

	if (id_normals != 0)
	{
		glDeleteBuffers(1, &id_normals);
	}

	if (id_tex_coord != 0)
	{
		glDeleteBuffers(1, &id_tex_coord);
	}

	if (id_vertices != 0)
	{
		glDeleteBuffers(1, &id_vertices);
	}
}