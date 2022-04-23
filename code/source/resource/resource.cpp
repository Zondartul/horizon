#include "resource.h"
#include "imageLoader.h"
#include "textureLoader.h"
#include "modelLoader.h"
#include "fontLoader.h"
#include "file.h"
#include "bitmap.h"
#include "model.h"
#include "fonts.h"
#include "globals.h"
#include "paint.h"
//maybe maps are faster, or maybe it's cold code.
//keep vectors until next profiling.
vector<bitmap*> g_bitmaps;
vector<texture*> g_textures;
vector<model*> g_models;
vector<font*> g_fonts;



bitmap *getBitmap(string name){
	for(unsigned int I = 0; I < g_bitmaps.size(); I++){
		if(g_bitmaps[I]->name == name){return g_bitmaps[I];}
	}
	string filepath = locateResource("bitmap", name.c_str());
	bitmap *bmp = loadImage(filepath.c_str());
	if(!bmp){error("can't load bitmap %s\n",name.c_str());}
	bmp->name = name;
	g_bitmaps.push_back(bmp);
	return bmp;
}
texture *getTexture(string name){
	for(unsigned int I = 0; I < g_textures.size(); I++){
		if(g_textures[I]->name == name){return g_textures[I];}
	}
	string filepath = locateResource("texture", name.c_str());
	texture *t = loadTexture(filepath.c_str());
	if(!t){error("can't load texture %s\n",name.c_str());}
	else{printf("texture %s loaded\n",name.c_str());}
	t->name = name;
	setLayer(g_loadLayer);
	uploadTexture(t);
	g_textures.push_back(t);
	return t;
}
texture *getModelTexture(string name){
	for(unsigned int I = 0; I < g_textures.size(); I++){
		if(g_textures[I]->name == name){return g_textures[I];}
	}
	string filepath = locateResource("model_texture", name.c_str());
	texture *t = loadTexture(filepath.c_str());
	if(!t){error("can't load texture %s\n",name.c_str());}
	else{printf("texture %s loaded\n",name.c_str());}
	t->name = name;
	setLayer(g_loadLayer);
	uploadTexture(t);
	g_textures.push_back(t);
	return t;
}
model *getModel(string name){
	for(unsigned int I = 0; I < g_models.size(); I++){
		if(g_models[I]->name == name){return g_models[I];}
	}
	string filepath = locateResource("model", name.c_str());
	model *m = loadModel(filepath.c_str());
	if(!m){error("can't load model %s\n",name.c_str());}
	m->t = getModelTexture(name);
	g_models.push_back(m);
	return m;
}
font *getFont(string name){
	for(unsigned int I = 0; I < g_fonts.size(); I++){
		if(g_fonts[I]->name == name){return g_fonts[I];}
	}
	char fontname[80];
	int size = 0;
	sscanf(name.c_str(),"%s %d",fontname,&size);
	string filepath = locateResource("font", fontname);
	font *f = loadFont(filepath.c_str(),size);
	f->name = name;
	if(!f){error("can't load font %s, size %d\n",fontname, size);}
	g_fonts.push_back(f);
	return f;
}

vector<bitmap*> listBitmaps(){return g_bitmaps;}
vector<texture*> listTextures(){return g_textures;}
vector<model*> listModels(){return g_models;}
vector<font*> listFonts(){return g_fonts;}

#include "paint.h"
#include "main.h"
#include "rmodel.h"
#include "editmodel.h"

void loadAssets(){
	getFont("cour 14");
	setFont(getFont("calibri 18"));

	g_m = getModel("box");
	e_model *em_box = g_m->toEmodel();
	e_selection e_sel = em_box->selectAll();
	e_sel.removeDuplicates();
	e_sel.recalculateNormalsSmooth();
	g_m->rm = em_box->getRmodel();
	g_m->rm->finalize();
	uploadTexture(g_m->t);
	uploadRmodel(g_m->rm);
}
