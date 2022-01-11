#include "gui/Gui2/GUI2horizontalList.h"

GUI2horizontalList::GUI2horizontalList():GUI2base(){
	wrap = true;
	resizible = false;
	movable = false;
	size.y = 0;
	size.x = 0;
}

void GUI2horizontalList::render(void *arg){
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	//glDisable(GL_SCISSOR_TEST);
}

void GUI2horizontalList::invalidate(vec2i newPos, vec2i newSize){
	
	listNode* Cur = children;
	if(Cur != NULL && Cur->thing != NULL){
		vec2i lastPos = ((GUI2base*)Cur->thing)->pos;
		while(Cur!=NULL)
		{
			if(Cur->thing){
				((GUI2base*)Cur->thing)->setPos(lastPos.x,lastPos.y);
				vec2i size = ((GUI2base*)Cur->thing)->size;
				lastPos.x = lastPos.x+size.x;
			}
			Cur = Cur->next;
		}
	}
	GUI2base::invalidate(newPos, newSize);
}