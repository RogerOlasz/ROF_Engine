#ifndef __MODULEGEOMETRY_H__
#define __MODULEGEOMETRY_H__

#include "Module.h"
#include <vector>

struct Mesh;
struct aiMesh;
class GameObject;

class ModuleGeometry : public Module
{
public:

	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	std::vector<const Mesh*> meshes;
	
	Mesh* LoadGeometry(const aiMesh* ai_mesh);

};

#endif // __MODULEGEOMETRY_H__