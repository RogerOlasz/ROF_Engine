#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

#include "Globals.h"
#include "SDL/include/SDL.h"

struct aiMaterial;

class ResourceTexture;

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	ResourceTexture* TextureImport(const aiMaterial* ai_material, Uint32 ID, const char* resource_name);
	void TextureToOwnFormat(ResourceTexture* r_tex, const char* buffer, uint size);

	ResourceTexture* TextureLoad(Uint32 ID);
};
#endif // !__TEXTURELOADER_H__