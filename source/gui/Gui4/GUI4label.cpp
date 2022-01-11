#include "gui/GUI4/GUI4label.h"
#include "display/paint.h"
#include "resource/fonts.h"

void GUI4label::render(){
	color3i color1 = {255*8/10,255*8/10,255};
	color3i color2 = {0,0,0};
	setAlpha(128);
	rect vis = getVisibleWorldArea();
	setColor(color1);
	paintRect(vis);//paintRect(area.toWorld());
	setColor(color2);
	paintRectOutline(vis);//paintRectOutline(area.toWorld());
	
	printw(vis.getx()+text_origin.x,vis.gety()+text_origin.y,-1,-1,"%s",text.c_str());
	GUI4base::render();	
}

GUI4label &GUI4label::setText(string text){
	this->text = text;
	checkSizeToContents();
	return *this;
}

//err, if we return a reference to GUI4base, does vtable still work?...
void GUI4label::checkSizeToContents(){
	if(size_to_contents){
		rect size;
		size.setStart({0,0}).setEnd({0,0});
		preprintw(&size, 0, -1, -1, "%s", text.c_str());
		
		printf("[%p]:G4label::cSTC:(%d,%d)\n",this,size.getw(),size.geth());
		setSize(size.getw(),size.geth());
		//area.setSize(size.size);
		text_origin = -(size.getStart());
		//printf("[x:%d,y:%d]\n",text_origin.x,text_origin.y);
	}
}