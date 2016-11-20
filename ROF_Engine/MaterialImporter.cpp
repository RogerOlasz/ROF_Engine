#include "MaterialImporter.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Globals.h"

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

bool MaterialImporter::Import(const char* file, const char* path, std::string& output_file)
{
	bool ret = false;

	std::string file_location;
	char tmp_c[LONG_STRING];
	sprintf(tmp_c, path, file);
	file_location = tmp_c;

	char* buffer = nullptr;
	uint size = App->physfs->Load(file_location.c_str(), &buffer); //Load throw physfs

	if (buffer != nullptr)
	{
		ret = ToOwnFormat(buffer, size, output_file);
	}
	else
	{
		LOG("[warning] Buffer is empty, texture couldn't import to own format.");
	}

	RELEASE_ARRAY(buffer);
	
	return ret;
}

bool MaterialImporter::ToOwnFormat(const void* buffer, uint size, std::string& output_file)
{
	bool ret = false;

	if (buffer == nullptr)
	{
		ret = false;
		LOG("Buffer was empty, couldn't import to own format.");
		return ret;
	}

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
			ret = App->physfs->Save(output_file.c_str(), data, il_size);
		}

		RELEASE_ARRAY(data);
	}	

	return ret;
}