#include "input.h"

#include "keybinds.h"
#include "global_vars.h"


void initInput(){
	auto& inputChannel = G->gs_input->g_inputChannel;
	

	inputChannel = new eventChannel();
	
	
	initKeybinds();
	printf("-------- input init done ----------\n");
}