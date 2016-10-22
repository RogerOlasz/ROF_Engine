#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

#include "Panel.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class PanelConfiguration : public Panel
{
private:
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	int max_fps = 60;

	float brightness;
	float gamma;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;

	vec cam_pos;
	float near_plane;
	float far_plane;
	float field_of_view;
	float aspect_ratio;

public:

	PanelConfiguration();
	virtual ~PanelConfiguration();

	void Draw();

	void DrawWindow();
	void DrawIlumination();
	void DrawTecnology();
	void DrawEditorCamera();
	void DrawApplication();	
	
	void Log(const float* fps, const float ms);
	unsigned int GetMaxFPS();

};

#endif // __PANELCONFIGURATION_H__

