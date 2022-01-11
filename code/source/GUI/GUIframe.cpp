#include "GUI_internal.h"


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

/*
void GUIframe::render(){
	if(!noframe){
		renderComment(fstring("frame %p begin\n",this));
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
		renderComment(fstring("frame %p end\n",this));
	}
}
*/

GUIpropertyTable GUIframe::getDefaultPropertyTable(){
	GUIpropertyTable table = GUIbase::getDefaultPropertyTable();
	table.table["bgColor"] = toString(vec3(defaultbgcolor));
	table.table["borderColor"] = toString(vec3(defaultborderColor));
	table.table["noframe"] = toString(bool(false));
	return table;
}

string GUIframe::getProperty(string key){
	if(key == "bgColor"){return toString(bgColor);}
	else if(key == "borderColor"){return toString(borderColor);}
	else if(key == "noframe"){return toString(noframe);}
	else return GUIbase::getProperty(key);
}

void GUIframe::setProperty(string key, string val){
	//printf("%s::setProperty(%s)=[%s]\n",getType().c_str(),key.c_str(),val.c_str());

	if(key == "bgColor"){setBgColor(fromString<vec3>(val));}
	else if(key == "borderColor"){setBorderColor(fromString<vec3>(val));}
	else if(key == "noframe"){noframe = fromString<bool>(val);}
	else GUIbase::setProperty(key,val);
}

string GUIframe::getType(){return "GUIframe";}


