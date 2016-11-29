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

	ResourceMesh* MeshImport(const aiMesh* new_mesh, Uint32 ID, const char* resource_name, const char* origin_file);
	bool MeshToOwnFormat(ResourceMesh* mesh);

	ResourceMesh* MeshLoad(Uint32 ID);
};

#endif // !__MESHLOADER_H__