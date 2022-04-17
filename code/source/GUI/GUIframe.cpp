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
	if(key == "bgColor"){setBgColor(fromString<vec3>(val));}
	else if(key == "borderColor"){setBorderColor(fromString<vec3>(val));}
	else if(key == "noframe"){noframe = fromString<bool>(val);}
	else GUIbase::setProperty(key,val);
}

string GUIframe::getType(){return "GUIframe";}


