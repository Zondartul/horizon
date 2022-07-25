#ifndef GUI_GUARD
#define GUI_GUARD
#include "util/event.h" 
#include "util/util.h"  
#include "util/elastic_ptr.h"
#include "GUI/GUI_types.h" 
#include "GUI/GUI_property.h"
#include "GUI/GUIrenderer.h"
#include "GUI/GUIrenderer_default.h"
#include "GUI/GUIbase.h"
#include "GUI/GUIframe.h"
#include "GUI/GUIgrid.h"
#include "GUI/GUIlabel.h"
#include "GUI/GUIimage.h"
#include "GUI/GUIbutton.h"
#include "GUI/GUIscrollbar.h"
#include "GUI/GUItextentry.h"
#include "GUI/GUIwindow.h"
#include "GUI/GUIselectable.h"
#include "GUI/GUIdropdownMenu.h"
#include <map>
#include <string>
#include <vector>
#include <functional>
using std::map;
using std::string;
using std::vector;
using std::function;

struct gs_GUIKind {
	class GUIbase* g_GUI;
	class renderLayer* g_layerGUI;
};

#endif
