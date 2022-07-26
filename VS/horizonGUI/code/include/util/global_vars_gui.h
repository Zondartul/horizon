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

extern DLLAPI_GUI GlobalsGui *Gg;