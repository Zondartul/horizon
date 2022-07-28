#include "main/framework_gui.h"
//for initHorizonGUI:
#include "main/main_gui.h"
#include "main/main_render.h"
#include "util/global_vars_gui.h"
#include "util/global_vars_util.h"
#include "util/global_vars_program.h"
#include "util/global_vars_render.h"
//for openGUI:
#include "GUI/GUI.h"
#include "render/paint.h"
#include "input/input.h"
#include "util/event.h"
#include "render/renderLayer.h"
#include "resource/resource.h"
#include "GUI/GUI_internal.h"
//---
#include <iostream>
using namespace std;
//void openGUI();

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

//void initHorizonSystem() {
//	//Gb = (GlobalsBtm*)malloc(sizeof(GlobalsBtm));
//	//new(Gb) GlobalsBtm();
//
//	Gr = new GlobalsRender();
//	//initUtil();
//	//initInput();
//	//addKeybinds();
//	initFreeType();
//
//	window_init(512, 512);
//	renderInit();
//	setupLayers();
//	loadAssets();
//}

void initHorizonGUI() {
	static bool initialized = false;
	if (initialized) {return;}
	initialized = true;

	initHorizonRender();
	struct font *f = getFont("cour 14");
	uploadFont(f);
	uploadFont(defaulttextfont);
	//printf("Hello World!\n");
	cout << "Hello World!" << endl;
	Gg = new GlobalsGui();
	//openGUI();
}


void openGUI() {
	auto& layerGUI = Gg->gs_GUI->g_layerGUI;
	auto& layer2D = Gr->gs_paint->g_layer2D;
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& inputChannel = Gp->sysInput->inputChannel;
	auto& globalChannel = Gu->sysEvent->globalChannel;
	layerGUI = new renderLayer("GUI.layerGUI");
	addLayer(layer2D, layerGUI);
	GUI = new GUIbase();
	inputChannel.addListenerFront(GUI);
	globalChannel.addListenerFront(GUI);
}
