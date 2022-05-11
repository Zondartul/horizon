#include "global_vars.h"
#include "tool_edit.h" 
#include "gui_editor.h" 
#include "picker.h" 
#include "debug.h" 
#include "renderLayer.h" 
#include "paint.h" 
#include "window.h" 
#include "timer.h" 
#include "resource.h" 
#include "render.h" 
#include "printw.h" 
#include "commands.h" 
#include "editor2.h" 
#include "keybinds.h" 
#include "hook.h" 
#include "GUI_internal.h" 
#include "event.h" 
#include "GUIrenderer_default.h" 
#include "camera.h" 
#include "map_editor.h" 
#include "console.h"
#include "GUI.h"

GlobalsTop* Gt = 0;
GlobalsGui* Gg = 0;
GlobalsBtm* Gb = 0;
GlobalsTop::GlobalsTop() {
	gs_map_editor = new gs_map_editorKind();
	gs_console = new gs_consoleKind();
	gs_commands = new gs_commandsKind();
	gs_editor2 = new gs_editor2Kind();
	gs_tool_edit = new gs_tool_editKind();
	gs_gui_editor = new gs_gui_editorKind();
	gs_picker = new gs_pickerKind();
}
GlobalsGui::GlobalsGui() {
	gs_GUI_internal = new gs_GUI_internalKind();
	gs_GUIrenderer_default = new gs_GUIrenderer_defaultKind();
	gs_GUI = new gs_GUIKind();
}
GlobalsBtm::GlobalsBtm() {
	gs_debug = new gs_debugKind();
	gs_renderLayer = new gs_renderLayerKind();
	gs_paint = new gs_paintKind();
	gs_window = new gs_windowKind();
	//gs_event = new gs_eventKind();
	sysEvent = new sysEventKind();
	//gs_timer = new gs_timerKind();
	sysTimer = new sysTimerKind(*sysEvent);
	gs_resource = new gs_resourceKind();
	gs_render = new gs_renderKind();
	gs_printw = new gs_printwKind();
	gs_keybinds = new gs_keybindsKind();
	gs_input = new gs_inputKind();
	//gs_hook = new gs_hookKind();
	gs_camera = new gs_cameraKind();
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