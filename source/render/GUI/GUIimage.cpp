#include "GUIimage.h"


//GUIimage
GUIimage::GUIimage(){
	image = defaultimage;
	noframe = true;
}
GUIimage *GUIimage::setImage(texture *newimage){
	image = newimage;
	return this;
}
GUIbase *GUIimage::sizeToContents(){
	if(image){
		area = area.setSize(image->size());
	}else{

	}
	return this;
}
void GUIimage::render(){
	if(GUIoptions.push){pushRenderOptions();}
	GUIframe::render();
	setColor({255,255,255});
	setTexture(image);
	//setTexturing(true); drawRect(thisToWorld(rect(area.size)));
	setTexturing(true);
	drawRect(worldArea());
	setTexturing(false);
	if(GUIoptions.push){popRenderOptions();}
}

