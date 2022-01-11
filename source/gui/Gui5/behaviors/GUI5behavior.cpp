#include "gui/GUI5/behaviors/GUI5behavior.h"
#include "util/globals.h"

GUI5behavior::GUI5behavior(){}
void GUI5behavior::run(GUI5base *B, message *msg){printf("GUI5base %p base behavior (msg.type = %s)\n",B,msg->type.c_str());};