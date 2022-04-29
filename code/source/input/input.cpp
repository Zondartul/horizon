#include "input.h"
#include "inputController.h"
#include "keybinds.h"

//eventChannel *g_inputChannel;

void initInput(){
	auto& inputChannel = G->gs_input->g_inputChannel;
	auto& inputController = G->gs_inputController->g_inputController;

	inputChannel = new eventChannel();
	inputController = new inputControllerKind();
	
	initKeybinds();
	printf("-------- input init done ----------\n");
}