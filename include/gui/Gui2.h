#ifndef GUI2_GUARD
#define GUI2_GUARD

#include <string>
#include <cstdio>
#include <math.h>
#include "../util/messenger.h"
#include "../util/globals.h"
using std::string;
#include "Gui2/GUI2base.h"
#include "Gui2/GUI2button.h"
#include "Gui2/GUI2checkbox.h"
#include "Gui2/GUI2colorbox.h"
#include "Gui2/GUI2dropdownlist.h"
#include "Gui2/GUI2frame.h"
#include "Gui2/GUI2label.h"
#include "Gui2/GUI2listbox.h"
#include "Gui2/GUI2radiobutton.h"
#include "Gui2/GUI2radiogroup.h"
#include "Gui2/GUI2scrollBar.h"
#include "Gui2/GUI2scrollslidey.h"
#include "Gui2/GUI2slider.h"
#include "Gui2/GUI2spinner.h"
#include "Gui2/GUI2textEntry.h"
#include "Gui2/GUI2valuedisplay.h"
 

extern color3i paintColor; //???
extern byte paintAlpha;
extern vec2i mouseP; // mid-man variable for extra GUI's
//void setColor(color3i color);
//void setAlpha(byte alpha);

// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
vec3i RGBtoHSV(vec3i RGB);
vec3i HSVtoRGB(vec3i HSV);
#endif