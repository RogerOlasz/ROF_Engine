#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

#include "Panel.h"
#include <vector>

class PanelConfiguration : public Panel
{
private:
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	int max_fps = 60;

public:

	PanelConfiguration();
	virtual ~PanelConfiguration();

	void Draw();

	void DrawApplication();
	void Log(const float* fps, const float ms);
	unsigned int GetMaxFPS();

};

#endif // __PANELCONFIGURATION_H__

