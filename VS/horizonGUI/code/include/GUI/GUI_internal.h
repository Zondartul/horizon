#ifndef GUI_INTERNAL_GUARD
#define GUI_INTERNAL_GUARD
#include "GUI/GUI.h"
#include "render/renderLayer.h"
#include "render/paint.h"		 
#include "resource/resource.h" 
#include "util/globals_gui.h"  
#include "math/simplemath.h" 
#include "input/mouse.h"
#include "program/window.h"
#include "resource/texture.h"
#include "resource/fonts.h"
#include "util/timer.h" 
#include "util/globals_gui.h"
#include "util/global_vars_gui.h"
#include "util/global_vars_render.h"
#include "util/stringUtils.h"
#include <cmath>
using std::isnan;

struct DLLAPI_GUI GUI_options_kind{
	bool debug = false;
	bool push = false;
};

void DLLAPI_GUI GUIsetFavoriteRenderOptions();

struct DLLAPI_GUI gs_GUI_internalKind {
	GUI_options_kind g_GUIoptions;
};

class GUIbase;

GUIbase DLLAPI_GUI *GUI_constructor_dispatch(string S);

string DLLAPI_GUI toString(GUIe_alignment al); 
template<> DLLAPI_GUI GUIe_alignment fromString<GUIe_alignment>(const string S);


#define defaultarea rect(100,100)
#define defaultclientarea rect(100,100)
#define defaultbgcolor vec3{128,196,255}
#define defaultborderColor vec3{100,128,196}
#define defaulttextColor vec3{0,0,0}
#define defaulttextfont getFont("calibri 18")
#define defaulttext "text"
#define defaultimage 0

#endif 
