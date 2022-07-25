#include "util/global_vars_render.h"
#include "render/paint.h"
#include "render/renderLayer.h"
#include "program/window.h"
#include "render/printw.h"
#include "render/renderLow.h"
#include "util/event.h"
#include "util/timer.h"
#include "render/camera.h"
class renderLowKind;

GlobalsRender* Gr = 0;
renderLowKind* g_renderLow;

GlobalsRender::GlobalsRender() {
	gs_paint = new gs_paintKind();
	gs_renderLayer = new gs_renderLayerKind();
	gs_window = new gs_windowKind();
	gs_printw = new gs_printwKind();
	gs_renderLow = new gs_renderLowKind();
	sysEvent = new sysEventKind();
	sysTimer = new sysTimerKind(*sysEvent);
	gs_camera = new gs_cameraKind();
}
