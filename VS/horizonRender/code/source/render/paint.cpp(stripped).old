#include "render/paint.h"
#include "render/renderLayer.h"
#include "render/rmodel.h"
#include "util/globals_render.h"
#include "util/global_vars_render.h"
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
using std::vector;
using std::ofstream;
using std::stringstream;
using std::runtime_error;

//idea: complex vs primitive commands
//complex commands (like drawText) are reworded into a sequence of primitive ones

void setLayer(renderLayer *L){
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer = L;
}

void frameReport(){
	auto& layers = Gr->gs_renderLayer->g_layers;

	string filename = "logs/framereport"; 
	filename = "framereport.txt";
	ofstream fs(filename);
	if(!fs.is_open()){
		//error((string("can't open file [")+filename+"] for writing\n").c_str());
		stringstream ss;
		ss << "can't open file [" << filename << "] for writing";
		throw runtime_error(ss.str());
	}
	stringstream ss;
	int j = 0;

    ss << "\nmain sequence (rc3):\n";
	for(auto i = layers.begin(); i != layers.end(); i++, j++){
		ss << (*i)->report3();
	}
	fs << ss.str();
	fs.close();
	printf("frame report saved to %s\n",filename.c_str());
}
void setTransparency(bool b)    {
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer->push(new renderCommand3(RC3T::TRANSPARENCY,      b));
}//{currentLayer->push(new rcmd_transparency(b));}		//enables transparency (no transparency = alpha 1)

void setTexturing(bool b)       {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::TEXTURING,         b));
}//{currentLayer->push(new rcmd_texturing(b));}			//enables texturing (no texture = colors only)

void setAlpha(float a)          {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::ALPHA,             a));
}//{currentLayer->push(new rcmd_alpha(a));}					//sets global alpha (default = 1), needs transparency

void setTexture(texture *t)     {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::TEXTURE_SELECT,    t));
}//{currentLayer->push(new rcmd_texture_select(t));}		//selects current texture, needs upload

void setRenderMode(int mode)    {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::MODE_SELECT,       mode));
}//{currentLayer->push(new rcmd_mode_select(mode));}	//0 - points, 1 - lines, 3 - triangles, 4 - triangles (wireframe)

void uploadRmodel(rmodel *rm)   {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::RMODEL_UPLOAD,         rm));
}//{currentLayer->push(new rcmd_rmodel_upload(rm));}	//uploads rmodel to GPU

void deleteRmodel(rmodel *rm)   {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::RMODEL_DELETE,         rm));
}//{currentLayer->push(new rcmd_rmodel_delete(rm));}	//deletes rmodel from GPU

void drawRmodel(rmodel *rm)     {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::RMODEL_RENDER,         rm));
}//{currentLayer->push(new rcmd_rmodel_render(rm));}//draws rendermodel

void printText(string text)     {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::PRINT_TEXT,            copyToHeap(text)));
}//{currentLayer->push(new rcmd_print_text(text));}	//draws text at current textPos

void drawRect(rect R){
	rmodel *rm = new rmodel();
	vec3 A = vec3(R.start.x,R.start.y,0.0f);
	vec3 B = vec3(R.start.x,R.end.y,0.0f);
	vec3 C = vec3(R.end.x,R.start.y,0.0f);
	vec3 D = vec3(R.end.x,R.end.y,0.0f);
	vec2 UVA = vec2(0,0);//*0.1f;
	vec2 UVB = vec2(0,1);//*0.1f;
	vec2 UVC = vec2(1,0);//*0.1f;
	vec2 UVD = vec2(1,1);//*0.1f;
	rm->vertices->push_back(A);
	rm->vertices->push_back(B);
	rm->vertices->push_back(C);
	rm->vertices->push_back(B);
	rm->vertices->push_back(D);
	rm->vertices->push_back(C);

	rm->uvs->push_back(UVA);
	rm->uvs->push_back(UVB);
	rm->uvs->push_back(UVC);
	rm->uvs->push_back(UVB);
	rm->uvs->push_back(UVD);
	rm->uvs->push_back(UVC);


	rm->colors->push_back(vec3(1,0,0));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,0,0));

	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(3);
	drawRmodel(rm);
	deleteRmodel(rm);
}
