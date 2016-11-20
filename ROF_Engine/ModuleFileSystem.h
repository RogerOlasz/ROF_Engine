#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include <string>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops *rw);

//Module has been extracted from project 2 
class ModuleFileSystem : public Module
{
public:
	
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Init();
	bool CleanUp();

	bool AddSearchPath(const char *path_or_zip, const char *mount_point = NULL);
	const std::string GetFileNameFromDirPath(const std::string path) const;

	bool RemoveAllSearchPaths();
	bool RemovePath(const char *path_or_zip);
	uint Load(const char* file, char **buffer) const;
	SDL_RWops *Load(const char* file) const;
	uint Save(const char *file, const void *buffer, uint size) const;

	const char *GetSaveDirectory() const;
	bool IsDirectory(const char *dir) const;
	bool Exists(const char *file) const;
	
};


#endif //!__MODULEFILESYSTEM_H__