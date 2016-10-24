#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Mesh.h"
#include <string>

#include "Assimp/include/material.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ComponentMaterial::ComponentMaterial(GameObject* bearer, int id) : Component(bearer, Types::Material, id)
{
	//Component names are to solve problems with ImGui same names
	char tmp[SHORT_STRING];
	sprintf(tmp, "Material##%d", id);
	name = tmp;

	//Components must have one init, temporal solution
	Init();
}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::Init()
{
	//Initialize DevIL 
	ilInit();
	iluInit();
	ilutInit();
}

void ComponentMaterial::LoadTexture(Mesh* mesh, aiMaterial* ai_material)
{
	uint tex_num = ai_material->GetTextureCount(aiTextureType_DIFFUSE);

	if (tex_num != 0)
	{
		aiString path;
		ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		//Adapt devIL to OpenGL buffer
		ilutRenderer(ILUT_OPENGL);

		//Loading texture
		ilGenImages(1, &mesh->id_tex_material);
		ilBindImage(mesh->id_tex_material);

		//Saving path for component info
		sprintf_s(tex_path, SHORT_STRING, "%s%s", "Textures/", App->physfs->GetFileNameFromDirPath(path.C_Str()));

		char *buff = nullptr;
		uint size = App->physfs->Load(tex_path, &buff);

		//devIL load from buffer (not from a path as ilutGLLoadImage(char* file_name))
		ilLoadL(IL_TYPE_UNKNOWN, buff, size);
		mesh->id_tex_material = ilutGLBindTexImage();

		texture = mesh->id_tex_material;
	}
	else
	{
		LOG("[error] aiMaterial couldn't be load.");
		mesh->id_tex_material = 0;
	}
}

uint ComponentMaterial::GetTexture() const
{
	return texture;
}