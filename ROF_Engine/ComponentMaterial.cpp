#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Mesh.h"

#include "Assimp/include/material.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ComponentMaterial::ComponentMaterial(GameObject* bearer) : Component(bearer, Types::Material)
{
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
		ilGenImages(1, &mesh->tex_material);
		ilBindImage(mesh->tex_material);

		char material_path[SHORT_STRING];
		sprintf_s(material_path, SHORT_STRING, "%s%s", "Materials/", App->physfs->GetFileNameFromDirPath(path.C_Str()));
		char *buff;
		uint size = App->physfs->Load(material_path, &buff);

		//devIL load from buffer (not from a path as ilutGLLoadImage(char* file_name))
		ilLoadL(IL_TYPE_UNKNOWN, buff, size);
		mesh->tex_material = ilutGLBindTexImage();
	}
	else
		mesh->tex_material = 0;
}