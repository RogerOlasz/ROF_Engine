#include "Application.h"
#include "ModuleFileSystem.h"
#include "SDL/include/SDL.h"
#include "PhysFS/include/physfs.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("FileSystem");

	// PHYSFS must be initialized before other modules awake,
	// because it will be used by them.
	if (PHYSFS_isInit() == 0)
	{
		char* base_path = SDL_GetBasePath();
		PHYSFS_init(base_path);
		SDL_free(base_path);

		AddSearchPath(".");
		AddSearchPath("Assets");
		if (PHYSFS_setWriteDir(".") == 0)
		{
			LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());
		}

		CreateDir("EditorConfig");
		CreateDir("Library");
		CreateDir("Library/Meshes");
		CreateDir("Library/Materials");
		CreateDir("Library/Textures");
	}	

	CreateAssimpIO();
}

ModuleFileSystem::~ModuleFileSystem()
{
	if (PHYSFS_isInit() != 0)
	{
		PHYSFS_deinit();
	}		
}

bool ModuleFileSystem::Init()
{
	bool ret = true;

	char* write_dir = SDL_GetPrefPath(App->GetAppName(), App->GetOrganization());

	//if (PHYSFS_setWriteDir(write_dir) == 0)
	//{
	//	LOG("%s,%s","[error] Error on setting Write Dir. Error:", PHYSFS_getLastError());
	//	ret = false;
	//}
	//else
	//{
	//	LOG("%s %s", "Write directory is ", write_dir);
	//	//AddSearchPath(write_dir, GetSaveDirectory());
	//	AddSearchPath(write_dir);
	//	AddSearchPath("Assets/Models", "Models");
	//}

	SDL_free(write_dir);
	
	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	RELEASE(AssimpIO);
	RemoveAllSearchPaths();

	PHYSFS_deinit();

	return true;
}

//Add or search a path
bool ModuleFileSystem::AddSearchPath(const char *path_or_zip, const char *mount_point)
{
	bool ret = true;

	if (PHYSFS_mount(path_or_zip, mount_point, 1) == 0)
	{
		LOG("%s %s", "[error] Failure on mounting or adding path", path_or_zip);
		LOG("%s", "Error:", PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}

bool ModuleFileSystem::CreateDir(const char* new_dir)
{
	bool ret = false;
	if (IsDirectory(new_dir) == false)
	{
		PHYSFS_mkdir(new_dir);
		ret = true;
	}

	return ret;
}

void ModuleFileSystem::DiscoverFiles(const char* directory, std::vector<std::string> & file_list, std::vector<std::string> & dir_list) const
{
	char **rc = PHYSFS_enumerateFiles(directory);
	char **i;

	std::string dir(directory);

	for (i = rc; *i != nullptr; i++)
	{
		if (PHYSFS_isDirectory((dir + *i).c_str()))
		{
			dir_list.push_back(*i);
		}
		else
		{
			file_list.push_back(*i);
		}
	}
	PHYSFS_freeList(rc);
}

//Clean a path to return the file name, method info: http://www.cplusplus.com/reference/string/string/find_last_of/
const std::string ModuleFileSystem::GetFileNameFromDirPath(const std::string path) const
{
	return path.substr(path.find_last_of("\\") + 1);
}

bool ModuleFileSystem::RemoveAllSearchPaths()
{
	bool ret = false;

	char **paths;
	for (paths = PHYSFS_getSearchPath(); *paths != NULL; paths++)
	{
		PHYSFS_removeFromSearchPath(*paths);
	}

	if (*(paths = PHYSFS_getSearchPath()) == NULL)
	{
		ret = true;
	}	

	PHYSFS_freeList(paths);

	return ret;
}

bool ModuleFileSystem::RemovePath(const char *path_or_zip)
{
	bool ret = true;
		
	if (PHYSFS_removeFromSearchPath(path_or_zip) == 0)
	{
		LOG("%s %s", "[error] Failure on removing directory or file on search path", path_or_zip);
		LOG("%s", "Error:", PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}

uint ModuleFileSystem::Load(const char* file, char **buffer) const
{
	uint ret = 0;

	PHYSFS_file *file_handle = PHYSFS_openRead(file);
	if (file_handle != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(file_handle);
		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 bytes_readed = PHYSFS_read(file_handle, *buffer, 1, size);
			if (bytes_readed != size)
			{
				LOG("[error] File system error while reading from file %s: %s", file, PHYSFS_getLastError());
				if (buffer != nullptr)
				{
					delete buffer;
				}					
			}
			else
				ret = (uint)size;
		}

		if (PHYSFS_close(file_handle) == 0)
			LOG("[error] File %s is not closed properly. Error: %s", file, PHYSFS_getLastError());
	}

	return ret;
}

SDL_RWops *ModuleFileSystem::Load(const char *file) const
{
	SDL_RWops *ret = NULL;
	char *buffer;
	uint size = Load(file, &buffer);

	if (size > 0)
	{
		ret = SDL_RWFromConstMem(buffer, size);
		if (ret != NULL)
		{
			ret->close = close_sdl_rwops;
		}			
	}
	
	return ret;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	if (rw->hidden.mem.base != nullptr)
	{
		delete rw->hidden.mem.base;
	}

	SDL_FreeRW(rw);
	return 0;
}

uint ModuleFileSystem::Save(const char* file, const void* buffer, uint size) const
{
	uint ret = 0;
	
	PHYSFS_file* file_handle = PHYSFS_openWrite(file);
	if (file_handle != NULL)
	{
		PHYSFS_sint64 bytes_written = PHYSFS_write(file_handle, (const void*) buffer, 1, size);
		if (bytes_written != size)
		{
			LOG("[error] Failure on writing %s. Error: %s", file, PHYSFS_getLastError());
		}			
		else
		{
			ret = (uint)size;
		}	

		if (PHYSFS_close(file_handle) == 0)
		{
			LOG("[error] File System can not close file %s. Error: %s", file, PHYSFS_getLastError());
		}			
	}
	else
	{
		LOG("[error] File System failure while opening file %s. Error: %s", file, PHYSFS_getLastError());
	}		
	
	return ret;
}

bool ModuleFileSystem::IsDirectory(const char *dir) const
{
	return (PHYSFS_isDirectory(dir) == 0) ? false : true;
}

bool ModuleFileSystem::Exists(const char *file) const
{
	return (PHYSFS_exists(file) == 0) ? false : true;
}

// -----------------------------------------------------
// ASSIMP IO
// -----------------------------------------------------

size_t AssimpWrite(aiFile* file, const char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_write((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if (ret == -1)
		LOG("[error] File System error while WRITE via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpRead(aiFile* file, char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_read((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if (ret == -1)
		LOG("[error] File System error while READ via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpTell(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_tell((PHYSFS_File*)file->UserData);
	if (ret == -1)
		LOG("[error] File System error while TELL via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpSize(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_fileLength((PHYSFS_File*)file->UserData);
	if (ret == -1)
		LOG("[error] File System error while SIZE via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

void AssimpFlush(aiFile* file)
{
	if (PHYSFS_flush((PHYSFS_File*)file->UserData) == 0)
		LOG("[error] File System error while FLUSH via assimp: %s", PHYSFS_getLastError());
}

aiReturn AssimpSeek(aiFile* file, size_t pos, aiOrigin from)
{
	int res = 0;

	switch (from)
	{
	case aiOrigin_SET:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, pos);
		break;
	case aiOrigin_CUR:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_tell((PHYSFS_File*)file->UserData) + pos);
		break;
	case aiOrigin_END:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_fileLength((PHYSFS_File*)file->UserData) + pos);
		break;
	}

	if (res == 0)
		LOG("[error] File System error while SEEK via assimp: %s", PHYSFS_getLastError());

	return (res != 0) ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

aiFile* AssimpOpen(aiFileIO* io, const char* name, const char* format)
{
	static aiFile file;

	file.UserData = (char*)PHYSFS_openRead(name);
	file.ReadProc = AssimpRead;
	file.WriteProc = AssimpWrite;
	file.TellProc = AssimpTell;
	file.FileSizeProc = AssimpSize;
	file.FlushProc = AssimpFlush;
	file.SeekProc = AssimpSeek;

	return &file;
}

void AssimpClose(aiFileIO* io, aiFile* file)
{
	if (PHYSFS_close((PHYSFS_File*)file->UserData) == 0)
		LOG("[error] File System error while CLOSE via assimp: %s", PHYSFS_getLastError());
}

void ModuleFileSystem::CreateAssimpIO()
{
	if (AssimpIO)
	{
		delete AssimpIO;
		AssimpIO = nullptr;
	}

	AssimpIO = new aiFileIO;
	AssimpIO->OpenProc = AssimpOpen;
	AssimpIO->CloseProc = AssimpClose;
}

aiFileIO* ModuleFileSystem::GetAssimpIO()
{
	return AssimpIO;
}