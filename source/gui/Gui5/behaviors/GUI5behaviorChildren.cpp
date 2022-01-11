#include "gui/Gui5/behaviors/GUI5behaviorChildren.h"
#include "gui/Gui5/GUI5base.h"

void GUI5behaviorChildren::run(GUI5base *B, message *msg){
	for(int I = 0; I < B->children.size(); I++){
		B->children[I]->receiveMessage(msg);
	}
}