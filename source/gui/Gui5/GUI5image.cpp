#include "gui/Gui5/GUI5image.h"
#include "resource/fonts.h"
#include "display/paint.h"
#include "resource/textureloader.h"

GUI5image &GUI5image::setImage(string img){
	tex = getTextureHandle(img);
	invalidate();
	return *this;
}

void GUI5image::render(){
	//printw(int x, int y, int xlim, int ylim, string format, ...);
	setColor({0,0,0});
	setAlpha(255);
	rect world = area.toWorld();
	paintTexturedRect(world.getx(), world.gety(), world.getx2(), world.gety2(),tex);
	
	GUI5base::render();
}

void GUI5image::layout(){
	area.setSize({tex->width, tex->height});
	
	GUI5base::layout();
}