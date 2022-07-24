#pragma once
struct gs_paintKind;
struct gs_renderLayerKind;
struct gs_windowKind;
struct gs_printwKind;
struct gs_renderLowKind;

struct GlobalsRender {
	GlobalsRender();

	struct gs_paintKind* gs_paint;
	struct gs_renderLayerKind* gs_renderLayer;
	struct gs_windowKind* gs_window;
	struct gs_printwKind* gs_printw;
	struct gs_renderLowKind* gs_renderLow;
};

extern GlobalsRender* Gr;