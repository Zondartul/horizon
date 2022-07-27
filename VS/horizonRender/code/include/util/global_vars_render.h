#pragma once
#include "globals_render.h"
//struct gs_paintKind;
//struct gs_renderLayerKind;
//struct gs_windowKind;
//struct gs_printwKind;
//struct gs_renderLowKind;
//class sysTimerKind;

struct DLLAPI_RENDER GlobalsRender {
	GlobalsRender();

	struct gs_paintKind* gs_paint;
	struct gs_renderLayerKind* gs_renderLayer;
	//struct gs_windowKind* gs_window;
	struct gs_printwKind* gs_printw;
	struct gs_renderLowKind* gs_renderLow;

	//random stuff
	//class sysEventKind* sysEvent;
	//class sysTimerKind* sysTimer;
	struct gs_cameraKind* gs_camera; 
	//struct sysInputKind* sysInput; 
};

extern DLLAPI_RENDER GlobalsRender* Gr;