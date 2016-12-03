#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Globals.h"
#include "Component.h"
#include "Color.h"
#include <string>

struct aiMaterial;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* bearer, int id);
	~ComponentMaterial();

	void OnSave(pugi::xml_node&);
	void OnLoad(pugi::xml_node&);

	void SetMaterialColor(float r, float g, float b, float a = 1.0f);
	void SetTextureId(uint id);
	void AppendTexturePath(const char* path);
	void SetTexturePath(const char* path);

	uint GetTextureId() const;
	const char* GetTexturePath() const;
	std::string GetTexturePathS() const;
	const char* GetAdaptedTexturePath() const;
	Color GetMaterialColor() const;
	
private:
	uint texture_id = 0;
	Color color = White;
	std::string tex_path = "";

	//TODO temporaly public
public:
	std::string path = "";
};

#endif // !__COMPONENTMATERIAL_H__