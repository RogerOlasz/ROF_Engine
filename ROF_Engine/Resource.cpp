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

const char* Resource::GetResourceFile() const
{
	return resource_file.c_str();
}