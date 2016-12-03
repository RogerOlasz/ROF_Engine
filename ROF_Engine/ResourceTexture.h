#ifndef __RESOURCETEXTURE_H__
#define __RESOURCETEXTURE_H__

#include "Resource.h"

class ResourceTexture : public Resource
{
public:
	ResourceTexture();
	~ResourceTexture();

	void LoadOnMemory() override;
	void UnloadFromMemory() override;

public:
	uint id_texture = 0;

};

#endif // !__RESOURCETEXTURE_H__