#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>
#include "Globals.h"
#include "SDL/include/SDL.h"

class Resource
{
	//To let Loaders use protected attributes
	friend class MeshLoader;
	friend class ModuleResourceManager;

public:
	enum ResType
	{
		Mesh,
		Texture,
		Material,
		Prefab, // ?? prefab/scene
	};

	Resource(ResType type);
	virtual ~Resource();

	ResType GetType() const;
	Uint32 GetID() const;

	const char* GetResourceFile() const;

	virtual void LoadOnMemory() {};
	virtual void UnloadFromMemory() {};

protected:
	Uint32 ID;
	ResType type;

	std::string resource_file;
	std::string origin_file;
	std::string name;

};

#endif // !__RESOURCE_H__