#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class Component
{
public:
	enum Types
	{
		Transformation,
		Geometry,
		Material,
		Unknown
	};


public:
	Component();
	virtual ~Component();

	bool IsActive() const;
	void SwitchActive(bool active);

private:
	bool active = true;

};

#endif // !__COMPONENT_H__

