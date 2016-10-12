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

	//In a member function declaration or definition, override ensures that the function is virtual and is overriding a virtual function 
	//from the base class.The program is ill - formed(a compile - time error is generated) if this is not true. override is an identifier 
	//with a special meaning when used after member function declarators : it's not a reserved keyword otherwise.
	void Draw() override;

	void DrawApplication();
	void Log(const float* fps, const float ms);
	unsigned int GetMaxFPS();

};

#endif // __PANELCONFIGURATION_H__

