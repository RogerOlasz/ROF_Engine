#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Globals.h"
#include "Component.h"
#include <string>

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
	std::string tex_path;

private:
	uint texture_id;
	

};

#endif // !__COMPONENTMATERIAL_H__