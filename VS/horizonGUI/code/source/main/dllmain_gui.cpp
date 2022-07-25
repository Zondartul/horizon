#include "main/framework_gui.h"
//for initHorizonSystem:
#include "loaders/fontLoader.h"
#include "program/window.h"
#include "render/paint.h"
#include "render/render.h"
#include "render/render_gui.h"
#include "render/renderLayer.h"
#include "resource/resource.h"
//for initHorizonGUI:
#include "GUI/GUI.h"
#include "input/input.h"
#include "util/global_vars_gui.h"
#include "util/global_vars_render.h"
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

	Gr = new GlobalsRender();
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
	auto& camera = Gr->gs_camera->g_camera;
	camera.setPos({ -0.5,0,0 });
	openGUI();
}

void openGUI() {
	auto& layerGUI = Gg->gs_GUI->g_layerGUI;
	auto& layer2D = Gr->gs_paint->g_layer2D;
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& inputChannel = Gg->sysInput->inputChannel;//Gb->gs_input->g_inputChannel;
	auto& globalChannel = Gr->sysEvent->globalChannel;//Gb->gs_event->g_globalChannel;
	layerGUI = new renderLayer("GUI.layerGUI");
	addLayer(layer2D, layerGUI);
	GUI = new GUIbase();
	inputChannel.addListenerFront(GUI);
	globalChannel.addListenerFront(GUI);
}
