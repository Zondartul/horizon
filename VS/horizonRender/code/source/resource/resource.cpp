#include "resource/resource.h"
#include "loaders/imageLoader.h"
#include "loaders/textureLoader.h"
#include "loaders/modelLoader.h"
#include "loaders/fontLoader.h"
#include "program/file.h"
#include "resource/bitmap.h"
#include "resource/model.h"
#include "resource/fonts.h"
#include "util/globals_render.h"
#include "util/global_vars_render.h"
#include "render/paint.h"
#include <stdexcept>
#include <sstream>
using std::stringstream;
using std::runtime_error;
//maybe maps are faster, or maybe it's cold code.
//keep vectors until next profiling.
vector<bitmap*> bitmaps;
vector<texture*> textures;
vector<model*> models;
vector<font*> fonts;



bitmap *getBitmap(string name){
	for(unsigned int I = 0; I < bitmaps.size(); I++){
		if(bitmaps[I]->name == name){return bitmaps[I];}
	}
	string filepath = locateResource("bitmap", name.c_str());
	bitmap *bmp = loadImage(filepath.c_str());
	if(!bmp){
		//error("can't load bitmap %s\n",name.c_str());
		stringstream ss;
		ss << "can't load bitmap " << name;
		throw runtime_error(ss.str());
	}
	bmp->name = name;
	bitmaps.push_back(bmp);
	return bmp;
}
texture *getTexture(string name){
	for(unsigned int I = 0; I < textures.size(); I++){
		if(textures[I]->name == name){return textures[I];}
	}
	string filepath = locateResource("texture", name.c_str());
	texture *t = loadTexture(filepath.c_str());
	if(!t){
		//error("can't load texture %s\n",name.c_str());
		stringstream ss;
		ss << "can't load texture " << name;
		throw runtime_error(ss.str());
	}
	else{printf("texture %s loaded\n",name.c_str());}
	t->name = name;
	setLayer(Gr->gs_paint->g_loadLayer);
	uploadTexture(t);
	textures.push_back(t);
	return t;
}
texture *getModelTexture(string name){
	for(unsigned int I = 0; I < textures.size(); I++){
		if(textures[I]->name == name){return textures[I];}
	}
	string filepath = locateResource("model_texture", name.c_str());
	texture *t = loadTexture(filepath.c_str());
	if(!t){
		//error("can't load texture %s\n",name.c_str());
		stringstream ss;
		ss << "can't load texture " << name;
		throw runtime_error(ss.str());
	}
	else{printf("texture %s loaded\n",name.c_str());}
	t->name = name;
	setLayer(Gr->gs_paint->g_loadLayer);
	uploadTexture(t);
	textures.push_back(t);
	return t;
}
model *getModel(string name){
	for(unsigned int I = 0; I < models.size(); I++){
		if(models[I]->name == name){return models[I];}
	}
	string filepath = locateResource("model", name.c_str());
	model *m = loadModel(filepath.c_str());
	if(!m){
		//error("can't load model %s\n",name.c_str());
		stringstream ss;
		ss << "can't load model " << name;
		throw runtime_error(ss.str());
	}
	m->t = getModelTexture(name);//getTexture(string()+"models/"+name+"/model_"+name);
	models.push_back(m);
	return m;
}
font *getFont(string name){
	//printf("get font [%s]\n",name.c_str());
	for(unsigned int I = 0; I < fonts.size(); I++){
		//printf("trying [%s]\n",fonts[I]->name.c_str());
		if(fonts[I]->name == name){return fonts[I];}
	}
	//printf("not found, loading\n");
	char fontname[80];
	int size = 0;
	sscanf(name.c_str(),"%s %d",fontname,&size);
	string filepath = locateResource("font", fontname);
	font *f = loadFont(filepath.c_str(),size);
	f->name = name;
	if(!f){
		//error("can't load font %s, size %d\n",fontname, size);
		stringstream ss;
		ss << "can't load font " << name << " at size " << size;
		throw runtime_error(ss.str());
	}
	fonts.push_back(f);
	return f;
}

vector<bitmap*> listBitmaps(){return bitmaps;}
vector<texture*> listTextures(){return textures;}
vector<model*> listModels(){return models;}
vector<font*> listFonts(){return fonts;}

#include "render/paint.h"
//#include "main.h"
#include "render/rmodel.h"
#include "generators/editmodel.h"

void loadAssets(){
	getFont("cour 14");
	setFont(getFont("calibri 18"));

	//something about that fancy box
	//m = getModel("box");
	//e_model *em_box = m->toEmodel();
	//e_selection e_sel = em_box->selectAll();
	//e_sel.removeDuplicates();
	//e_sel.recalculateNormalsSmooth();
	//uploadModel(mBox);
	//m->rm = em_box->getRmodel();//new rmodel(m);
	//m->rm->finalize();
	//uploadTexture(m->t);
	//uploadRmodel(m->rm);
}
