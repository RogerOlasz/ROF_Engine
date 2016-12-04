#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>
#include "Globals.h"
#include "SDL/include/SDL.h"

class Resource
{
private:
	//To let Loaders use protected attributes (prefered to getters/setters)
	friend class Component;
	friend class MeshLoader;
	friend class MaterialLoader;
	friend class TextureLoader;
	friend class ModuleResourceManager;

public:
	enum ResType
	{
		Mesh = 0,
		Texture,
		Material,
	};

	Resource(ResType type);
	virtual ~Resource();

	ResType GetType() const;
	Uint32 GetID() const;

	virtual bool IsOnMemory();

	const char* GetResourceFile() const;
	const char* GetOriginFile() const;

	virtual void LoadOnMemory() {};
	virtual void UnloadFromMemory() {};

protected:
	Uint32 ID;
	ResType type;

	uint on_use = 0;

	bool loaded_on_memory = false;

	std::string resource_file;
	std::string origin_file;
	std::string name;

};

#endif // !__RESOURCE_H__