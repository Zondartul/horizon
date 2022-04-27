#pragma once

struct gs_ecsKind;
struct gs_entityKind;
struct gs_tool_editKind;
struct gs_gui_editorKind;
struct gs_node_graphKind;
struct gs_pickerKind;
struct gs_debugKind;
struct gs_renderLayerKind;
struct gs_paintKind;
struct gs_windowKind;
struct gs_timerKind;
struct gs_physboxKind;
struct gs_resourceKind;
struct gs_mainKind;
struct gs_narrowphaseKind;
struct gs_modelLoaderKind;
struct gs_renderKind;
struct gs_printwKind;
struct gs_commandsKind;
struct gs_editor2Kind;
struct gs_keybindsKind;
struct gs_inputKind;
struct gs_hookKind;
struct gs_GUI_internalKind;
struct gs_eventKind;
struct gs_GUIrenderer_defaultKind;
struct gs_cameraKind;
struct gs_map_editorKind;
struct gs_frameprinterKind;

class Globals {
public:
	Globals();
	gs_ecsKind *gs_ecs;
	gs_entityKind* gs_entity;
	gs_tool_editKind* gs_tool_edit;
	gs_gui_editorKind* gs_gui_editor;
	gs_node_graphKind* gs_node_graph;
	gs_pickerKind* gs_picker;
	gs_debugKind* gs_debug;
	gs_renderLayerKind* gs_renderLayer;
	gs_paintKind* gs_paint;
	gs_windowKind* gs_window;
	gs_timerKind* gs_timer;
	gs_physboxKind* gs_physbox;
	gs_resourceKind* gs_resource;
	gs_mainKind* gs_main;
	gs_narrowphaseKind* gs_narrowphase;
	gs_modelLoaderKind* gs_modelLoader;
	gs_renderKind* gs_render;
	gs_printwKind* gs_printw;
	gs_commandsKind* gs_commands;
	gs_editor2Kind* gs_editor2;
	gs_keybindsKind* gs_keybinds;
	gs_inputKind* gs_input;
	gs_hookKind* gs_hook;
	gs_GUI_internalKind* gs_GUI_internal;
	gs_eventKind* gs_event;
	gs_GUIrenderer_defaultKind* gs_GUIrenderer_default;
	gs_cameraKind* gs_camera;
	gs_map_editorKind* gs_map_editor;
	gs_frameprinterKind* gs_frameprinter;
	gs_inputControllerKind* gs_inputController;
};

extern Globals *G;