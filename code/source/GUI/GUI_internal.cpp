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


GUIbase *GUI_constructor_dispatch(string S){
	GUIbase *W = 0;
	if(S == "GUIbase"){W = new GUIbase();}
	else if(S == "GUIbutton"){W = new GUIbutton();}
	else if(S == "GUIframe"){W = new GUIframe();}
	else if(S == "GUIgrid"){W = new GUIgrid();}
	else if(S == "GUIimage"){W = new GUIimage();}
	else if(S == "GUIlabel"){W = new GUIlabel();}
	else if(S == "GUIscrollbar"){W = new GUIscrollbar();}
	else if(S == "GUItextEntry"){W = new GUItextEntry();}
	else if(S == "GUIwindow"){W = new GUIwindow();}
	else if(S == "GUIselectionGroup"){W = new GUIselectionGroup();}
	else if(S == "GUIselectable"){W = new GUIselectable();}
	
	if(!W){printf("dispatch missing widget type %s\n",S.c_str());}
	return W;
}







