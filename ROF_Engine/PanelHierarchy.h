#ifndef __PANELHIERARCHY_H__
#define __PANELHIERARCHY_H__

#include "Panel.h"
#include <vector>

class PanelHierarchy : public Panel
{
public:

	PanelHierarchy();
	virtual ~PanelHierarchy();

	void Draw() override;

};

#endif // __PANELHIERARCHY_H__

