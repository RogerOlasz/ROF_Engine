#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "ModuleSceneImporter.h"
#include <string>

class aiMesh;
struct Mesh;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	bool Import(aiMesh* new_mesh, std::string& output_file);
	bool ToOwnFormat(Mesh* mesh, std::string& output_file);
	Mesh* Load(const char* path);
};
#endif // !__MESHIMPORTER_H__