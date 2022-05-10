#include "game.h"



















































































int main(int argc, char **argv){
	printf("Hello World!\n");
	G = new Globals();

	auto& camera = G->gs_camera->g_camera;
	auto& console = G->gs_console->g_console;


	initUtil();
	window_init(512,512);
	initInput();
	initFreeType();
	renderInit();
	loadAssets();

	camera.setPos({-0.5,0,0});
	
	setupLayers();
	addKeybinds();

	console = new consoleKind();
	addConsoleCommands();

	openGUI();

	printf("-------- frames begin ----------\n");

	while(1){
		tick();
	}
	return 0;
}

void tick(){
	auto& globalChannel = G->gs_event->g_globalChannel;
	
	auto& eventTime = G->gs_main->g_eventTime;
	auto& renderTime = G->gs_main->g_renderTime;
	auto& sysmsgTime = G->gs_main->g_sysmsgTime;
	auto& frametime = G->gs_main->g_frametime;
	auto& eventTimeFlt = G->gs_main->g_eventTimeFlt;
	auto& renderTimeFlt = G->gs_main->g_renderTimeFlt;
	auto& sysmsgTimeFlt = G->gs_main->g_sysmsgTimeFlt;
	auto& frametimeFlt = G->gs_main->g_frametimeFlt;
	auto& mem1 = G->gs_main->g_mem1;
	auto& mem2 = G->gs_main->g_mem2;

	eventKind e1; e1.type = EVENT_TICK;
	globalChannel->publishEvent(e1);
		
	eventKind e2; e2.type = EVENT_FRAME;
	profile(globalChannel->publishEvent(e2),eventTime);
	
	eventKind e3; e3.type = EVENT_CLEANUP;
	globalChannel->publishEvent(e3);
	
	renderTick();  
	sysMessageTick(); 
}

void openGUI(){
	auto& layerGUI = G->gs_main->g_layerGUI;
	auto& layer2D = G->gs_paint->g_layer2D;
	auto& GUI = G->gs_main->g_GUI;
	auto& inputChannel = G->gs_input->g_inputChannel;
	auto& globalChannel = G->gs_event->g_globalChannel;

	layerGUI = new renderLayer("GUI.layerGUI");
	addLayer(layer2D,layerGUI);
	GUI = new GUIbase();
	inputChannel->addListenerFront(GUI);
	globalChannel->addListenerFront(GUI);
}
