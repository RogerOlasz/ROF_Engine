#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include "XMLUtilities.h"

class GameObject;
class Resource;

class Component
{
public:
	enum Type
	{
		Transformation = 0,
		Geometry,
		Material,
		Camera,
		Unknown
	};

	std::string name;
	bool active = false;

public:
	Component(GameObject* bearer, Type type, int id);
	virtual ~Component();

	bool IsActive() const;

	virtual void Update() {};

	virtual void OnLoad(pugi::xml_node&) {};
	virtual void OnSave(pugi::xml_node&) {};

	Type GetType() const;
	int GetID() const;
	const Resource* GetResource() const;

	void SetResource(Resource* resource);
	void UnsetResource(Resource* resource);
	

private:
	Type type = Type::Unknown;
	int id;

protected:
	GameObject* game_object = nullptr;

	Resource* resource = nullptr;
	
};

#endif // !__COMPONENT_H__

