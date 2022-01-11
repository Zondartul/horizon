#ifndef GUI2SPINNER_GUARD
#define GUI2SPINNER_GUARD
#include "gui/Gui2/GUI2base.h"
#include "gui/Gui2/GUI2button.h"
class GUI2spinner: public GUI2base
{
	public:
	double vals[5];// min - cur - max - speed - precision
	GUI2button* btnUp;
	GUI2button* btnDown;
	void (*func)(void*);
	void* arg;
	string text;
	
	static void fUp(void* arg);
	static void fDown(void* arg);
	GUI2spinner();
	void setVals(double a,double b,double c,double d,double e);
	void invalidate(vec2i newPos, vec2i newSize);
	void render(void* arg);
};
#endif