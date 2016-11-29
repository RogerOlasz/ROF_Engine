#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include "Assimp/include/cfileio.h"
#include <string>
#include <vector>

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
	bool CreateDir(const char* new_dir);
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	const std::string GetFileNameFromDirPath(const std::string path) const;

	bool RemoveAllSearchPaths();
	bool RemovePath(const char *path_or_zip);
	uint Load(const char* file, char **buffer) const;
	SDL_RWops *Load(const char* file) const;
	uint Save(const char *file, const void *buffer, uint size) const;

	bool IsDirectory(const char *dir) const;
	bool Exists(const char *file) const;

	aiFileIO * ModuleFileSystem::GetAssimpIO();

private:
	aiFileIO* AssimpIO = nullptr;
	void ModuleFileSystem::CreateAssimpIO();

};


#endif //!__MODULEFILESYSTEM_H__