#include "resource/resource.h"
#include "loaders/imageLoader.h"
#include "loaders/textureLoader.h"
#include "loaders/modelLoader.h"
#include "loaders/fontLoader.h"
#include "program/file.h"
#include "resource/bitmap.h"
#include "resource/model.h"
#include "resource/fonts.h"
#include "util/globals_resource.h"
//#include "util/global_vars_resource.h"
//#include "render/paint.h"
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



Result<bitmap*,zError> getBitmap(string name){
	for(unsigned int I = 0; I < bitmaps.size(); I++){
		if(bitmaps[I]->name == name){return bitmaps[I];}
	}
	//string filepath = locateResource("bitmap", name.c_str());
	auto filepath = locateResource("bitmap", name.c_str());
	if(!filepath.ok()){return *filepath.err;}
	
	bitmap *bmp = loadImage(filepath.val().c_str());
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

Result<texture*,zError> getTexture(string name){
	if (name.empty()) {return nullptr;}
	for(unsigned int I = 0; I < textures.size(); I++){
		if(textures[I]->name == name){return textures[I];}
	}
	auto filepath = locateResource("texture", name.c_str());
	if(!filepath.ok()){return *filepath.err;}
	texture *t = loadTexture(filepath.val().c_str());
	if(!t){
		//error("can't load texture %s\n",name.c_str());
		stringstream ss;
		ss << "can't load texture " << name;
		throw runtime_error(ss.str());
	}
	else{printf("texture %s loaded\n",name.c_str());}
	t->name = name;
	//setLayer(Gr->gs_paint->g_loadLayer);
	//uploadTexture(t);
	//someone else do this
	textures.push_back(t);
	return t;
}

Result<texture*, zError> getModelTexture(string name){
	for(unsigned int I = 0; I < textures.size(); I++){
		if(textures[I]->name == name){return textures[I];}
	}
	auto filepath = locateResource("model_texture", name.c_str());
	if(!filepath.ok()){return *filepath.err;}
	texture *t = loadTexture(filepath.val().c_str());
	if(!t){
		//error("can't load texture %s\n",name.c_str());
		stringstream ss;
		ss << "can't load texture " << name;
		throw runtime_error(ss.str());
	}
	else{printf("texture %s loaded\n",name.c_str());}
	t->name = name;
	//setLayer(Gr->gs_paint->g_loadLayer);
	//uploadTexture(t);
	
	textures.push_back(t);
	return t;
}
Result<model*, zError> getModel(string name){
	for(unsigned int I = 0; I < models.size(); I++){
		if(models[I]->name == name){return models[I];}
	}
	auto filepath = locateResource("model", name.c_str());
	if(!filepath.ok()){return *filepath.err;}
	model *m = loadModel(filepath.val().c_str());
	if(!m){
		//stringstream ss;
		//ss << "can't load model " << name;
		//throw runtime_error(ss.str());
		return zError(SS("can't load model" << name));
	}
	auto tex = getModelTexture(name);
	if(tex.ok()){
		m->t = tex.val();
	}else{
		tex.err->severity = zError::WARNING;
		push(*tex.err);
	}
	models.push_back(m);
	return m;
}
Result<font*, zError> getFont(string name){
	//printf("get font [%s]\n",name.c_str());
	for(unsigned int I = 0; I < fonts.size(); I++){
		//printf("trying [%s]\n",fonts[I]->name.c_str());
		if(fonts[I]->name == name){return fonts[I];}
	}
	//printf("not found, loading\n");
	char fontname[80];
	int size = 0;
	sscanf(name.c_str(),"%s %d",fontname,&size);
	auto filepath = locateResource("font", fontname);
	if(!filepath.ok()){return *filepath.err;}
	font *f = loadFont(filepath.val().c_str(),size);
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

//#include "render/paint.h"
//#include "main.h"
//#include "render/rmodel.h"
//#include "generators/editmodel.h"

void loadAssets(){
	//getFont("cour 14");
	//setFont(getFont("calibri 18"));

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
