#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Globals.h"
#include "Component.h"
#include <vector>
#include <string>

struct aiMaterial;
struct Mesh;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* bearer);
	~ComponentMaterial();

	void Init();

	void LoadTexture(Mesh* mesh, aiMaterial* ai_material);

private:
	std::vector<uint> textures;
	std::vector<std::string> tex_paths;

};

#endif // !__COMPONENTMATERIAL_H__