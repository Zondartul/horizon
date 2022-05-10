#include "global_vars.h"
//#include "ecs.h" //gs_ecsKind
//#include "entity.h" //gs_entityKind
#include "tool_edit.h" //gs_tool_editKind
#include "gui_editor.h" //gs_tool_editorKind
//#include "node_graph.h" //gs_node_graphKind
#include "picker.h" //gs_pickerKind
#include "debug.h" //gs_debugKind
#include "renderLayer.h" //gs_renderLayerKind
#include "paint.h" //gs_paintKind
#include "window.h" //gs_windowKind
#include "timer.h" //gs_timerKind
//#include "physbox.h" //gs_physboxKind
#include "resource.h" //gs_resourceKind
//#include "narrowphase.h" //gs_narrowphaseKind
//#include "modelLoader.h" //gs_modelLoaderKind
#include "render.h" //gs_renderKind
#include "printw.h" //gs_printwKind
#include "commands.h" //gs_commandsKind
#include "editor2.h" //gs_editor2Kind
#include "keybinds.h" //gs_keybindsKind
#include "hook.h" //gs_hookKind
#include "GUI_internal.h" //gs_GUI_internalKind
#include "event.h" //gs_eventKind
#include "GUIrenderer_default.h" //gs_GUIrenderer_defaultKind
#include "camera.h" //gs_cameraKind
#include "map_editor.h" //gs_map_editorKind
#include "console.h"

Globals* G = 0;

Globals::Globals() {
//	gs_ecs = new gs_ecsKind();
//	gs_entity = new gs_entityKind();
	gs_tool_edit = new gs_tool_editKind();
	gs_gui_editor = new gs_gui_editorKind();
//	gs_node_graph = new gs_node_graphKind();
	gs_picker = new gs_pickerKind();
	gs_debug = new gs_debugKind();
	gs_renderLayer = new gs_renderLayerKind();
	gs_paint = new gs_paintKind();
	gs_window = new gs_windowKind();
	gs_timer = new gs_timerKind();
//	gs_physbox = new gs_physboxKind();
	gs_resource = new gs_resourceKind();
	gs_main = new gs_mainKind();
//	gs_narrowphase = new gs_narrowphaseKind();
//	gs_broadphase = new gs_broadphaseKind();
//	gs_modelLoader = new_gs_modelLoaderKind();
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
	//gs_frameprinter = new gs_frameprinterKind();
	//gs_inputController = new gs_inputControllerKind();
	gs_console = new gs_consoleKind();
}
