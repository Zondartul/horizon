#include "input.h"
#include "inputController.h"

eventChannel *inputChannel;

void initInput(){
	inputChannel = new eventChannel();
	inputController = new inputControllerKind();
}