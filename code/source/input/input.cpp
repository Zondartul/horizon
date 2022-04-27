#include "input.h"
#include "inputController.h"
#include "keybinds.h"

//eventChannel *g_inputChannel;

void initInput(){
	g_inputChannel = new eventChannel();
	g_inputController = new inputControllerKind();
	
	initKeybinds();
	printf("-------- input init done ----------\n");
}