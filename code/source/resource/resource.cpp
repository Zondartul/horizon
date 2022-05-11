#include "resource.h"
#include "imageLoader.h"
#include "textureLoader.h"
#include "fontLoader.h"
#include "file.h"
#include "bitmap.h"
#include "model.h"
#include "fonts.h"
#include "globals.h"
#include "paint.h"
#include "paint.h"
#include "main.h"
#include "rmodel.h"
#include "editmodel.h"
#include "global_vars.h"
bitmap *getBitmap(string name){
	auto& bitmaps = Gb->gs_resource->g_bitmaps;
	for(unsigned int I = 0; I < bitmaps.size(); I++){
		if(bitmaps[I]->name == name){return bitmaps[I];}
	}
	string filepath = locateResource("bitmap", name.c_str());
	bitmap *bmp = loadImage(filepath.c_str());
	if(!bmp){error("can't load bitmap %s\n",name.c_str());}
	bmp->name = name;
	bitmaps.push_back(bmp);
	return bmp;
}
texture *getTexture(string name){
	auto& textures = Gb->gs_resource->g_textures;
	auto& loadLayer = Gb->gs_paint->g_loadLayer;
	for(unsigned int I = 0; I < textures.size(); I++){
		if(textures[I]->name == name){return textures[I];}
	}
	string filepath = locateResource("texture", name.c_str());
	texture *t = loadTexture(filepath.c_str());
	if(!t){error("can't load texture %s\n",name.c_str());}
	else{printf("texture %s loaded\n",name.c_str());}
	t->name = name;
	setLayer(loadLayer);
	uploadTexture(t);
	textures.push_back(t);
	return t;
}
texture *getModelTexture(string name){
	auto& textures = Gb->gs_resource->g_textures;
	auto& loadLayer = Gb->gs_paint->g_loadLayer;
	for(unsigned int I = 0; I < textures.size(); I++){
		if(textures[I]->name == name){return textures[I];}
	}
	string filepath = locateResource("model_texture", name.c_str());
	texture *t = loadTexture(filepath.c_str());
	if(!t){error("can't load texture %s\n",name.c_str());}
	else{printf("texture %s loaded\n",name.c_str());}
	t->name = name;
	setLayer(loadLayer);
	uploadTexture(t);
	textures.push_back(t);
	return t;
}
model *getModel(string name){
	auto& models = Gb->gs_resource->g_models;
	for(unsigned int I = 0; I < models.size(); I++){
		if(models[I]->name == name){return models[I];}
	}
	string filepath = locateResource("model", name.c_str());
	return 0;
}
font *getFont(string name){
	auto& fonts = Gb->gs_resource->g_fonts;
	for(unsigned int I = 0; I < fonts.size(); I++){
		if(fonts[I]->name == name){return fonts[I];}
	}
	char fontname[80];
	int size = 0;
	sscanf(name.c_str(),"%s %d",fontname,&size);
	string filepath = locateResource("font", fontname);
	font *f = loadFont(filepath.c_str(),size);
	f->name = name;
	if(!f){error("can't load font %s, size %d\n",fontname, size);}
	fonts.push_back(f);
	return f;
}
vector<bitmap*> listBitmaps(){
	auto& bitmaps = Gb->gs_resource->g_bitmaps;
	return bitmaps;
}
vector<texture*> listTextures(){
	auto& textures = Gb->gs_resource->g_textures;
	return textures;
}
vector<model*> listModels(){
	auto& models = Gb->gs_resource->g_models;
	return models;
}
vector<font*> listFonts(){
	auto& fonts = Gb->gs_resource->g_fonts; 
	return fonts;
}
void loadAssets(){
	//auto& m = Gb->gs_main->g_m; 
	getFont("cour 14");
	setFont(getFont("calibri 18"));
}
