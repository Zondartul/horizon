#include "paint.h"
#include "stdio.h"
#include "globals.h"
#include "simplemath.h"
#include "renderLayer.h"
#include "rmodel.h"
#include "model.h"
#include "timer.h"
#include "util.h"
#include <vector>
using std::vector;
#include <fstream>
using std::ofstream;
#include <sstream>
using std::stringstream;
#include "renderCommand.h"
renderLayer *currentLayer = 0;
//core functions

//vector<renderLayer*> layers; //we moved to renderLayer.cpp

void setLayer(renderLayer *L){currentLayer = L;}
void addLayer(renderLayer *L){
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
	for(auto I = layers.begin(); I!= layers.end(); I++){
		if(*I == L){
			I = layers.erase(I);
		}
	}
}
bool printAllPending = false;
bool frameReportPending = false;
void renderAllLayers(){
	if(printAllPending){printAllPending = false; printAllLayers();}
	if(frameReportPending){frameReportPending = false; frameReport();}
	for(auto I = layers.begin(); I!= layers.end(); I++){
        renderLayer *L = *I;
        //printf("parsing layer %p [%s]\n",L,L->name.c_str());
		if(!L->special){(*I)->render();}
		if(!L->persistent){(*I)->clear();}
	}
}
void printAllLayers(){
	int J = 0;
	for(auto I = layers.begin(); I != layers.end(); I++, J++){
		printf("layer %d: \"%s\"\n",J, (*I)->name.c_str());
		(*I)->print();
	}
}
void frameReport(){

	string filename = "logs/framereport"; //string()+"framereport_"+__DATE__+"_"+__TIME__+"_"+toString(getGameTicks());
	filename = filename + "_" + getCalendarDateStr()+"_"+getCalendarTimeStr()+".txt";
	ofstream fs(filename);
	if(!fs.is_open()){error((string("can't open file [")+filename+"] for writing\n").c_str());}
	stringstream ss;
	int J = 0;
	ss << "main sequence (RC2):\n";
	for(auto I = layers.begin(); I != layers.end(); I++, J++){
		ss << (*I)->report();
	}
    ss << "\nmain sequence (RC3):\n";
	for(auto I = layers.begin(); I != layers.end(); I++, J++){
		ss << (*I)->report3();
	}
	fs << ss.str();
	fs.close();
	printf("frame report saved to %s\n",filename.c_str());
}

void printAllLayersNextRender(){printAllPending = true;}
void frameReportNextRender(){frameReportPending = true;}
void drawLayer(renderLayer *L)  {currentLayer->push(new renderCommand3(RC3T::LAYER,             L));}//{currentLayer->push(new rcmd_layer(L));}

void setColoring(bool b)        {currentLayer->push(new renderCommand3(RC3T::COLORING,          b));}//{currentLayer->push(new rcmd_coloring(b));}				//enables per-vertex coloring (no color = white)
void setUvColoring(bool b)      {currentLayer->push(new renderCommand3(RC3T::UVCOLORING,        b));}//{currentLayer->push(new rcmd_uvcoloring(b));}			//(debug) add color based on UV-coordinates
void setNormalColoring(bool b)  {currentLayer->push(new renderCommand3(RC3T::NORMALCOLORING,    b));}//{currentLayer->push(new rcmd_normalcoloring(b));}	//(debug) add color based on the normal vector
void setTransparency(bool b)    {currentLayer->push(new renderCommand3(RC3T::TRANSPARENCY,      b));}//{currentLayer->push(new rcmd_transparency(b));}		//enables transparency (no transparency = alpha 1)
void setDepthMask(bool b)       {currentLayer->push(new renderCommand3(RC3T::DEPTHMASK,         b));}//{currentLayer->push(new rcmd_depthmask(b));}			//enables or disables depth buffer writing
void setTexturing(bool b)       {currentLayer->push(new renderCommand3(RC3T::TEXTURING,         b));}//{currentLayer->push(new rcmd_texturing(b));}			//enables texturing (no texture = colors only)
void setDebug(bool b)           {currentLayer->push(new renderCommand3(RC3T::DEBUG,             b));}//{currentLayer->push(new rcmd_debug(b));}					//enables debug rendering mode
void setScissoring(bool b)      {currentLayer->push(new renderCommand3(RC3T::SCISSORING,        b));}//{currentLayer->push(new rcmd_scissoring(b));}			//enable scissor test

void setColor(vec3 col)         {currentLayer->push(new renderCommand3(RC3T::COLOR,             col));}//{currentLayer->push(new rcmd_color(col));}				//sets global color (default = 1,1,1)
void setAlpha(float a)          {currentLayer->push(new renderCommand3(RC3T::ALPHA,             a));}//{currentLayer->push(new rcmd_alpha(a));}					//sets global alpha (default = 1), needs transparency
void setTexture(texture *t)     {currentLayer->push(new renderCommand3(RC3T::TEXTURE_SELECT,    t));}//{currentLayer->push(new rcmd_texture_select(t));}		//selects current texture, needs upload
void setFont(font *f)           {currentLayer->push(new renderCommand3(RC3T::FONT_SELECT,       f));}//{currentLayer->push(new rcmd_font_select(f));}				//selects current font, needs upload
void setRenderMode(int mode)    {currentLayer->push(new renderCommand3(RC3T::MODE_SELECT,       mode));}//{currentLayer->push(new rcmd_mode_select(mode));}	//0 - points, 1 - lines, 3 - triangles, 4 - triangles (wireframe)
void setTextPos(vec2 textPos)   {currentLayer->push(new renderCommand3(RC3T::TEXT_POS,          textPos));}//{currentLayer->push(new rcmd_text_pos(textPos));} 	//textPos is advanced automatically after each print
void setScissor(rect S)         {currentLayer->push(new renderCommand3(RC3T::SCISSOR,           S));}//{currentLayer->push(new rcmd_scissor(S));}				//limits the drawn-to area
void setPointSize(float size)   {currentLayer->push(new renderCommand3(RC3T::POINTSIZE,         size));}//{currentLayer->push(new rcmd_pointsize(size));}		//sets size of single points (not lines)
void setLineWidth(float width)  {currentLayer->push(new renderCommand3(RC3T::LINEWIDTH,         width));}//{currentLayer->push(new rcmd_linewidth(width));}	//sets the width of lines

void uploadTexture(texture *t)  {currentLayer->push(new renderCommand3(RC3T::TEXTURE_UPLOAD,        t));}//{currentLayer->push(new rcmd_texture_upload(t));}	//uploads texture to GPU
void uploadRmodel(rmodel *rm)   {currentLayer->push(new renderCommand3(RC3T::RMODEL_UPLOAD,         rm));}//{currentLayer->push(new rcmd_rmodel_upload(rm));}	//uploads rmodel to GPU
void deleteRmodel(rmodel *rm)   {currentLayer->push(new renderCommand3(RC3T::RMODEL_DELETE,         rm));}//{currentLayer->push(new rcmd_rmodel_delete(rm));}	//deletes rmodel from GPU

void setProjection(camprojection cpj)
                                {currentLayer->push(new renderCommand3(RC3T::PROJECTION,            copyToHeap(cpj)));}//{currentLayer->push(new rcmd_projection(cpj));}	//changes current projection matrix
void setProjectionToCamera(cameraKind *camera)  {setProjection(camera->getProjection());}
void setPosition(vec3 pos)      {currentLayer->push(new renderCommand3(RC3T::POSITION,              pos));}//{currentLayer->push(new rcmd_position(pos));}			//changes current world pos
void setRotation(vec3 rot)      {currentLayer->push(new renderCommand3(RC3T::ROTATION,              rot));}//{currentLayer->push(new rcmd_rotation(rot));}			//changes current rotation
void setScale(vec3 scale)       {currentLayer->push(new renderCommand3(RC3T::SCALE,                 scale));}//{currentLayer->push(new rcmd_scale(scale));}       		//changes current object scale

void setTexturePosition(vec2 pos){currentLayer->push(new renderCommand3(RC3T::TEXTURE_POS,			pos));}//changes global texture shift
void setTextureScale(vec2 scale){currentLayer->push(new renderCommand3(RC3T::TEXTURE_SCALE,			scale));}//changes global texture scale

void clearScreen()              {currentLayer->push(new renderCommand3(RC3T::CLEAR_SCREEN           ));}//{currentLayer->push(new rcmd_clear_screen());}					//clears screen with current color
void drawRmodel(rmodel *rm)     {currentLayer->push(new renderCommand3(RC3T::RMODEL_RENDER,         rm));}//{currentLayer->push(new rcmd_rmodel_render(rm));}//draws rendermodel
void printText(string text)     {currentLayer->push(new renderCommand3(RC3T::PRINT_TEXT,            copyToHeap(text)));}//{currentLayer->push(new rcmd_print_text(text));}	//draws text at current textPos
void renderComment(string str)  {currentLayer->push(new renderCommand3(RC3T::COMMENT,               copyToHeap(str)));}//{currentLayer->push(new rcmd_comment(str));}
void setDepthTest(bool b)       {currentLayer->push(new renderCommand3(RC3T::DEPTH_TEST,            b));}//{currentLayer->push(new rcmd_depth_test(b));}
void setLighting(bool b)        {currentLayer->push(new renderCommand3(RC3T::LIGHTING,              b));}//{currentLayer->push(new rcmd_lighting(b));}
void setSunPos(vec3 pos)        {currentLayer->push(new renderCommand3(RC3T::SUN_POS,               pos));}//{currentLayer->push(new rcmd_sun_pos(pos));}						//sets the directional light position
void setSunColor(vec3 val)      {currentLayer->push(new renderCommand3(RC3T::SUN_LIGHT_COLOR,       val));}//{currentLayer->push(new rcmd_sun_light_color(val));}			//sets the directional light brightness
void setAmbientColor(vec3 val)  {currentLayer->push(new renderCommand3(RC3T::AMBIENT_LIGHT_COLOR,   val));}//{currentLayer->push(new rcmd_ambient_light_color(val));}	//sets the ambient light brightness
void pushRenderOptions()        {currentLayer->push(new renderCommand3(RC3T::PUSH_OPTIONS              ));}
void popRenderOptions()         {currentLayer->push(new renderCommand3(RC3T::POP_OPTIONS               ));}


void debugFloatingText(vec3 p, string S){
	setLayer(layerDebug2D);
	setColor(vec3(0,0,0));
	if(dot(p-camera.pos,camera.forward()) > 0){
		vec3 tp = camera.worldToScreen(p);
		//printf("world %s -> screen %s [%s]\n",toCString(p),toCString(tp), truncateString(S,13).c_str());

		setTextPos(vec2(tp.x,tp.y));
		printText(S);
	}

}


void drawRmodelStd(rmodel *rm){
	setPosition(vec3(0,0,0));
	setRotation(vec3(0,0,0));
	setScale(vec3(1,1,1));
	drawRmodel(rm);
}
//simple drawing functions:
void drawPoint(vec3 pos){
	rmodel *rm = new rmodel();
	rm->vertices->push_back(pos);
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
	//printf("dv = %s, len = %f, angV = %f, angH = %f\n",toCString(dv),len,angV,angH);
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
	//drawPoint(vec3(1,0,0),vec3(255,0,0));
	//drawPoint(vec3(0,1,0),vec3(0,255,0));
	//drawPoint(vec3(0,0,1),vec3(0,0,255));
	//drawPoint(vec3(0,0,0),vec3(0,0,0));
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
void drawRectOutline(rect R){
	rmodel *rm = new rmodel();
	vec3 A = vec3(R.start.x,R.start.y,0.0f);
	vec3 B = vec3(R.start.x,R.end.y,0.0f);
	vec3 C = vec3(R.end.x,R.end.y,0.0f);
	vec3 D = vec3(R.end.x,R.start.y,0.0f);
	vec2 UVA = vec2(0,0);//*0.1f;
	vec2 UVB = vec2(0,1);//*0.1f;
	vec2 UVC = vec2(1,0);//*0.1f;
	vec2 UVD = vec2(1,1);//*0.1f;

	rm->vertices->push_back(A);	rm->uvs->push_back(UVA);	rm->colors->push_back(vec3(1,0,0));
	rm->vertices->push_back(B);	rm->uvs->push_back(UVB);	rm->colors->push_back(vec3(0,1,0));

	rm->vertices->push_back(B);	rm->uvs->push_back(UVB);	rm->colors->push_back(vec3(0,1,0));
	rm->vertices->push_back(C);	rm->uvs->push_back(UVC);	rm->colors->push_back(vec3(0,0,1));

	rm->vertices->push_back(C);	rm->uvs->push_back(UVC);	rm->colors->push_back(vec3(0,0,1));
	rm->vertices->push_back(D);	rm->uvs->push_back(UVD);	rm->colors->push_back(vec3(1,1,0));

	rm->vertices->push_back(D);	rm->uvs->push_back(UVD);	rm->colors->push_back(vec3(1,1,0));
	rm->vertices->push_back(A);	rm->uvs->push_back(UVA);	rm->colors->push_back(vec3(1,0,0));


	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(2);
	drawRmodel(rm);
	deleteRmodel(rm);
}
void drawRectOutlineColored(rect R, vec3 col){
	setScissoring(false);
	setColor(col);
	drawRectOutline(R);
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

#include "modelprimitives.h"
#include "editmodel.h"
rmodel *rm_unitboxWF = 0;
rmodel *rm_unitcyllinderWF = 0;
rmodel *rm_unitconeWF = 0;
rmodel *rm_unitsphereWF = 0;

rmodel *rm_unitbox = 0;
rmodel *rm_unitcyllinder = 0;
rmodel *rm_unitcone = 0;
rmodel *rm_unitsphere = 0;

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
	if(!rm_unitboxWF){
		renderLayer *oldLayer = currentLayer;
		rm_unitboxWF = generateBox(vec3(1,1,1))->getRmodel(1);
		currentLayer = oldLayer;
	}
	setRenderMode(2);
	setPosition(pos);
	setRotation(rot);
	setScale(scale);
	drawRmodel(rm_unitboxWF);
}

void drawCyllinderWireframe(vec3 pos, vec3 rot, float r, float h){
	if(!rm_unitcyllinderWF){
		renderLayer *oldLayer = currentLayer;
		rm_unitcyllinderWF = generateCyllinder(1,1,16)->getRmodel(1);
		currentLayer = oldLayer;
	}
	setRenderMode(2);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,h));
	drawRmodel(rm_unitcyllinderWF);
}
void drawConeWireframe(vec3 pos, vec3 rot, float r, float h){
	if(!rm_unitconeWF){
		renderLayer *oldLayer = currentLayer;
		rm_unitconeWF = generateCone(1,1,16)->getRmodel(1);
		currentLayer = oldLayer;
	}

	setRenderMode(2);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,h));
	drawRmodel(rm_unitconeWF);
}
void drawSphereWireframe(vec3 pos, vec3 rot, float r){
	if(!rm_unitsphereWF){
		renderLayer *oldLayer = currentLayer;
		rm_unitsphereWF = generateSphere(1,16,8)->getRmodel(1);
		currentLayer = oldLayer;
	}
	setRenderMode(2);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,r));
	drawRmodel(rm_unitsphereWF);
}

void drawBox(vec3 pos, vec3 rot,  vec3 scale){
	if(!rm_unitbox){
		renderLayer *oldLayer = currentLayer;
		rm_unitbox = generateBox(vec3(1,1,1))->getRmodel(2);
		currentLayer = oldLayer;
	}
	setRenderMode(3);
	setPosition(pos);
	setRotation(rot);
	setScale(scale);
	drawRmodel(rm_unitbox);
}
void drawCyllinder(vec3 pos, vec3 rot, float r, float h){
	if(!rm_unitcyllinder){
		renderLayer *oldLayer = currentLayer;
		rm_unitcyllinder = generateCyllinder(1,1,16)->getRmodel(2);
		currentLayer = oldLayer;
	}
	setRenderMode(3);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,h));
	drawRmodel(rm_unitcyllinder);
}
void drawCone(vec3 pos, vec3 rot, float r, float h){
	if(!rm_unitcone){
		renderLayer *oldLayer = currentLayer;
		rm_unitcone = generateCone(1,1,16)->getRmodel(2);
		currentLayer = oldLayer;
	}
	setRenderMode(3);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,h));
	drawRmodel(rm_unitcone);
}
void drawSphere(vec3 pos, vec3 rot, float r){
	if(!rm_unitsphere){
		renderLayer *oldLayer = currentLayer;
		rm_unitsphere = generateSphere(1,16,8)->getRmodel(2);
		currentLayer = oldLayer;
	}
	setRenderMode(3);
	setPosition(pos);
	setRotation(rot);
	setScale(vec3(r,r,r));
	drawRmodel(rm_unitsphere);
}

renderLayer *loadLayer;		//data load commands go here
renderLayer *layer3D;		//3D drawing commands go here
renderLayer *layerDebug;	//3D debug indication goes here
renderLayer *layer2D;		//2D drawing commands go here
renderLayer *layerDebug2D;	//2D debug indication goes here
renderLayer *deleteLayer;	//data delete commands go here

void initLayers(){
	loadLayer 			= addNewLayer("main.loadLayer"); //data is uploaded to GPU here
	layer3D 			= addNewLayer("main.layer3D"); //3d stuff goes here
	layer3D->resetLayer	= addNewLayer("main.layer3D.reset",true,true); //3d perspective is setup
	layerDebug			= addNewLayer("main.layerDebug"); //3d debug points/lines go here
	layerDebug->resetLayer = addNewLayer("main.layerDebug.reset",true,true); //3d debug is set up
	layer2D 			= addNewLayer("main.layer2D"); //2d stuff goes here
	layer2D->resetLayer = addNewLayer("main.layer2D.reset",true,true); //2d projection is setup
	layerDebug2D		= addNewLayer("main.layerDebug2D"); //2d debug text goes here
	layerDebug2D->resetLayer = addNewLayer("main.layerDebug2D.reset",true,true);
	deleteLayer 		= addNewLayer("main.deleteLayer"); //data is deleted from GPU here
	setLayer(loadLayer);	//always set layer before any render commands
}

//these are in printw.cpp
//rect preprintText2D(const char *text);
//void printw(const char *format, ...);
//rect preprintw(const char *format, ...);	//returns the area taken up by the printed string


// void setTexture(texture *t){
	// rqueue->push_back(new rcmd_texture_select(t));
// }


// void drawTexturedRectUV(rect area,rect UV){
	// rmodel *rm = new rmodel();
	// vec3 A = vec3(area.start.x,area.start.y,0.0f)*0.05f;
	// vec3 B = vec3(area.start.x,area.end.y,0.0f)*0.05f;
	// vec3 C = vec3(area.end.x,area.start.y,0.0f)*0.05f;
	// vec3 D = vec3(area.end.x,area.end.y,0.0f)*0.05f;
	// vec2 UVA = vec2(UV.start.x,UV.start.y);//*0.1f;
	// vec2 UVB = vec2(UV.start.x,UV.end.y);//*0.1f;
	// vec2 UVC = vec2(UV.end.x,UV.start.y);//*0.1f;
	// vec2 UVD = vec2(UV.end.x,UV.end.y);//*0.1f;
	// rm->vertices->push_back(A);
	// rm->vertices->push_back(B);
	// rm->vertices->push_back(C);
	// rm->vertices->push_back(B);
	// rm->vertices->push_back(C);
	// rm->vertices->push_back(D);

	// rm->uvs->push_back(UVA);
	// rm->uvs->push_back(UVB);
	// rm->uvs->push_back(UVC);
	// rm->uvs->push_back(UVB);
	// rm->uvs->push_back(UVC);
	// rm->uvs->push_back(UVD);


	// rm->colors->push_back(vec3(1,0,0));
	// rm->colors->push_back(vec3(1,1,1));
	// rm->colors->push_back(vec3(1,1,1));
	// rm->colors->push_back(vec3(1,1,1));
	// rm->colors->push_back(vec3(1,1,1));
	// rm->colors->push_back(vec3(1,0,0));

	// once(printf("A = (%f,%f,%f)\n",A.x,A.y,A.z));
	// once(printf("B = (%f,%f,%f)\n",B.x,B.y,B.z));
	// once(printf("C = (%f,%f,%f)\n",C.x,C.y,C.z));
	// once(printf("D = (%f,%f,%f)\n",D.x,D.y,D.z));

	// once(printf("UVA = (%f,%f)\n",UVA.x,UVA.y));
	// once(printf("UVB = (%f,%f)\n",UVB.x,UVB.y));
	// once(printf("UVC = (%f,%f)\n",UVC.x,UVC.y));
	// once(printf("UVD = (%f,%f)\n",UVD.x,UVD.y));

	// rm->finalize();
	// rqueue->push_back(new rcmd_rmodel_upload(rm));
	// rqueue->push_back(new rcmd_coloring(true));
	// rqueue->push_back(new rcmd_texturing(true));
	// rqueue->push_back(new rcmd_rmodel_render(rm,3));
	// rqueue->push_back(new rcmd_rmodel_delete(rm));
// }
