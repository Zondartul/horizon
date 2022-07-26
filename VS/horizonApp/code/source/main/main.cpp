#include "game/game.h"
#include "render/render_gui.h" //for renderTick
#include "util/global_vars_app.h"
#include "util/global_vars_gui.h" //for globalChannel
#include "util/global_vars_render.h"
#include "main/main_gui.h" //for init stuff

//void initHorizonSystem() {
//	//Gb = (GlobalsBtm*)malloc(sizeof(GlobalsBtm));
//	//new(Gb) GlobalsBtm();
//
//	Gb = new GlobalsBtm();
//	//initUtil();
//	//initInput();
//	addKeybinds();
//	initFreeType();
//
//	window_init(512, 512);
//	renderInit();
//	setupLayers();
//	loadAssets();
//}

//void initHorizonGUI() {
//	printf("Hello World!\n");
//	Gg = new GlobalsGui();
//	auto& camera = Gb->gs_camera->g_camera;
//	camera.setPos({ -0.5,0,0 });
//	openGUI();
//}

void initTop() {
	Ga = new GlobalsApp();
	auto& console = Ga->gs_console->g_console;
	console = new consoleKind();
	addConsoleCommands();
}

void tick();

int main(int argc, char **argv){
	initHorizonSystem();
	initHorizonGUI();
	initTop();
	while(1){
		tick();
	}
	return 0;
}

void tick(){
	auto& globalChannel = Gr->sysEvent->globalChannel;//Gb->gs_event->g_globalChannel;
	//eventKind e1; e1.type = EVENT_TICK;
	globalChannel.publishEvent(eventKind(EVENT_TICK));//(e1);
	//eventKind e2; e2.type = EVENT_FRAME;
	globalChannel.publishEvent(eventKind(EVENT_FRAME));//(e2);
	//eventKind e3; e3.type = EVENT_CLEANUP;
	globalChannel.publishEvent(eventKind(EVENT_CLEANUP));//(e3);
	renderTick();  
	sysMessageTick(); 
}
//void openGUI(){
//	auto& layerGUI = Gg->gs_GUI->g_layerGUI;
//	auto& layer2D = Gb->gs_paint->g_layer2D;
//	auto& GUI = Gg->gs_GUI->g_GUI;
//	auto& inputChannel = Gb->sysInput->inputChannel;//Gb->gs_input->g_inputChannel;
//	auto& globalChannel = Gb->sysEvent->globalChannel;//Gb->gs_event->g_globalChannel;
//	layerGUI = new renderLayer("GUI.layerGUI");
//	addLayer(layer2D,layerGUI);
//	GUI = new GUIbase();
//	inputChannel.addListenerFront(GUI);
//	globalChannel.addListenerFront(GUI);
//}
