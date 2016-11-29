#ifndef __MODULEGEOMETRY_H__
#define __MODULEGEOMETRY_H__

#include "Module.h"
#include <vector>

struct Mesh;
struct aiMesh;
struct aiScene;
class GameObject;
class ComponentMaterial;

class ModuleGeometry : public Module
{
public:

	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Init();
	bool CleanUp();
	
	Mesh* LoadGeometry(const aiMesh* ai_mesh, const aiScene* ai_scene, ComponentMaterial* material);

};

#endif // __MODULEGEOMETRY_H__