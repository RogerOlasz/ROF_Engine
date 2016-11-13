#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "ModuleSceneImporter.h"
#include <string>

class aiMesh;
class Mesh;

class MeshImporter 
{
public:
	MeshImporter();
	~MeshImporter();

	bool Import(aiMesh* new_mesh, std::string& output_file);
	bool ToOwnFormat(Mesh* mesh, std::string& output_file);

};
#endif // !__MESHIMPORTER_H__