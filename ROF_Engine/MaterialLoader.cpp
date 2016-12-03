#include "MaterialLoader.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResourceManager.h"

#include "Assimp/include/material.h"

#include "ResourceTexture.h"
#include "ResourceMaterial.h"

MaterialLoader::MaterialLoader()
{

}

MaterialLoader::~MaterialLoader()
{

}

ResourceMaterial* MaterialLoader::MaterialImport(const aiMaterial* ai_material, Uint32 ID, const char* origin_file, const char* resource_name)
{
	ResourceMaterial* r_mat = nullptr;

	if (ai_material)
	{
		r_mat = new ResourceMaterial();

		r_mat->ID = ID;
		r_mat->name = resource_name;

		std::string path = "Library/Materials/";
		path.append(std::to_string(ID)).append(".rof");

		r_mat->resource_file = path;
		r_mat->origin_file = origin_file;
		std::size_t tmp_pos = r_mat->origin_file.rfind("Assets");
		r_mat->origin_file = r_mat->origin_file.substr(tmp_pos);

		aiColor4D diff_color;
		ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, diff_color);
		r_mat->diffuse_color.Set(diff_color.r, diff_color.g, diff_color.b, diff_color.a);

		aiString ai_path;
		ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &ai_path);
		std::string tex_path;
		tex_path.append("Assets/Textures/").append(App->physfs->GetFileNameFromDirPath(ai_path.data).c_str());
		std::string tex_name;
		tex_name = App->physfs->GetFileNameFromDirPath(ai_path.data);
		tex_name = tex_name.substr(0, tex_name.size() - 4);

		if (ai_path.length > 0)
		{
			r_mat->texture = App->res_manager->ImportTextureResource(ai_material, tex_path.c_str(), tex_name.c_str());
			r_mat->resource_texture_id = r_mat->texture->GetID();
		}

		MaterialToOwnFormat(r_mat);
	}

	return r_mat;
}

void MaterialLoader::MaterialToOwnFormat(ResourceMaterial* r_mat)
{
	// Binary file import
	uint size = sizeof(float4) + sizeof(bool);
	if (r_mat->texture)
	{
		r_mat->have_texture = true;
		size += sizeof(Uint32);
	}

	// Allocate 
	char* data = new char[size];
	char* pointer = data;

	uint bytes = sizeof(float4);
	float4 color(r_mat->diffuse_color.r, r_mat->diffuse_color.g, r_mat->diffuse_color.b, r_mat->diffuse_color.a);
	memcpy(pointer, &color, bytes);
	pointer += bytes;

	bytes = sizeof(bool);
	memcpy(pointer, &r_mat->have_texture, bytes);
	pointer += bytes;

	if (r_mat->texture)
	{
		bytes = sizeof(Uint32);
		memcpy(pointer, &r_mat->resource_texture_id, bytes);
	}

	App->physfs->Save(r_mat->resource_file.c_str(), data, size);

	RELEASE_ARRAY(data);
}

ResourceMaterial* MaterialLoader::MaterialLoad(Uint32 ID)
{
	std::string path = "Library/Materials/";
	path.append(std::to_string(ID)).append(".rof"); //Unique UUID for each resource file

	char* buffer = nullptr;
	uint size = App->physfs->Load(path.c_str(), &buffer);	

	ResourceMaterial* r_mat = nullptr;

	if (buffer != nullptr && size > 0)
	{
		r_mat = new ResourceMaterial();

		uint tmp_size = sizeof(float4) + sizeof(Uint32);

		//Copy path from binary to char*
		char* pointer = buffer;
		uint bytes = sizeof(float4);
		float4 color;
		memcpy(&color, pointer, bytes);		
		r_mat->diffuse_color.Set(color.x, color.y, color.z, color.w);
		pointer += bytes;

		bytes = sizeof(bool);
		memcpy(&r_mat->have_texture, pointer, bytes);

		if (r_mat->have_texture)
		{
			pointer += bytes;
			bytes = sizeof(Uint32);
			memcpy(&r_mat->resource_texture_id, pointer, bytes);
		}

		r_mat->ID = ID;
		r_mat->resource_file = path;

		RELEASE_ARRAY(buffer);
	}
	return r_mat;
}