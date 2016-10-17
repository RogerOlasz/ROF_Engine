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

	float brightness = 1.0f;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;

public:

	PanelConfiguration();
	virtual ~PanelConfiguration();

	void Draw();

	//TODO may add a camera controller with three axis and some more features
	void DrawWindow();
	void DrawIlumination();
	void DrawTecnology();
	void DrawApplication();
	
	void Log(const float* fps, const float ms);
	unsigned int GetMaxFPS();

};

#endif // __PANELCONFIGURATION_H__

