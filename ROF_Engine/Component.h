#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

class GameObject;

class Component
{
public:
	enum Types
	{
		Transformation,
		Geometry,
		Material,
		Camera,
		Unknown
	};

	std::string name;
	bool active = false;

public:
	Component(GameObject* bearer, Types type, int id);
	virtual ~Component();

	bool IsActive() const;

	virtual void Update() {};
	virtual void CleanUp() {};

	Types GetType() const;
	int GetID() const;

private:
	Types type = Types::Unknown;
	GameObject* game_object = nullptr;

	int id;

};

#endif // !__COMPONENT_H__

