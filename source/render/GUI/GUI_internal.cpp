#include "GUI_internal.h"

//#define defaultimage getTexture("resource/party.jpg")
extern renderLayer *layerGUI;
//utility functions for GUI
GUI_options_kind GUIoptions;

void GUIsetFavoriteRenderOptions(){
	setLayer(layerGUI);
	layerGUI->clear();
	setColoring(false);
	setTexturing(false);
	setColor(defaultbgcolor);
    setTransparency(true);
	setAlpha(255);
	//firstn(50,printf("setAlpha\n"));
	//clearScissor();
	setScissoring(false);
	//setThickness(1);
	//setTextMaxX(-1);
	//setTextMaxY(-1);
	setTextPos({0,0});
	//setInfiniteLine(false);
	//setInfinitePlane(false);
	setFont(defaulttextfont);
    setPosition(vec3(0,0,0));
    setRotation(vec3(0,0,0));
    setScale(vec3(1,1,1));
	setTexturePosition(vec2(0,0));
	setTextureScale(vec2(1,1));
	//setTextScale(1);
}
