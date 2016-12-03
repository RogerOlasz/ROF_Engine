#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include <string>
#include "SDL/include/SDL.h"

struct aiMesh;
class ResourceMesh;

class MeshLoader 
{
public:
	MeshLoader();
	~MeshLoader();

	ResourceMesh* MeshImport(const aiMesh* ai_mesh, Uint32 ID, const char* origin_file, const char* resource_name);
	bool MeshToOwnFormat(ResourceMesh* r_mesh);

	ResourceMesh* MeshLoad(Uint32 ID);
};

#endif // !__MESHLOADER_H__