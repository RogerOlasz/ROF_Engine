#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Mesh.h"

#include "Assimp/include/material.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ComponentMaterial::ComponentMaterial(GameObject* bearer, int id) : Component(bearer, Type::Material, id)
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

		tex_path.assign("Assets/Textures/");
		tex_path.append(App->physfs->GetFileNameFromDirPath(path.data));
		texture_id = mesh->id_tex_material;

		//Adapt devIL to OpenGL buffer
		ilutRenderer(ILUT_OPENGL);
		ilGenImages(1, &texture_id);
		ilBindImage(texture_id);
		ilLoadImage(tex_path.c_str());

		texture_id = ilutGLBindTexImage();

		mesh->id_tex_material = texture_id;
	}
	else
	{
		texture_id = 0;
		mesh->id_tex_material = 0;
		LOG("[error] aiMaterial couldn't be load.");		
	}

	tex_path = tex_path.substr(tex_path.find_first_of("/") + 1);
}

uint ComponentMaterial::GetTexture() const
{
	return texture_id;
}