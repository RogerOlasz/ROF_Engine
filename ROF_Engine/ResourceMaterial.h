#ifndef __RESOURCEMATERIAL_H__
#define __RESOURCEMATERIAL_H__

#include "Resource.h"
#include "Color.h"
#include "SDL/include/SDL.h"

class ResourceTexture;

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial();
	~ResourceMaterial();

	void LoadOnMemory() override;
	void UnloadFromMemory() override;

public:
	bool have_texture = false;
	ResourceTexture* texture = nullptr;
	Uint32 resource_texture_id = 0;
	Color diffuse_color = White;
	
};

#endif // !__RESOURCEMATERIAL_H__