#ifndef __MATERIALLOADER_H__
#define __MATERIALLOADER_H__

#include "Globals.h"
#include "SDL/include/SDL.h"

class ResourceMaterial;
class Color;
struct aiMaterial;

class MaterialLoader
{
public:
	MaterialLoader();
	~MaterialLoader();

	ResourceMaterial* MaterialImport(const aiMaterial* ai_material, Uint32 ID, const char* origin_file, const char* resource_name);
	void MaterialToOwnFormat(ResourceMaterial* r_mat);

	ResourceMaterial* MaterialLoad(Uint32 ID);
};
#endif // !__MATERIALLOADER_H__