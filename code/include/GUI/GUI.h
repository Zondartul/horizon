#ifndef GUI_GUARD
#define GUI_GUARD
//=============================
// Horizon GUI widget toolkit
// see readme_GUI.h for details
// include this file, and only
// this file, to use the GUI.
//=============================
// GUI header files should not
// be included on their own.
// GUI source files should
// include GUI_internal.h.
//-----------------------------
#include <map>
#include <string>
#include <vector>
#include <functional>
using std::map;
using std::string;
using std::vector;
using std::function;

//external dependencies
#include "event.h" 
#include "util.h"  //for enum ops 
#include "elastic_ptr.h"
//internal files
#include "GUI_types.h" //border
#include "GUI_property.h"
#include "GUIrenderer.h"
#include "GUIrenderer_default.h"
#include "GUIbase.h"
#include "GUIframe.h"
#include "GUIgrid.h"
#include "GUIlabel.h"
#include "GUIimage.h"
#include "GUIbutton.h"
#include "GUIscrollbar.h"
#include "GUItextentry.h"
#include "GUIwindow.h"
#include "GUIselectable.h"
#include "GUIdropdownMenu.h"

#endif
