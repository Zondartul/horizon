#include "game.h"

void openGUI();

void initHorizonGUI() {
	printf("Hello World!\n");
	Gb = new GlobalsBtm();
	Gg = new GlobalsGui();
	auto& camera = Gb->gs_camera->g_camera;
	initUtil();
	window_init(512, 512);
	initInput();
	initFreeType();
	renderInit();
	loadAssets();
	camera.setPos({ -0.5,0,0 });
	setupLayers();
	addKeybinds();
	openGUI();
	printf("-------- frames begin ----------\n");
}

void tick();

int main(int argc, char **argv){
	initHorizonGUI();
	Gt = new GlobalsTop();
	auto& console = Gt->gs_console->g_console;
	console = new consoleKind();
	addConsoleCommands();
	while(1){
		tick();
	}
	return 0;
}
void tick(){
	auto& globalChannel = Gb->gs_event->g_globalChannel;
	//auto& eventTime = Gb->gs_main->g_eventTime;
	//auto& renderTime = Gb->gs_main->g_renderTime;
	//auto& sysmsgTime = Gb->gs_main->g_sysmsgTime;
	//auto& frametime = Gb->gs_main->g_frametime;
	//auto& eventTimeFlt = Gb->gs_main->g_eventTimeFlt;
	//auto& renderTimeFlt = Gb->gs_main->g_renderTimeFlt;
	//auto& sysmsgTimeFlt = Gb->gs_main->g_sysmsgTimeFlt;
	//auto& frametimeFlt = Gb->gs_main->g_frametimeFlt;
	//auto& mem1 = Gb->gs_main->g_mem1;
	//auto& mem2 = Gb->gs_main->g_mem2;
	eventKind e1; e1.type = EVENT_TICK;
	globalChannel->publishEvent(e1);
	eventKind e2; e2.type = EVENT_FRAME;
	globalChannel->publishEvent(e2);// profile(globalChannel->publishEvent(e2), eventTime);
	eventKind e3; e3.type = EVENT_CLEANUP;
	globalChannel->publishEvent(e3);
	renderTick();  
	sysMessageTick(); 
}
void openGUI(){
	auto& layerGUI = Gg->gs_GUI->g_layerGUI;
	auto& layer2D = Gb->gs_paint->g_layer2D;
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& inputChannel = Gb->gs_input->g_inputChannel;
	auto& globalChannel = Gb->gs_event->g_globalChannel;
	layerGUI = new renderLayer("GUI.layerGUI");
	addLayer(layer2D,layerGUI);
	GUI = new GUIbase();
	inputChannel->addListenerFront(GUI);
	globalChannel->addListenerFront(GUI);
}
