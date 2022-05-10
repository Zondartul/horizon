#ifndef MAIN_GUARD
#define MAIN_GUARD
#include "GUI.h"
void tick();
void openGUI();
struct gs_mainKind {
	float g_eventTime;
	float g_renderTime;
	float g_sysmsgTime;
	float g_eventTimeFlt = 0;
	float g_renderTimeFlt = 0;
	float g_sysmsgTimeFlt = 0;
	float g_frametimeFlt = 1.f;
	model* g_m;
	GUIbase* g_GUI = 0;
	float g_frametime = 0;
	float g_fps = 0;
	bool g_gamePaused = false;
	bool g_printf_enabled = true;
	void* g_mem1 = 0;
	void* g_mem2 = 0;
	uint32_t g_audio_len = 0;
	uint8_t* g_audio_pos = 0;
	renderLayer* g_layerGUI;
};
#endif