#include "GUI_internal.h"
#include "main.h"
void GUIsetFavoriteRenderOptions(){
	auto& layerGUI = Gg->gs_GUI->g_layerGUI;
	setLayer(layerGUI);
	layerGUI->clear();
	setColoring(false);
	setTexturing(false);
	setColor(defaultbgcolor);
    setTransparency(true);
	setAlpha(255);
	setScissoring(false);
	setTextPos({0,0});
	setFont(defaulttextfont);
    setPosition(vec3(0,0,0));
    setRotation(vec3(0,0,0));
    setScale(vec3(1,1,1));
	setTexturePosition(vec2(0,0));
	setTextureScale(vec2(1,1));
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
