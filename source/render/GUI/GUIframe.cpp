#include "GUIframe.h"


//GUIframe ---------------------------------------------------------------------
GUIframe::GUIframe(){
	bgColor = defaultbgcolor;
	borderColor = defaultborderColor;
	noframe = false;
}
GUIframe *GUIframe::setBgColor(vec3 color){
	bgColor = color;
	return this;
}

GUIframe *GUIframe::setBorderColor(vec3 color){
	borderColor = color;
	return this;
}

void GUIframe::render(){
	if(!noframe){
		if(GUIoptions.push){pushRenderOptions();}
		//GUIbase::render();
		setColor(bgColor);
		setAlpha(128);
		//drawRect(thisToWorld(rect(area.size)));
		drawRect(worldArea());
		setAlpha(255);
		setColor(borderColor);
		//drawRectOutline(thisToWorld(rect(area.size)));
		drawRectOutline(worldArea());
		if(GUIoptions.push){popRenderOptions();}
	}
}
