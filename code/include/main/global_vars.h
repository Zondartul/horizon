#pragma once


//
//struct gs_tool_editKind;
//struct gs_gui_editorKind;
//struct gs_node_graphKind;
//struct gs_pickerKind;
//struct gs_debugKind;
//struct gs_renderLayerKind;
//struct gs_paintKind;
//struct gs_windowKind;
//struct gs_timerKind;
//
//struct gs_resourceKind;
//struct gs_mainKind;


//
//struct gs_renderKind;
//struct gs_printwKind;
//struct gs_commandsKind;
//struct gs_editor2Kind;
//struct gs_keybindsKind;
//struct gs_inputKind;
//struct gs_hookKind;
//struct gs_GUI_internalKind;
//struct gs_eventKind;
//struct gs_GUIrenderer_defaultKind;
//struct gs_cameraKind;
//struct gs_map_editorKind;
//struct gs_consoleKind;

class GlobalsTop {
public:
	GlobalsTop();

	struct gs_map_editorKind* gs_map_editor;
	struct gs_consoleKind* gs_console;
	struct gs_commandsKind* gs_commands;
	struct gs_editor2Kind* gs_editor2;
	struct gs_tool_editKind* gs_tool_edit;
	struct gs_gui_editorKind* gs_gui_editor;
	struct gs_pickerKind* gs_picker;
};

class GlobalsGui {
public:
	GlobalsGui();
	//struct gs_ecsKind *gs_ecs;
	//struct gs_entityKind* gs_entity;
	
	struct gs_GUI_internalKind* gs_GUI_internal;
	struct gs_GUIrenderer_defaultKind* gs_GUIrenderer_default;
};

class GlobalsBtm {
public:
	GlobalsBtm();

	struct gs_mainKind* gs_main;
	struct gs_renderLayerKind* gs_renderLayer;
	struct gs_paintKind* gs_paint;
	struct gs_windowKind* gs_window;
	struct gs_timerKind* gs_timer;
	struct gs_cameraKind* gs_camera;
	struct gs_eventKind* gs_event;
	struct gs_inputKind* gs_input;
	struct gs_hookKind* gs_hook;
	struct gs_renderKind* gs_render;
	struct gs_printwKind* gs_printw;
	struct gs_keybindsKind* gs_keybinds;
	struct gs_resourceKind* gs_resource;
	struct gs_debugKind* gs_debug;
};

extern GlobalsTop *Gt;
extern GlobalsGui *Gg;
extern GlobalsBtm *Gb;

