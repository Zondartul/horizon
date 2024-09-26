#include "GUI/GUI_internal.h"

GUIimage::GUIimage(){
	auto& loadLayer = Gr->gs_paint->g_loadLayer;
	image = defaultimage;
	if (image) {
		setLayer(loadLayer);
		uploadTexture(image);
	}
	noframe = true;
}
GUIimage *GUIimage::setImage(texture *newimage){
	auto& loadLayer = Gr->gs_paint->g_loadLayer;
	image = newimage;
	if (image) {
		setLayer(loadLayer);
		uploadTexture(image);
	}
	return this;
}
GUIbase *GUIimage::sizeToContents(){
	if(image){
		area = area.setSize(image->size());
	}else{
	}
	return this;
}
GUIpropertyTable GUIimage::getDefaultPropertyTable(){
	GUIpropertyTable table = GUIframe::getDefaultPropertyTable();
	table.table["image"] = toString((texture*)defaultimage);
	return table;
}
string GUIimage::getProperty(string key){
	if(key == "image"){return toString(image);}
	else{return GUIframe::getProperty(key);}
}
void GUIimage::setProperty(string key, string val){
	if(key == "image"){setImage(	fromString<texture*>(val).val()		);}
	else{GUIframe::setProperty(key,val);}
}
string GUIimage::getType(){return "GUIimage";}
