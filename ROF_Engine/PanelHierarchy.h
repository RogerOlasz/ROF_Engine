#ifndef __PANELHIERARCHY_H__
#define __PANELHIERARCHY_H__

#include "Panel.h"
#include <vector>

class GameObject;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy();
	~PanelHierarchy();

	void Draw();

	GameObject* GetSelectedGO() const;
	void SetSelectedGO(GameObject* go);

private:
	void GameObjectsHierarchy(GameObject* node);

	GameObject* selected_go = nullptr;

};

#endif // __PANELHIERARCHY_H__

