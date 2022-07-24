#include "util/global_vars_render.h"
#include "render/paint.h"
#include "render/renderLayer.h"
#include "program/window.h"
#include "render/printw.h"
#include "render/renderLow.h"
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
