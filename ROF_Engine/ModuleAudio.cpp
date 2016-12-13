#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h> // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>    // Default memory and stream managers

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Audio");
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	LOG("Loading Audio Engine");
	bool ret = true;

	AkMemSettings memory_settings;
	memory_settings.uMaxNumPools = 20;

	/*if (AK::MemoryMgr::Init(&memory_settings) != AK_Success)
	{
		LOG("Could not create the audio memory manager.");
		ret = false;
	}*/

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing sound FX, closing Mixer and Audio subsystem");



	return true;
}

void* ModuleAudio::AllocHook(size_t in_size)
{
	return malloc(in_size);
}

void ModuleAudio::FreeHook(void* in_ptr)
{
	free(in_ptr);
}

void* ModuleAudio::VirtualAllocHook(void* in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
{
	return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
}

void ModuleAudio::VirtualFreeHook(void* in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
{
	VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
}