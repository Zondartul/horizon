#include "framework.h"
//for initHorizonSystem:
#include "global_vars_gui.h"
#include "fontLoader.h"
#include "window.h"
#include "render.h"
#include "renderLayer.h"
#include "resource.h"
//for initHorizonGUI:
#include "GUI.h"
#include "paint.h"
#include "input.h"
void openGUI();

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void initHorizonSystem() {
	//Gb = (GlobalsBtm*)malloc(sizeof(GlobalsBtm));
	//new(Gb) GlobalsBtm();

	Gb = new GlobalsBtm();
	//initUtil();
	//initInput();
	//addKeybinds();
	initFreeType();

	window_init(512, 512);
	renderInit();
	setupLayers();
	loadAssets();
}

void initHorizonGUI() {
	printf("Hello World!\n");
	Gg = new GlobalsGui();
	auto& camera = Gb->gs_camera->g_camera;
	camera.setPos({ -0.5,0,0 });
	openGUI();
}

void openGUI() {
	auto& layerGUI = Gg->gs_GUI->g_layerGUI;
	auto& layer2D = Gb->gs_paint->g_layer2D;
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& inputChannel = Gb->sysInput->inputChannel;//Gb->gs_input->g_inputChannel;
	auto& globalChannel = Gb->sysEvent->globalChannel;//Gb->gs_event->g_globalChannel;
	layerGUI = new renderLayer("GUI.layerGUI");
	addLayer(layer2D, layerGUI);
	GUI = new GUIbase();
	inputChannel.addListenerFront(GUI);
	globalChannel.addListenerFront(GUI);
}
