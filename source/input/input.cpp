#include "input.h"
#include "inputController.h"
#include "keybinds.h"

eventChannel *inputChannel;

void initInput(){
	inputChannel = new eventChannel();
	inputController = new inputControllerKind();
	
	initKeybinds();
	printf("-------- input init done ----------\n");
}