#pragma once
#include "util/globals_gui.h"

class DLLAPI_GUI GlobalsGui {
public:
	GlobalsGui();
	struct gs_GUI_internalKind* gs_GUI_internal;
	struct gs_GUIrenderer_defaultKind* gs_GUIrenderer_default;
	struct gs_GUIKind* gs_GUI;
	struct gs_render_guiKind* gs_render_gui;
	struct sysInputKind* sysInput;
};

//class DLLAPI_GUI GlobalsBtm {
//public:
//	GlobalsBtm();
//	//struct gs_mainKind* gs_main;
//	struct gs_renderLayerKind* gs_renderLayer;
//	struct gs_paintKind* gs_paint;
//	struct gs_windowKind* gs_window;
//	//struct gs_timerKind* gs_timer;
//	//struct gs_hookKind* gs_hook;
//	class sysTimerKind* sysTimer;
//	struct gs_cameraKind* gs_camera;
//	//struct gs_eventKind* gs_event;
//	class sysEventKind* sysEvent;
//	//struct gs_inputKind* gs_input;
//	class sysInputKind* sysInput;
//	struct gs_renderKind* gs_render;
//	struct gs_printwKind* gs_printw;
//	//struct gs_keybindsKind* gs_keybinds;
//	struct gs_resourceKind* gs_resource;
//	//struct gs_debugKind* gs_debug;
//};

extern GlobalsGui *Gg;
//extern GlobalsBtm *Gb;