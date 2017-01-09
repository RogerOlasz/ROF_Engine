#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include "Module.h"
#include "Wwise/SDK/samples/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h" // Sample low-level I/O implementation

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	/*void* AllocHook(size_t in_size);
	void ModuleAudio::FreeHook(void* in_ptr);

	void* ModuleAudio::VirtualAllocHook(void* in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void ModuleAudio::VirtualFreeHook(void* in_pMemAddress, size_t in_size, DWORD in_dwFreeType);*/

private:
	//CAkFilePackageLowLevelIOBlocking low_level_IO;

};

#endif // !__MODULEAUDIO_H__