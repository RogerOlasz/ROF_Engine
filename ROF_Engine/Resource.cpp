#include "Resource.h"

Resource::Resource(ResType type) : type(type)
{

}

Resource::~Resource()
{

}

Resource::ResType Resource::GetType() const
{
	return type;
}

Uint32 Resource::GetID() const
{
	return ID;
}

bool Resource::IsOnMemory()
{
	return loaded_on_memory;
}

const char* Resource::GetResourceFile() const
{
	return resource_file.c_str();
}

const char* Resource::GetOriginFile() const
{
	return origin_file.c_str();
}