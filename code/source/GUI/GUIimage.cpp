#include "GUI_internal.h"


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

/*
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
*/

	//property table
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
	//printf("%s::setProperty(%s)=[%s]\n",getType().c_str(),key.c_str(),val.c_str());

	if(key == "image"){setImage(fromString<texture*>(val));}
	else{GUIframe::setProperty(key,val);}
}


string GUIimage::getType(){return "GUIimage";}

