#include "ResourceTexture.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ResourceTexture::ResourceTexture() : Resource(ResType::Texture)
{

}

ResourceTexture::~ResourceTexture()
{

}

void ResourceTexture::LoadOnMemory()
{
	loaded_on_memory = true;

	ilGenImages(1, (ILuint*)id_texture);
	ilBindImage(id_texture);
	ilLoadImage(resource_file.c_str());

	id_texture = ilutGLBindTexImage();
}

void ResourceTexture::UnloadFromMemory()
{
	loaded_on_memory = false;

	glDeleteBuffers(1, &id_texture);
}