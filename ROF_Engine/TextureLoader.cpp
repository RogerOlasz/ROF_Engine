#include "TextureLoader.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include "ResourceTexture.h"

#include "Assimp/include/material.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

TextureLoader::TextureLoader()
{
	//Initialize DevIL 
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

TextureLoader::~TextureLoader()
{

}

ResourceTexture* TextureLoader::TextureImport(const aiMaterial* ai_material, Uint32 ID, const char* resource_name)
{	
	aiString path;
	ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	std::string tex_path;
	tex_path.append("Assets/Textures/").append(App->physfs->GetFileNameFromDirPath(path.data).c_str());

	char* buffer = nullptr;
	uint size = App->physfs->Load(tex_path.c_str(), &buffer);	

	ResourceTexture* r_tex = nullptr;

	if (size > 0)
	{
		r_tex = new ResourceTexture();

		r_tex->ID = ID;
		r_tex->name = resource_name;

		std::string path = "Library/Textures/";
		path.append(std::to_string(ID)).append(".dds");

		r_tex->resource_file = path;
		r_tex->origin_file = tex_path;

		TextureToOwnFormat(r_tex, buffer, size);

		return r_tex;
	}

	return r_tex;
}

void TextureLoader::TextureToOwnFormat(ResourceTexture* r_tex, const char* buffer, uint size)
{
	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
	{
		// DDS import
		ILuint il_size;
		ILubyte* data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use

		// More help about DevIL Save methods: http://openil.sourceforge.net/docs/DevIL%20Manual.pdf
		il_size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
		if (il_size > 0)
		{
			data = new ILubyte[il_size]; // Allocate data buffer
			if (ilSaveL(IL_DDS, data, il_size) > 0) // Save to buffer with the ilSaveL function
			{
				App->physfs->Save(r_tex->resource_file.c_str(), data, il_size);
			}
			RELEASE_ARRAY(data);
			RELEASE_ARRAY(buffer);
		}
	}
}

ResourceTexture* TextureLoader::TextureLoad(Uint32 ID)
{
	std::string path = "Library/Textures/";
	path.append(std::to_string(ID)).append(".dds"); //Unique UUID for each resource file

	ResourceTexture* r_tex = new ResourceTexture();

	r_tex->ID = ID;
	r_tex->resource_file = path;

	return r_tex;
}