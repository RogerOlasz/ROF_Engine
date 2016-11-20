#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "ModuleSceneImporter.h"
#include <string>

class MaterialImporter
{
public:
	MaterialImporter();
	~MaterialImporter();

	bool Import(const char* file, const char* path, std::string& output_file);
	bool ToOwnFormat(const void* buffer, uint size, std::string& output_file);
};
#endif // !__MATERIALIMPORTER_H__