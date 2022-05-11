#include "global_vars.h"
#include "paint.h"
#include "renderLayer.h"
#include "window.h"
#include "printw.h"
#include "renderLow.h"
class renderLowKind;

GlobalsRender* G = 0;
renderLowKind* g_renderLow;

GlobalsRender::GlobalsRender() {
	gs_paint = new gs_paintKind();
	gs_renderLayer = new gs_renderLayerKind();
	gs_window = new gs_windowKind();
	gs_printw = new gs_printwKind();
	gs_renderLow = new gs_renderLowKind();
}
