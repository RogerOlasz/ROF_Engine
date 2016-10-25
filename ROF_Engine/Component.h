#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

class GameObject;

class Component
{
public:
	enum Type
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
	Component(GameObject* bearer, Type type, int id);
	virtual ~Component();

	bool IsActive() const;

	virtual void Update() {};
	virtual void CleanUp() {};

	Type GetType() const;
	int GetID() const;

private:
	Type type = Type::Unknown;
	int id;

protected:
	GameObject* game_object = nullptr;
	
};

#endif // !__COMPONENT_H__

