#pragma once

class GlobalsTop {
public:
	GlobalsTop();
	struct gs_map_editorKind* gs_map_editor;
	struct gs_consoleKind* gs_console;
	//struct gs_commandsKind* gs_commands;
	struct gs_editor2Kind* gs_editor2;
	struct gs_tool_editKind* gs_tool_edit;
	struct gs_gui_editorKind* gs_gui_editor;
	struct gs_pickerKind* gs_picker;
};
class GlobalsGui {
public:
	GlobalsGui();
	struct gs_GUI_internalKind* gs_GUI_internal;
	struct gs_GUIrenderer_defaultKind* gs_GUIrenderer_default;
	struct gs_GUIKind* gs_GUI;
};
class GlobalsBtm {
public:
	GlobalsBtm();
	//struct gs_mainKind* gs_main;
	struct gs_renderLayerKind* gs_renderLayer;
	struct gs_paintKind* gs_paint;
	struct gs_windowKind* gs_window;
	//struct gs_timerKind* gs_timer;
	//struct gs_hookKind* gs_hook;
	class sysTimerKind* sysTimer;
	struct gs_cameraKind* gs_camera;
	//struct gs_eventKind* gs_event;
	class sysEventKind* sysEvent;
	//struct gs_inputKind* gs_input;
	class sysInputKind* sysInput;
	struct gs_renderKind* gs_render;
	struct gs_printwKind* gs_printw;
	//struct gs_keybindsKind* gs_keybinds;
	struct gs_resourceKind* gs_resource;
	//struct gs_debugKind* gs_debug;
};
extern GlobalsTop *Gt;
extern GlobalsGui *Gg;
extern GlobalsBtm *Gb;
