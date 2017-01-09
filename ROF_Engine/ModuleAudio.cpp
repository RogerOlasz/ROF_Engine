#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>     // Default memory and stream managers

#include <AK/SoundEngine/Common/IAkStreamMgr.h> // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>    // Thread defines

#include <AK/SoundEngine/Common/AkSoundEngine.h>// Sound Engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>// Music Engine

#ifdef _DEBUG
#pragma comment (lib, "Wwise/Win32(140)/Debug(StaticCRT)/lib/AkSoundEngine.lib")
#pragma comment (lib, "Wwise/Win32(140)/Debug(StaticCRT)/lib/AkMemoryMgr.lib")
#pragma comment (lib, "Wwise/Win32(140)/Debug(StaticCRT)/lib/AkStreamMgr.lib")
#pragma comment (lib, "Wwise/Win32(140)/Debug(StaticCRT)/lib/AkMusicEngine.lib")
#pragma comment (lib, "Wwise/Win32(140)/Debug(StaticCRT)/lib/CommunicationCentral.lib")
#else
#pragma comment (lib, "Wwise/Win32(140)/Release(StaticCRT)/lib/AkSoundEngine.lib")
#pragma comment (lib, "Wwise/Win32(140)/Release(StaticCRT)/lib/AkMemoryMgr.lib")
#pragma comment (lib, "Wwise/Win32(140)/Release(StaticCRT)/lib/AkStreamMgr.lib")
#pragma comment (lib, "Wwise/Win32(140)/Release(StaticCRT)/lib/AkMusicEngine.lib")
#endif

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

	if (AK::MemoryMgr::Init(&memory_settings) != AK_Success)
	{
		LOG("Could not create the audio memory manager.");
		assert(!"Could not create the Streaming Manager");
		ret = false;
	}

	////STREAM MANAGER INITIALITZATION
	//AkStreamMgrSettings stream_Settings;
	//AK::StreamMgr::GetDefaultSettings(stream_Settings);

	//// Customize the Stream Manager settings here.

	//if (!AK::StreamMgr::Create(stream_Settings))
	//{
	//	LOG("Could not create the Streaming Manager.");
	//	assert(!"Could not create the Streaming Manager.");
	//	ret = false;
	//}

	//AkDeviceSettings device_settings;
	//AK::StreamMgr::GetDefaultDeviceSettings(device_settings);

	//// Customize the streaming device settings here.

	//if (low_level_IO.Init(device_settings) != AK_Success)
	//{
	//	LOG("Could not create the streaming device and Low-Level I/O system.");
	//	assert(!"Could not create the streaming device and Low-Level I/O system.");
	//	ret = false;
	//}

	////SOUND INITIALITZATION
	//AkInitSettings init_settings;
	//AkPlatformInitSettings platform_init_settings;
	//AK::SoundEngine::GetDefaultInitSettings(init_settings);
	//AK::SoundEngine::GetDefaultPlatformInitSettings(platform_init_settings);

	//if (AK::SoundEngine::Init(&init_settings, &platform_init_settings) != AK_Success)
	//{
	//	LOG("Could not initialize the Sound Engine.");
	//	assert(!"Could not initialize the Sound Engine.");
	//	return false;
	//}

	////MUSIC INITIALITZATION
	//AkMusicSettings music_init;
	//AK::MusicEngine::GetDefaultInitSettings(music_init);

	//if (AK::MusicEngine::Init(&music_init) != AK_Success)
	//{
	//	LOG("Could not initialize the Music Engine.");
	//	assert(!"Could not initialize the Music Engine.");
	//	return false;
	//}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	return true;
}

void* AK::AllocHook(size_t in_size)
{
	return malloc(in_size);
}

void AK::FreeHook(void* in_ptr)
{
	free(in_ptr);
}

void* AK::VirtualAllocHook(void* in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
{
	return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
}

void AK::VirtualFreeHook(void* in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
{
	VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
}

//namespace AK
//{
//#ifdef WIN32
//
//	void* AllocHook(size_t in_size)
//	{
//		return malloc(in_size);
//	}
//	
//	void FreeHook(void* in_ptr)
//	{
//		free(in_ptr);
//	}
//	
//	void* VirtualAllocHook(void* in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
//	{
//		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
//	}
//	
//	void VirtualFreeHook(void* in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
//	{
//		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
//	}
//	
//#endif
//}