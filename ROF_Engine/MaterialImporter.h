#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "ModuleSceneImporter.h"
#include <string>

class ComponentMaterial;
class Color;

class MaterialImporter
{
public:
	MaterialImporter();
	~MaterialImporter();

	bool Import(const char* file, const char* path, Color s_color, std::string &output_file);
	bool ToOwnFormat(uint size, char* buffer,Color s_color, std::string &output_file);
	void Load(const char* path, std::string &mat_tex_path, Color &mat_color, uint &buff_id);
};
#endif // !__MATERIALIMPORTER_H__