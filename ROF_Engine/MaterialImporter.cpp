#include "MaterialImporter.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ComponentMaterial.h"
#include "Globals.h"
#include "Color.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

MaterialImporter::MaterialImporter()
{
	//Should be on any Init() method
	ilInit();
	iluInit();
	ilutInit();
}

MaterialImporter::~MaterialImporter()
{

}

bool MaterialImporter::Import(const char* file, const char* path, Color s_color, std::string &output_file)
{
	bool ret = false;

	std::string file_location;
	file_location.append(path).append(file);

	char* buffer = nullptr;
	uint size = App->physfs->Load(file_location.c_str(), &buffer); 

	if (buffer != nullptr)
	{
		ret = ToOwnFormat(s_color, output_file);
	}
	else
	{
		LOG("[warning] Buffer is empty, texture couldn't import to own format.");
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

bool MaterialImporter::ToOwnFormat(Color s_color, std::string &output_file)
{
	bool ret = false;
	
	// Binary file import
	uint size_m = sizeof(char) * output_file.size() + sizeof(float4); 

	// Allocate 
	char* data_m = new char[size_m];
	char* pointer = data_m;

	// Store texture path 
	uint bytes = sizeof(char);
	uint path_size = output_file.size();
	memcpy(pointer, &path_size, bytes);
	pointer += bytes;

	bytes = output_file.size();
	memcpy(pointer, output_file.c_str(), bytes);
	pointer += bytes;

	float4 color(s_color.r, s_color.g, s_color.b, s_color.a);
	bytes = sizeof(float4);
	memcpy(pointer, &color, bytes);
	pointer += bytes;

	App->physfs->Save(output_file.c_str(), data_m, size_m);

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
			ret = App->physfs->Save(output_file.append(".dds").c_str(), data, il_size);
		}

		RELEASE_ARRAY(data);
	}

	return ret;
}

void MaterialImporter::Load(const char* path, std::string &mat_tex_path, Color mat_color, uint buff_id)
{
	if (path == nullptr)
	{
		LOG("[error] Path on material importer load is null.");
	}
	else
	{
		char* buffer = nullptr;
		uint size = App->physfs->Load(path, &buffer);

		if (buffer != nullptr && size > 0)
		{
			//Copy path from binary to char*
			char* pointer = buffer;
			uint bytes = sizeof(char);
			uint path_size = 0;
			memcpy(&path_size, pointer, bytes);
			pointer += bytes;

			char* tex_path = new char[path_size + 1]; //Save an extra space to finish the char string
			bytes = sizeof(char) * path_size;
			memcpy(tex_path, pointer, bytes);
			pointer += bytes;

			tex_path[path_size] = '\0'; //Must finish string of chars
			mat_tex_path = tex_path;
			RELEASE_ARRAY(tex_path);

			if (!mat_tex_path.empty())
			{
				App->importer->LoadTextureBuffer(mat_tex_path.c_str(), buff_id);
			}

			float4 color;
			bytes = sizeof(float4);
			memcpy(&color, pointer, bytes);
			pointer += bytes;
			mat_color.Set(color.x, color.y, color.z, color.w);

			RELEASE_ARRAY(buffer);

		}
		else
		{
			LOG("[error] Path on material importer gave an empty buffer when load.");
		}
	}
}