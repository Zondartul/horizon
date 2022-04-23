#ifndef GUI_INTERNAL_GUARD
#define GUI_INTERNAL_GUARD

//=======================================================
// Used internally by GUI files as base include.
//
// Clients: to use GUI in your project,
// #include "GUI.h" instead.
//=======================================================
#include "GUI.h"
#include "renderLayer.h"
#include "paint.h"		 
#include "resource.h" 
#include "globals.h"  //for error()
#include "simplemath.h" //for max()
#include "mouse.h"
#include "window.h"
#include "texture.h"
#include "fonts.h"
#include "timer.h" //for getRealTime()
#include <cmath>
using std::isnan;

struct GUI_options_kind{
	bool debug = false;
	bool push = false;
};
extern GUI_options_kind g_GUIoptions;
void GUIsetFavoriteRenderOptions();










class GUIbase;
//creates a gui widget by class name
GUIbase *GUI_constructor_dispatch(string S);


#define defaultarea rect(100,100)
#define defaultclientarea rect(100,100)
#define defaultbgcolor vec3{128,196,255}
#define defaultborderColor vec3{100,128,196}
#define defaulttextColor vec3{0,0,0}
#define defaulttextfont getFont("calibri 18")
#define defaulttext "text"
#define defaultimage 0


#endif // GUI_INTERNAL_GUARD
