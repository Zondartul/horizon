#include "util/global_vars_gui.h"
#include "input/input.h"
#include "render/renderLayer.h" 
#include "render/paint.h" 
#include "program/window.h" 
#include "util/timer.h" 
#include "resource/resource.h" 
#include "render/render.h" 
#include "render/printw.h" 
#include "input/keybinds.h" 
#include "GUI/GUI_internal.h" 
#include "util/event.h" 
#include "GUI/GUIrenderer_default.h" 
#include "GUI/GUI.h"

GlobalsGui* Gg = 0;

GlobalsGui::GlobalsGui() {
	gs_GUI_internal = new gs_GUI_internalKind();
	gs_GUIrenderer_default = new gs_GUIrenderer_defaultKind();
	gs_GUI = new gs_GUIKind();
	sysInput = new sysInputKind();
}


// now how do we change these globals into organized systems?
/*
*  --- apps -------             
*  [texbrowser] [gui_editor] [map_editor] [model_editor]
* ---------------- horizonGUI.dll
*  Gui
*   [GUI]
* ---------------- horizonRender.dll
*  RenderHigh
*   [camera //future scene stuff]
*  RenderMid
*   [renderlayer, paint, printw]
*  RenderLow
*   [render]
*  OStools
*   [ util: debug]
*   [ input: keybinds, input]
*   [ event: hook, event, timer]
*   [ window,  resource] 
*   [//also [file] but it has no globals]
* 
	//-- renderLayer
		vector<renderLayer*> g_all_layers;
		vector<renderLayer*> g_layers;
	//-- paint
		renderLayer* g_loadLayer;
		renderLayer* g_layer3D;
		renderLayer* g_layerDebug;
		renderLayer* g_layer2D;
		renderLayer* g_layerDebug2D;
		renderLayer* g_deleteLayer;
		renderLayer* g_currentLayer = 0;
		rmodel* g_rm_unitboxWF = 0;
		rmodel* g_rm_unitcyllinderWF = 0;
		rmodel* g_rm_unitconeWF = 0;
		rmodel* g_rm_unitsphereWF = 0;
		rmodel* g_rm_unitbox = 0;
		rmodel* g_rm_unitcyllinder = 0;
		rmodel* g_rm_unitcone = 0;
		rmodel* g_rm_unitsphere = 0;
		bool g_printAllPending = false;
		bool g_frameReportPending = false;
	//-- printw
		bool g_printFromTop = false;
	//-- window
		int g_height;
		int g_width;
		map<char, char> g_keyboardMap
		map<string, bool> g_keyboardState;
		SDL_Window* g_mainWindow;
		SDL_GLContext g_mainContext;
	//-- 
*/