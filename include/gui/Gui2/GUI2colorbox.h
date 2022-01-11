#ifndef GUI2COLORBOX_GUARD
#define GUI2COLORBOX_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2colorbox: public GUI2base
{
	public:
	vec3i colorHSV;
	vec3i colorRGB;
	vec3i colLU;
	vec3i colRU;
	vec3i colLD;
	vec3i colRD;
	vec2i cursor;
	GUI2colorbox();
	void onClick(int mb);
	void render(void *arg);
};
#endif