#ifndef __RESOURCEMESH__
#define __RESOURCEMESH__

#include "Resource.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh();
	~ResourceMesh();

	void CreateAABB();

	void LoadOnMemory() override;
	void FreeMemory() override;

public:
	// Vertices
	uint id_vertices = 0; // id in VRAM
	uint num_vertices = 0;
	vec* vertices = nullptr;

	// Texture coords
	uint id_tex_coord = 0; // id in VRAM
	uint num_tex_coord = 0;
	float2* tex_coord = nullptr;

	// Normals
	uint id_normals = 0; // id in VRAM
	uint num_normals = 0;
	vec* normals = nullptr;

	// Indices
	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	AABB bounding_box;
};

#endif // !__RESOURCEMESH__