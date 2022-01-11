#ifndef GUI2SLIDER_GUARD
#define GUI2SLIDER_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2slider: public GUI2base
{
	public:
	//double vals[3];
	vector<double> vals;
	bool sliding;
	GUI2slider();
	void render(void *arg);
	void onClick(int mb);
	void updateSlider();
};
#endif