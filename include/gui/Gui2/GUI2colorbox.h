#ifndef GUI2COLORBOX_GUARD
#define GUI2COLORBOX_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2colorbox: public GUI2base
{
	public:
	color3i colorHSV;
	color3i colorRGB;
	color3i colLU;
	color3i colRU;
	color3i colLD;
	color3i colRD;
	vec2i cursor;
	GUI2colorbox();
	void onClick(int mb);
	void render(void *arg);
};
#endif