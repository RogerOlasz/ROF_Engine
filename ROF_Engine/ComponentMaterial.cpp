#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneImporter.h"
#include "MaterialImporter.h"

#include "Assimp/include/material.h"

ComponentMaterial::ComponentMaterial(GameObject* bearer, int id) : Component(bearer, Type::Material, id)
{
	//Component names are to solve problems with ImGui same names
	char tmp[SHORT_STRING];
	sprintf(tmp, "Material##%d", id);
	name = tmp;
}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::OnSave(pugi::xml_node &scene)
{
	scene = scene.append_child("Material");

	scene.append_child("Type").append_attribute("Value") = this->GetType();

	scene.append_child("Path").text().set(path.c_str());

	scene = scene.parent();
}

void ComponentMaterial::OnLoad(pugi::xml_node &scene)
{
	App->importer->material_importer->Load(scene.child("Material").child("Path").text().get(), tex_path, color, texture_id);
}

void ComponentMaterial::SetMaterialColor(float r, float g, float b, float a)
{
	color.Set(r, g, b, a);
}

void ComponentMaterial::SetTextureId(uint id)
{
	texture_id = id;
}

void ComponentMaterial::AppendTexturePath(const char* path)
{
	tex_path.append(path);
}

void ComponentMaterial::SetTexturePath(const char* path)
{
	tex_path = path;
}

uint ComponentMaterial::GetTextureId() const
{
	return texture_id;
}

const char* ComponentMaterial::GetTexturePath() const
{
	return tex_path.c_str();
}

std::string ComponentMaterial::GetTexturePathS() const
{
	return tex_path;
}

const char* ComponentMaterial::GetAdaptedTexturePath() const
{
	return tex_path.substr(tex_path.find_first_of("/") + 1).c_str();
}

Color ComponentMaterial::GetMaterialColor() const
{
	return color;
}