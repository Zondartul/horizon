#include "math/simplemath.h"
#include "render/paint.h"
#include "render/renderLayer.h"
#include "render/rmodel.h"
#include "render/renderCommand.h"
#include "resource/model.h"
#include "resource/fonts.h"
#include "generators/editmodel.h"
#include "util/timer.h"
#include "util/util.h"
#include "util/global_vars_render.h"
#include "util/globals_render.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
using namespace std;

gs_paintKind::gs_paintKind() {

}

void setLayer(renderLayer *L){
	auto &currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer = L;
}
void addLayer(renderLayer *L){
	auto &currentLayer = Gr->gs_paint->g_currentLayer;
	auto &layers = Gr->gs_renderLayer->g_layers;
	if(currentLayer){
		for(auto I = layers.begin(); I!= layers.end(); I++){
			if(*I == currentLayer){
				layers.insert(I+1,L);
				return;
			}
		}
	}else{
		layers.push_back(L);
	}
}
void addLayer(renderLayer *L1, renderLayer *L2){
	auto &layers = Gr->gs_renderLayer->g_layers;
	for(auto I = layers.begin(); I!= layers.end(); I++){
			if(*I == L1){
				layers.insert(I+1,L2);
				return;
			}
	}
}
renderLayer *addNewLayer(){
	renderLayer *L = new renderLayer();
	addLayer(L);
	return L;
}
renderLayer *addNewLayer(string name, bool persistent, bool special){
	renderLayer *L = new renderLayer(name);
	L->persistent = persistent;
	L->special = special;
	addLayer(L);
	return L;
}
renderLayer *duplicateLayer(renderLayer *L){
	return L->duplicate();
}
void clearLayer(renderLayer *L){L->clear();}
void resetLayer(renderLayer *L){L->reset();}
void removeLayer(renderLayer *L){
	auto &layers = Gr->gs_renderLayer->g_layers;
	for(auto I = layers.begin(); I!= layers.end(); I++){
		if(*I == L){
			I = layers.erase(I);
		}
	}
}
void renderAllLayers(){
	auto &printAllPending = Gr->gs_paint->g_printAllPending;
	auto &frameReportPending = Gr->gs_paint->g_frameReportPending;
	auto &layers = Gr->gs_renderLayer->g_layers;
	if(printAllPending){printAllPending = false; printAllLayers();}
	if(frameReportPending){frameReportPending = false; frameReport();}
	for(auto I = layers.begin(); I!= layers.end(); I++){
        renderLayer *L = *I;
		if(!L->special){(*I)->render();}
		if(!L->persistent){(*I)->clear();}
	}
}
void printAllLayers(){
	auto &layers = Gr->gs_renderLayer->g_layers;
	int J = 0;
	for(auto I = layers.begin(); I != layers.end(); I++, J++){
		printf("layer %d: \"%s\"\n",J, (*I)->name.c_str());
		(*I)->print();
	}
}
void frameReport(){
	auto &layers = Gr->gs_renderLayer->g_layers;
	string filename = "logs/framereport";
	filename = filename + "_" + getCalendarDateStr()+"_"+getCalendarTimeStr()+".txt";
	ofstream fs(filename);
	if(!fs.is_open()){
		//error((string("can't open file [")+filename+"] for writing\n").c_str());
		stringstream ss;
		ss << "can't open file[" << filename << "] for writing\n";
		throw std::runtime_error(ss.str());
	}
	stringstream ss;
	int J = 0;
    ss << "\nmain sequence (RC3):\n";
	for(auto I = layers.begin(); I != layers.end(); I++, J++){
		ss << (*I)->report3();
	}
	fs << ss.str();
	fs.close();
	printf("frame report saved to %s\n",filename.c_str());
}
void printAllLayersNextRender()	{
	auto &printAllPending = Gr->gs_paint->g_printAllPending;
	printAllPending = true;
}
void frameReportNextRender()	{
	auto& frameReportPending = Gr->gs_paint->g_frameReportPending;
	frameReportPending = true;
}
void drawLayer(renderLayer *L)  {
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer->push(new renderCommand3(RC3T::LAYER,             L));
}
void setColoring(bool b)        {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::COLORING,          b));
}		
void setUvColoring(bool b)      {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::UVCOLORING,        b));
}		
void setNormalColoring(bool b)  {
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer->push(new renderCommand3(RC3T::NORMALCOLORING,    b));
}	
void setTransparency(bool b)    {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::TRANSPARENCY,      b));
}	
void setDepthMask(bool b)       {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::DEPTHMASK,         b));
}	
void setTexturing(bool b)       {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::TEXTURING,         b));
}	
void setDebug(bool b)           {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::DEBUG,             b));
}		
void setScissoring(bool b)      {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::SCISSORING,        b));
}		
void setColor(vec3 col)			{
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::COLOR,				col));
}		
void setAlpha(float a)          {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::ALPHA,             a));
}		
void setTexture(texture *t)     {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::TEXTURE_SELECT,    t));
}		
//void setFont(font *f)           {
//	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
//	currentLayer->push(new renderCommand3(RC3T::FONT_SELECT,       f));
//}		
void setRenderMode(int mode)    {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::MODE_SELECT,       mode));
}
//void setTextPos(vec2 textPos)   {
//	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
//	currentLayer->push(new renderCommand3(RC3T::TEXT_POS,          textPos));
//} 	
void setScissor(rect S)         {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::SCISSOR,           S));
}	
void setPointSize(float size)   {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::POINTSIZE,         size));
}	
void setLineWidth(float width)  {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::LINEWIDTH,         width));
}
void uploadTexture(texture *t)  {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::TEXTURE_UPLOAD,        t));
}	
void uploadRmodel(rmodel *rm)   {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::RMODEL_UPLOAD,         rm));
}	
void deleteRmodel(rmodel *rm)   {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::RMODEL_DELETE,         rm));
}	
void setProjection(camprojection cpj){
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::PROJECTION,            copyToHeap(cpj)));
}
void setProjectionToCamera(cameraKind *camera){
	setProjection(camera->getProjection());
}
void setPosition(vec3 pos){
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer->push(new renderCommand3(RC3T::POSITION,              pos));
}
void setRotation(vec3 rot){
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::ROTATION,              rot));
}
void setScale(vec3 scale)       {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::SCALE,                 scale));
}
void setTexturePosition(vec2 pos){
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::TEXTURE_POS,			pos));
}
void setTextureScale(vec2 scale){
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::TEXTURE_SCALE,			scale));
}
void clearScreen(){
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::CLEAR_SCREEN           ));
}
void drawRmodel(rmodel *rm)     {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::RMODEL_RENDER,         rm));
}
void printText(string text)     {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::PRINT_TEXT,            copyToHeap(text)));
}
void renderComment(string str)  {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::COMMENT,               copyToHeap(str)));
}
void setDepthTest(bool b)       {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::DEPTH_TEST,            b));
}
void setLighting(bool b)        {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::LIGHTING,              b));
}
void setSunPos(vec3 pos)        {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::SUN_POS,               pos));
}
void setSunColor(vec3 val)      {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::SUN_LIGHT_COLOR,       val));
}
void setAmbientColor(vec3 val)  {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::AMBIENT_LIGHT_COLOR,   val));
}
void pushRenderOptions()        {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::PUSH_OPTIONS              ));
}
void popRenderOptions()         {
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::POP_OPTIONS               ));
}
void resetRenderOptions() {
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer->push(new renderCommand3(RC3T::RESET_OPTIONS			  ));
}

void printRenderOptions(){
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer->push(new renderCommand3(RC3T::PRINT_OPTIONS			  ));
}

void setFaceCulling(bool b)		{
	auto& currentLayer = Gr->gs_paint->g_currentLayer; 
	currentLayer->push(new renderCommand3(RC3T::FACE_CULLING, b));
}
void setFaceCullCCW() {
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer->push(new renderCommand3(RC3T::FACE_CULL_CCW));
}
void readPixels(int x, int y, int w, int h, void* buff) {
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	currentLayer->push(new renderCommand3(RC3T::READ_PIXELS, buff));
}
void setViewport(int x, int y, int w, int h) {
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	rect R;
	R = R.setStart(vec2(x, y)).setSize(vec2(w, h));
	currentLayer->push(new renderCommand3(RC3T::VIEWPORT, R));
}
void debugFloatingText(vec3 p, string S){
	auto& layerDebug2D = Gr->gs_paint->g_layerDebug2D;
	auto& camera = Gr->gs_camera->g_camera;
	setLayer(layerDebug2D);
	setColor(vec3(0,0,0));
	if(dot(p-camera.pos,camera.forward()) > 0){
		vec3 tp = camera.worldToScreen(p);
		//setTextPos(vec2(tp.x,tp.y));
		printText(S);
	}
}
void drawRmodelStd(rmodel *rm){
	setPosition(vec3(0,0,0));
	setRotation(vec3(0,0,0));
	setScale(vec3(1,1,1));
	drawRmodel(rm);
}
void drawPoint(vec3 pos){
	pos += vec3(0,1,0);
	rmodel *rm = new rmodel();
	rm->vertices->push_back(pos);
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(1);
	drawRmodelStd(rm);
	deleteRmodel(rm);
}
void drawPoints(const vector<vec3> &points){
	rmodel *rm = new rmodel();
	for(auto I = points.begin(); I != points.end(); I++){
		rm->vertices->push_back(*I);
	}
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(1);
	drawRmodelStd(rm);
	deleteRmodel(rm);
}
void drawPoint(vec3 pos,vec3 color){
	setColor(color);
	rmodel *rm = new rmodel();
	rm->vertices->push_back(pos);
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(1);
	drawRmodelStd(rm);
	deleteRmodel(rm);
}
void drawLine(vec3 start, vec3 end){
	rmodel *rm = new rmodel();
	rm->vertices->push_back(start);
	rm->vertices->push_back(end);
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(2);
	drawRmodelStd(rm);
	deleteRmodel(rm);
}
void drawLine(vec3 start, vec3 end,vec3 color){
	setColor(color);
	rmodel *rm = new rmodel();
	rm->vertices->push_back(start);
	rm->vertices->push_back(end);
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(2);
	drawRmodelStd(rm);
	deleteRmodel(rm);
}
void drawArrow(vec3 start, vec3 end){
	vec3 O(0,0,0);
	vec3 T(0,0,1);
	vec3 A(0.2f,0,0.8f);
	vec3 B(-0.2f,0,0.8f);
	vec3 C(0,0.2f,0.8f);
	vec3 D(0,-0.2f,0.8f);
	vec3 dv = end-start;
	vec3 hori(dv.x,dv.y,0);
	float len = length(dv);
	float v = dv.z;
	float h = length(hori);
	float angV = atan2(v,h);
	float angH = atan2(dv.y,dv.x);
	mat3 rot = yrotr(90*d2r)*yrotr(-angV)*zrotr(angH);
	vec3 O2 = start+O;
	vec3 T2 = start+T*rot*len;
	vec3 A2 = start+A*rot*len;
	vec3 B2 = start+B*rot*len;
	vec3 C2 = start+C*rot*len;
	vec3 D2 = start+D*rot*len;
	drawLine(O2,T2);
	drawLine(T2,A2);
	drawLine(T2,B2);
	drawLine(T2,C2);
	drawLine(T2,D2);
}
void drawArrow(vec3 start, vec3 end, vec3 color){
	setColor(color);
	drawArrow(start,end);
}
void drawTriangle(vec3 A, vec3 B, vec3 C){
	rmodel *rm = new rmodel();
	rm->vertices->push_back(A); rm->uvs->push_back(vec2(0,0)); rm->colors->push_back(vec3(1,0,0));
	rm->vertices->push_back(B); rm->uvs->push_back(vec2(0,1)); rm->colors->push_back(vec3(0,1,0));
	rm->vertices->push_back(C); rm->uvs->push_back(vec2(1,1)); rm->colors->push_back(vec3(0,0,1));
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(3);
	drawRmodel(rm);
	deleteRmodel(rm);
}
void drawRect(rect R){
	rmodel *rm = new rmodel();
	vec3 A = vec3(R.start.x,R.start.y,0.0f);
	vec3 B = vec3(R.start.x,R.end.y,0.0f);
	vec3 C = vec3(R.end.x,R.start.y,0.0f);
	vec3 D = vec3(R.end.x,R.end.y,0.0f);
	vec2 UVA = vec2(0,0);
	vec2 UVB = vec2(0,1);
	vec2 UVC = vec2(1,0);
	vec2 UVD = vec2(1,1);
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
void drawRectOutline(rect R){
	rmodel *rm = new rmodel();
	vec3 A = vec3(R.start.x,R.start.y,0.0f);
	vec3 B = vec3(R.start.x,R.end.y,0.0f);
	vec3 C = vec3(R.end.x,R.end.y,0.0f);
	vec3 D = vec3(R.end.x,R.start.y,0.0f);
	vec2 UVA = vec2(0,0);
	vec2 UVB = vec2(0,1);
	vec2 UVC = vec2(1,0);
	vec2 UVD = vec2(1,1);
	rm->vertices->push_back(A+vec3(1,0,0));	rm->uvs->push_back(UVA);	rm->colors->push_back(vec3(1,0,0));
	rm->vertices->push_back(B+vec3(1,0,0));	rm->uvs->push_back(UVB);	rm->colors->push_back(vec3(0,1,0));
	rm->vertices->push_back(B+vec3(0,-1,0));	rm->uvs->push_back(UVB);	rm->colors->push_back(vec3(0,1,0));
	rm->vertices->push_back(C+vec3(0,-1,0));	rm->uvs->push_back(UVC);	rm->colors->push_back(vec3(0,0,1));
	rm->vertices->push_back(C);	rm->uvs->push_back(UVC);	rm->colors->push_back(vec3(0,0,1));
	rm->vertices->push_back(D);	rm->uvs->push_back(UVD);	rm->colors->push_back(vec3(1,1,0));
	rm->vertices->push_back(D);	rm->uvs->push_back(UVD);	rm->colors->push_back(vec3(1,1,0));
	rm->vertices->push_back(A);	rm->uvs->push_back(UVA);	rm->colors->push_back(vec3(1,0,0));
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(2);
	drawRmodel(rm);
	deleteRmodel(rm);
	/// 11.02.2023: dealing with [bug 19] - don't leave rendermode at 2?
	setRenderMode(3);
}
void drawRectOutlineColored(rect R, vec3 col){
	setScissoring(false);
	setColor(col);
	drawRectOutline(R);
}
void drawImage(texture *t, rect R){
	setTexture(t);
	setTexturing(true);
	drawRect(R);
	setTexturing(false);
}
void drawModel(vec3 pos, vec3 rot, vec3 scale, model *m){
	setPosition(pos);
	setRotation(rot);
	setScale(scale);
	rmodel *rm;
	if(m->rm){
		rm = m->rm;
		drawRmodel(rm);
	}else{
		rm = new rmodel(m);
		uploadRmodel(rm);
		drawRmodel(rm);
		deleteRmodel(rm);
	}
}
#define getPosRotScale(aabb)				\
	vec3 pos = (aabb.start+aabb.end)/2.f;	\
	vec3 rot = vec3(0,0,0);					\
	vec3 scale = aabb.end-aabb.start;
void drawBoxWireframe(AABB aabb){
	getPosRotScale(aabb);
	drawBoxWireframe(pos,rot,scale);
}
void drawCyllinderWireframe(AABB aabb){
	getPosRotScale(aabb);
	drawCyllinderWireframe(pos,rot,scale.x/2.f,scale.z);
}
void drawConeWireframe(AABB aabb){
	getPosRotScale(aabb);
	drawConeWireframe(pos,rot,scale.x/2.f,scale.z);
}
void drawSphereWireframe(AABB aabb){
	getPosRotScale(aabb);
	drawSphereWireframe(pos,rot,scale.x/2.f);
}
void drawBoxWireframe(vec3 pos, vec3 rot, vec3 scale){
	auto& rm_unitboxWF = Gr->gs_paint->g_rm_unitboxWF;
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	if(!rm_unitboxWF){
		renderLayer *oldLayer = currentLayer;
		currentLayer = oldLayer;
	}
	setRenderMode(2);
	setPosition(pos);
	setRotation(rot);
	setScale(scale);
	drawRmodel(rm_unitboxWF);
}
void drawCyllinderWireframe(vec3 pos, vec3 rot, float r, float h){
	auto& rm_unitcyllinderWF = Gr->gs_paint->g_rm_unitcyllinderWF;
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	if(!rm_unitcyllinderWF){
		renderLayer *oldLayer = currentLayer;
		currentLayer = oldLayer;
	}
	setRenderMode(2);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,h));
	drawRmodel(rm_unitcyllinderWF);
}
void drawConeWireframe(vec3 pos, vec3 rot, float r, float h){
	auto& rm_unitconeWF = Gr->gs_paint->g_rm_unitconeWF;
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	if(!rm_unitconeWF){
		renderLayer *oldLayer = currentLayer;
		currentLayer = oldLayer;
	}
	setRenderMode(2);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,h));
	drawRmodel(rm_unitconeWF);
}
void drawSphereWireframe(vec3 pos, vec3 rot, float r){
	auto& rm_unitsphereWF = Gr->gs_paint->g_rm_unitsphereWF;
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	if(!rm_unitsphereWF){
		renderLayer *oldLayer = currentLayer;
		currentLayer = oldLayer;
	}
	setRenderMode(2);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,r));
	drawRmodel(rm_unitsphereWF);
}
void drawBox(vec3 pos, vec3 rot,  vec3 scale){
	auto& rm_unitbox = Gr->gs_paint->g_rm_unitbox;
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	if(!rm_unitbox){
		renderLayer *oldLayer = currentLayer;
		currentLayer = oldLayer;
	}
	setRenderMode(3);
	setPosition(pos);
	setRotation(rot);
	setScale(scale);
	drawRmodel(rm_unitbox);
}
void drawCyllinder(vec3 pos, vec3 rot, float r, float h){
	auto& rm_unitcyllinder = Gr->gs_paint->g_rm_unitcyllinder;
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	if(!rm_unitcyllinder){
		renderLayer *oldLayer = currentLayer;
		currentLayer = oldLayer;
	}
	setRenderMode(3);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,h));
	drawRmodel(rm_unitcyllinder);
}
void drawCone(vec3 pos, vec3 rot, float r, float h){
	auto& rm_unitcone = Gr->gs_paint->g_rm_unitcone;
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	if(!rm_unitcone){
		renderLayer *oldLayer = currentLayer;
		currentLayer = oldLayer;
	}
	setRenderMode(3);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,h));
	drawRmodel(rm_unitcone);
}
void drawSphere(vec3 pos, vec3 rot, float r){
	auto& rm_unitsphere = Gr->gs_paint->g_rm_unitsphere;
	auto& currentLayer = Gr->gs_paint->g_currentLayer;
	if(!rm_unitsphere){
		renderLayer *oldLayer = currentLayer;
		currentLayer = oldLayer;
	}
	setRenderMode(3);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,r));
	drawRmodel(rm_unitsphere);
}
void initLayers(){
	auto& loadLayer = Gr->gs_paint->g_loadLayer;
	auto& layer3D = Gr->gs_paint->g_layer3D;
	auto& layerDebug = Gr->gs_paint->g_layerDebug;
	auto& layer2D = Gr->gs_paint->g_layer2D;
	auto& layerDebug2D = Gr->gs_paint->g_layerDebug2D;
	auto& deleteLayer = Gr->gs_paint->g_deleteLayer;
	loadLayer 			= addNewLayer("main.loadLayer"); 
	layer3D 			= addNewLayer("main.layer3D"); 
	layer3D->resetLayer	= addNewLayer("main.layer3D.reset",true,true); 
	layerDebug			= addNewLayer("main.layerDebug"); 
	layerDebug->resetLayer = addNewLayer("main.layerDebug.reset",true,true); 
	layer2D 			= addNewLayer("main.layer2D"); 
	layer2D->resetLayer = addNewLayer("main.layer2D.reset",true,true); 
	layerDebug2D		= addNewLayer("main.layerDebug2D"); 
	layerDebug2D->resetLayer = addNewLayer("main.layerDebug2D.reset",true,true);
	deleteLayer 		= addNewLayer("main.deleteLayer"); 
	setLayer(loadLayer);	
}

void uploadFont(font* f) {
	//cout << "uploading font " << f->name << endl;
	for (auto& [c,G] : f->charmap) {
		assert(G.t); /// texture must exist
		uploadTexture(G.t);
		//cout << c;
	}
	//cout << endl << "upload font done" << endl;
}