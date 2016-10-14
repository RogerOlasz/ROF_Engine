#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Globals.h"
#include "Component.h"


struct aiMaterial;
struct Mesh;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* bearer, int id);
	~ComponentMaterial();

	void Init();

	void LoadTexture(Mesh* mesh, aiMaterial* ai_material);

	uint GetTexture() const;

public:
	char tex_path[SHORT_STRING];

private:
	uint texture;
	

};

#endif // !__COMPONENTMATERIAL_H__