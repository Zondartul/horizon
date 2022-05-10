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

Globals* G = 0;

Globals::Globals() {


	gs_tool_edit = new gs_tool_editKind();
	gs_gui_editor = new gs_gui_editorKind();

	gs_picker = new gs_pickerKind();
	gs_debug = new gs_debugKind();
	gs_renderLayer = new gs_renderLayerKind();
	gs_paint = new gs_paintKind();
	gs_window = new gs_windowKind();
	gs_timer = new gs_timerKind();

	gs_resource = new gs_resourceKind();
	gs_main = new gs_mainKind();



	gs_render = new gs_renderKind();
	gs_printw = new gs_printwKind();
	gs_commands = new gs_commandsKind();
	gs_editor2 = new gs_editor2Kind();
	gs_keybinds = new gs_keybindsKind();
	gs_input = new gs_inputKind();
	gs_hook = new gs_hookKind();
	gs_GUI_internal = new gs_GUI_internalKind();
	gs_event = new gs_eventKind();
	gs_GUIrenderer_default = new gs_GUIrenderer_defaultKind();
	gs_camera = new gs_cameraKind();
	gs_map_editor = new gs_map_editorKind();
	
	
	gs_console = new gs_consoleKind();
}
