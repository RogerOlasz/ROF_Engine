#ifndef __PANELHIERARCHY_H__
#define __PANELHIERARCHY_H__

#include "Panel.h"
#include <vector>

class GameObject;

class PanelHierarchy : public Panel
{
public:

	PanelHierarchy();
	virtual ~PanelHierarchy();

	void Draw();

	GameObject* GetSelectedGO() const;

private:
	void SceneTreeGameObject(GameObject* node);

	GameObject* selected_go = nullptr;

};

#endif // __PANELHIERARCHY_H__

