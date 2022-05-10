#ifndef GUI_INTERNAL_GUARD
#define GUI_INTERNAL_GUARD
#include <cmath>
#include "GUI.h"
#include "renderLayer.h"
#include "paint.h"		 
#include "resource.h" 
#include "globals.h"  
#include "simplemath.h" 
#include "mouse.h"
#include "window.h"
#include "texture.h"
#include "fonts.h"
#include "timer.h" 
#include "global_vars.h"
using std::isnan;
struct GUI_options_kind{
	bool debug = false;
	bool push = false;
};
void GUIsetFavoriteRenderOptions();
struct gs_GUI_internalKind {
	GUI_options_kind g_GUIoptions;
};
class GUIbase;
GUIbase *GUI_constructor_dispatch(string S);
#define defaultarea rect(100,100)
#define defaultclientarea rect(100,100)
#define defaultbgcolor vec3{128,196,255}
#define defaultborderColor vec3{100,128,196}
#define defaulttextColor vec3{0,0,0}
#define defaulttextfont getFont("calibri 18")
#define defaulttext "text"
#define defaultimage 0
#endif 
