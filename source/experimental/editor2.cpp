#include "editor2.h"
#include "event.h"
#include "mouse.h"
#include "stringUtils.h"
#include "stdio.h"
#include "camera.h"
#include "renderLayer.h"
#include "paint.h"
#include "window.h"
#include "geometry.h"
#include "frameprinter.h"
#include "editmodel.h"
#include "simplemath.h"
#include "input.h"

/*
	editor scratchpad-style rendering:
	1) make a persistent render-layer with stuff in it
	2) draw to it
	3) only clear when closing editor
*/

class editor2Kind:public eventListener{
	public:
	editor2Kind();
	
	struct{
		renderLayer *l3D;
		renderLayer *l3Dimmediate;
		renderLayer *l2D;
		renderLayer *l2Dimmediate;
	} layers;
	void setupLayers();
	void resetLayer(renderLayer *L);
	
	e_model EM;
	void constructTestModel();
	e_selection sel;
	void redraw();
	void printselection();
	
	void think();
	void onEvent(eventKind event);
	
	bool hasLastPoint = false;
	bool showLine = false;
	vec3 lastPoint;
	
	bool boxSelectOn = false;
	bool hasBox = false;
	vec2 boxStart;
	vec2 boxEnd;
	void boxSelect(vec2 boxStart, vec2 boxEnd);
};
editor2Kind *editor2;

//renderLayer *editorLayer;
//renderLayer *editorLayerImmediate3D;

void editor2Kind::setupLayers(){
	layers.l3D 			= new renderLayer();
	layers.l3Dimmediate = new renderLayer();
	layers.l2D 			= new renderLayer();
	layers.l2Dimmediate = new renderLayer();
	addLayer(layer3D,		layers.l3D);
	addLayer(layers.l3D,	layers.l3Dimmediate);
	addLayer(layer2D,		layers.l2D);
	addLayer(layers.l2D,	layers.l2Dimmediate);
}

void editor2Kind::resetLayer(renderLayer *L){
	if(L == layers.l3D){
		layers.l3D->clear();
		setLayer(layers.l3D);
		setColoring(true);
		setTexturing(false);
		setPointSize(3);
	}
	if(L == layers.l3Dimmediate){
		layers.l3Dimmediate->clear();
		setLayer(layers.l3Dimmediate);
		setColoring(true);
		setTexturing(false);
		setPointSize(3);
	}
	if(L == layers.l2D){
		layers.l2D->clear();
	}
	if(L == layers.l2Dimmediate){
		layers.l2Dimmediate->clear();
	}
	
}

void editor2Kind::printselection(){
	printf("selected %d verts, %d edges, %d tris\n",sel.verts.size(),sel.edges.size(),sel.tris.size());
}

/*
	"  F          G\n"
	"\n"
	"  .  \n"
	"\n"
	".   .\n"
	" . . \n"
	"  E  \n"
	" . . \n"
	".   .\n"
	"\n"
	"A---B\n"
	"|   |\n"
	"|   |\n"
	"|   |\n"
	"C---D\n"
*/

void editor2Kind::constructTestModel(){
	EM = e_model();
	/*
	   D   F
	   |
	   C
	   |\
	   | \\
	   O-+-A
	    \|/
	     B
	
	*/
	e_vertex *vO = new e_vertex({0,0,0},&EM);
	e_vertex *vA = new e_vertex({1,0,0},&EM);
	e_vertex *vB = new e_vertex({0,1,0},&EM);
	e_vertex *vC = new e_vertex({0,0,1},&EM);	
	e_vertex *vD = new e_vertex({0,0,2},&EM);
	e_vertex *vF = new e_vertex({1,1,2},&EM);
	
	new e_edge(vO,vA,&EM);
	new e_edge(vO,vB,&EM);
	new e_edge(vA,vB,&EM);
	new e_edge(vO,vC,&EM);
	new e_edge(vB,vC,&EM);
	new e_edge(vA,vC,&EM);
	new e_edge(vC,vD,&EM);
	
	new e_triangle(vO,vA,vB,&EM);
	new e_triangle(vO,vA,vC,&EM);
	new e_triangle(vO,vB,vC,&EM);
	new e_triangle(vA,vB,vC,&EM);
	
	EM.recalculateNeighbors();
	
	setLayer(layers.l3D);
	sel = EM.selectAll();
	sel.rebuildRmodel();
}

void openEditor2(){
	editor2 = new editor2Kind();
	
	//editorLayerImmediate3D = new renderLayer();
	//editorLayer = new renderLayer();
	//setLayer(layer3D);
	//addLayer(editorLayer);
	//addLayer(editorLayerImmediate3D);
	//resetEditorLayer();
	//resetEditorLayerImmediate3D();
	
	//plane1[0] = vec3(0,0,0);
	//plane1[1] = vec3(1,0,0);
	//plane1[2] = vec3(0,1,0);
}

editor2Kind::editor2Kind():sel(&EM){
	inputChannel->addListener(this);
	inputChannel->moveListenerToFront(this);
	globalChannel->addListener(this);
	setupLayers();
	constructTestModel();
	redraw();
}

//extern vector<renderLayer*> layers; why tf is this here?

void editor2Kind::boxSelect(vec2 boxStart, vec2 boxEnd){
	sel.clear();
	camera.go3D();
	string S1 = string("boxStart: ")+toString(boxStart)+"boxEnd: "+toString(boxEnd);
	//printf("%s\n",S1.c_str());
	for(auto I = EM.verts.begin(); I != EM.verts.end(); I++){
		vec3 vw = (*I)->pos;
		vec3 vs = camera.worldToScreen(vw);
		vec2 vsi = {vs.x,vs.y};
		bool contains = false;
		if(rect(boxStart,boxEnd).repair().contains(vsi)){
			sel.verts.push_back(*I);
			contains = true;
		}
		//string S = string("vw: ")+toString(vw)+", vs: "+toString(vs)+", contains: "+toString(contains);
		//printf("%s\n",S.c_str());
	}
	redraw();
}

void editor2Kind::redraw(){
	resetLayer(layers.l3D);
	setLayer(layers.l3D);
	e_selection selAll = EM.selectAll();
	selAll.removeElements(sel);
	sel.colorVerts	= {0,1.f,1.f};
	sel.colorEdges	= {0,0,1.f};
	sel.colorTris	= {0,0.5f,1.f};
	printf("selAll.verts = %d\nsel.verts = %d\n",selAll.verts.size(),sel.verts.size());
	sel.rebuildRmodel();
	selAll.rebuildRmodel();
	
	//setDepthTest(false);
	setPointSize(3);
	selAll.render();
	
	
	//clearDepthBuffer();
	setPointSize(5);
	sel.render();
	//setDepthTest(true);
}


static vec3 plane[3] = {vec3(0,0,0),vec3(1,0,0),vec3(0,1,0)};

void editor2Kind::think(){
	//logic
		//point at plane the user is pointing at
		vec3 p1;
		vec3 dir = camera.getMouseDir();
		bool has_hit = ray_plane_intersection(camera.pos,dir,plane[0],plane[1],plane[2],&p1);
		float dist1 = length(p1-lastPoint);																	//point to last point distance
		float dist2 = point_line_distance(camera.pos,camera.pos+dir,lastPoint);				//screen ray to last point distance
		float dist3 = point_plane_distance(plane[0],plane[1],plane[2],camera.pos);				//plane to last point distance
		frameprint(has_hit? string("dist: ")+toString(dist1)+", dist2: "+toString(dist2) : "no hit");
		frameprint(string("cam.z: ")+toString(camera.pos.z)+", dist3: "+toString(dist3));
		bool lastPointSelected = (dist2 < 0.05f);															//selected if screen ray within distance (sphere test)
		//box select stuff
		if(hasBox){boxEnd = getMousePos();}
		frameprint(string("hasBox: ")+toString(hasBox));
	//redraw immediate layers
	resetLayer(layers.l3Dimmediate);
	resetLayer(layers.l2Dimmediate);
	//resetEditorLayerImmediate3D();
	if(hasLastPoint){
		setLayer(layers.l3Dimmediate);
		vec3 lastPointColor;
		if(has_hit){
			if(showLine){drawLine(p1,lastPoint,{0,255,0});}
			if(lastPointSelected){
				lastPointColor = {255,0,0};
			}else{
				lastPointColor = {0,255,0};
			}
		}
		setPointSize(10);
		drawPoint(lastPoint,lastPointColor);
		setPointSize(3);
	}
	if(hasBox){
		setLayer(layers.l2Dimmediate);
		setColor({0,128,255});
		drawRectOutline(rect(boxStart,boxEnd));
	}
}

void editor2Kind::onEvent(eventKind event){
	vec2 screenpos;
	vec3 forward;
	switch(event.type){
		case(EVENT_FRAME):
			think();
		break;
		case(EVENT_MOUSE_BUTTON_DOWN):
			if(event.mousebutton.button == MOUSE_LEFT){
				event.maskEvent();
				if(boxSelectOn){
					hasBox = true;
					boxStart = getMousePos();
				}else{
					setLayer(layers.l3D);
					vec3 dir = camera.getMouseDir();
					vec3 p1;
					if(ray_plane_intersection(camera.pos,dir,plane[0],plane[1],plane[2],&p1)){
						hasLastPoint = true;
						lastPoint = p1;
						drawLine(camera.pos,p1,{0,255,0});
						drawPoint(p1,{0,255,0});
					}else{
						printf("no hit\n");
					}
					redraw();
				}
			}
			if(event.mousebutton.button == MOUSE_RIGHT){
				resetLayer(layers.l3D);
			}
			if(event.mousebutton.button == MOUSE_MIDDLE){
				showLine = true;
			}
		break;
		case(EVENT_MOUSE_BUTTON_UP):
			if(event.mousebutton.button == MOUSE_LEFT){
				if(boxSelectOn && hasBox){
					boxEnd = getMousePos();
					boxSelect(boxStart,boxEnd);
					hasBox = false;
				}
			}
			if(event.mousebutton.button == MOUSE_MIDDLE){
				showLine = false;
			}
		break;
		case(EVENT_MOUSE_MOVE):
			screenpos = event.mousemove.pos;
		break;
		case(EVENT_KEY_DOWN):
			string K = event.keyboard.key;
			if(K == "P"){
				event.maskEvent();
				printAllLayersNextRender();
			}
			if(K == "B"){
				event.maskEvent();
				boxSelectOn = !boxSelectOn;
				printf("boxSelectOn = %d\n",boxSelectOn);
			}
			if(K == "1"){
				event.maskEvent();
				//printf("drawing ORTHODOX umbrella\n");
				//drawFrustumbrella(Z_IS_ORTHODOX);
				//e_selection sel2 = EM.selectAll();
				//sel = e_selection();
				//sel.EM = &EM;
				//sel.verts.push_back(sel2.verts.back());
				//printf("selected last vertex\n");
				sel = sel.getImplicitVerts();
				printselection();
				redraw();
			}
			if(K == "2"){
				event.maskEvent();
				//printf("drawing DISTANCE umbrella\n");
				//drawFrustumbrella(Z_IS_DISTANCE);
				
				//e_selection sel2 = EM.selectAll();
				//sel = e_selection();
				//sel.EM = &EM;
				//sel.edges.push_back(sel2.edges.back());
				//printf("selected last edge\n");
				sel = sel.getImplicitEdges();
				printselection();
				redraw();
				
			}
			if(K == "3"){
				event.maskEvent();
				//printf("drawing PLANE umbrella\n");
				//drawFrustumbrella(Z_IS_PLANE);
				//e_selection sel2 = EM.selectAll();
				//sel = e_selection();
				//sel.EM = &EM;
				//sel.tris.push_back(sel2.tris.back());
				//printf("selected last triangle\n");
				sel = sel.getImplicitTris();
				printselection();
				redraw();
				
			}
			if(K == "4"){
				event.maskEvent();
				//drawCurvyPoint();
				
				//e_selection sel;
				//e_selection sel2 = EM.selectAll();
				//sel.EM = &EM;
				//sel.verts.push_back(sel2.verts[3]);
				//sel.verts.push_back(sel2.verts[4]);
				sel.rotate({0,0,0},{1,0,0},d2r*30);
				
				//resetEditorLayer();
				redraw();
			}
			if(K == "5"){
				event.maskEvent();
				const char *bork = EM.checkDegenerate(false);
				printf("bork = [%s]\n",bork);
				printf("EM: %d verts, %d edges, %d tris\n",EM.verts.size(),EM.edges.size(),EM.tris.size());
			}
			if(K == "6"){
				event.maskEvent();
				sel = sel.getNeighborsEssential();
				printselection();
				redraw();
			}
			if(K == "7"){
				event.maskEvent();
				sel = sel.getNeighborsDirect();
				printselection();
				redraw();
			}
			if(K == "8"){
				event.maskEvent();
				e_selection sel2(&EM);
				sel2.addElements(sel.getNeighborsDirect());
				sel2.addElements(sel.getNeighborsEssential());
				sel = sel2;
				printselection();
				redraw();
			}
			if(K == "9"){
				event.maskEvent();
				sel = sel.extrude()[0];
				sel.move({1,0,0});
				redraw();
			}
			if(K == "0"){
				event.maskEvent();
				EM.verts.push_back(EM.verts.back());
				redraw();
			}
		break;
	}
}








/*

void drawWorldRay(vec3 start, vec3 end, vec3 color){
	drawLine(toVec3(start),toVec3(end),color);
}
void drawWorldRay2(vec3 start, vec3 end, vec3 color={0,0,0}){
	drawWorldRay(tovec3(start),tovec3(end),color);
}
void drawCamRay(vec3 dir,vec3 color){
	drawLine(toVec3(camera.pos),toVec3(camera.pos+dir),color);
}

void drawScreenRay(vec2 screenpos,vec3 color,float len=1.0f,z_meaning zm=Z_IS_DISTANCE){
	vec3 worldpos;
	worldpos = camera.screenToWorld(vec3(screenpos.x,screenpos.y,len),zm);
	drawWorldRay(camera.pos,tovec3(worldpos),color);
}

void drawSomeRays(){
	setLayer(editorLayer);
	vec2 scr = toVec2((vec2)getScreenSize());
	printf("\ndrawing len = 1.f\n");
	drawScreenRay(scr/2.f,{255,0,0},1.0f);
	printf("\ndrawing len = 0.f\n");
	drawScreenRay(scr/2.f,{0,0,0},0.f);
	printf("\ndrawing len = -1.f\n");
	drawScreenRay(scr/2.f,{0,0,0},-1.f);
	printf("drawing len = 1.f at 0,0\n");
	drawScreenRay({0,0},{0,0,255},1.f);

	printf("\ndrawing len = 2.f\n");
	drawScreenRay(scr/2.f,{255,255,0},2.0f);
	printf("\ndrawing len = 2f at 0,0\n");
	drawScreenRay({0,0},{0,255,0},2.0f);

	printf("\ndrawing len = 0.1f\n");
	drawScreenRay(scr/2.f,{255,255,255},0.1f);
	printf("\ndrawing len = 100.f\n");
	drawScreenRay(scr/2.f,{0,0,0},100.f);
	printf("\ndrawing len = 10.f\n");
	drawScreenRay(scr/2.f,{0,0,0},10.f);
	printf("\ndrawing len = 10.f at (0,h)\n");
	drawScreenRay({0,scr.y-1},{0,0,0},10.f);
	printf("\ndrawing len = 10.f at (w,0)\n");
	drawScreenRay({scr.x-1,0},{0,0,0},10.f);
}

void drawFrustumbrella(z_meaning zm){
	for(int x = 0; x < width; x += 10){
		drawScreenRay({x,0},{255,0,0},1,zm);
	}
	for(int y = 0; y < height; y += 10){
		drawScreenRay({width,y},{255,255,0},1,zm);
	}
	for(int x = 0; x < width; x += 10){
		drawScreenRay({x,height},{0,255,0},1,zm);
	}
	for(int y = 0; y < height; y += 10){
		drawScreenRay({0,y},{0,0,255},1,zm);
	}
	float len = 1.f;
	vec3 p1 = camera.screenToWorld(vec3(0,0,len),zm);
	vec3 p2 = camera.screenToWorld(vec3(width,0,len),zm);
	vec3 p3 = camera.screenToWorld(vec3(width,height,len),zm);
	vec3 p4 = camera.screenToWorld(vec3(0,height,len),zm);
	setLayer(editorLayer);
	drawWorldRay(tovec3(p1),tovec3(p2),{255,0,0});
	drawWorldRay(tovec3(p2),tovec3(p3),{255,255,0});
	drawWorldRay(tovec3(p3),tovec3(p4),{0,255,0});
	drawWorldRay(tovec3(p4),tovec3(p1),{0,0,255});
	drawWorldRay(tovec3(p1),tovec3(p3),{0,0,0});
	drawWorldRay(tovec3(p2),tovec3(p4),{0,0,0});
	drawWorldRay(camera.pos,camera.pos+camera.forward(),{255,0,255});
}

void drawFrustum(){
	camera.go3D();
	vec3 p1 = camera.deviceToWorld({1,1,1});
	vec3 p2 = camera.deviceToWorld({1,1,-1});
	vec3 p3 = camera.deviceToWorld({1,-1,1});
	vec3 p4 = camera.deviceToWorld({1,-1,-1});
	vec3 p5 = camera.deviceToWorld({-1,1,1});
	vec3 p6 = camera.deviceToWorld({-1,1,-1});
	vec3 p7 = camera.deviceToWorld({-1,-1,1});
	vec3 p8 = camera.deviceToWorld({-1,-1,-1});

	setLayer(editorLayer);
	drawWorldRay2(p1,p2); //right
	drawWorldRay2(p2,p4);
	drawWorldRay2(p3,p4);
	drawWorldRay2(p3,p1);

	drawWorldRay2(p5,p6); //left
	drawWorldRay2(p6,p8);
	drawWorldRay2(p7,p8);
	drawWorldRay2(p7,p5);

	drawWorldRay2(p1,p5); //top+bottom
	drawWorldRay2(p2,p6); 
	drawWorldRay2(p3,p7);
	drawWorldRay2(p4,p8);

	drawWorldRay2(p4,p6,{255,255,255}); //near cross
	drawWorldRay2(p2,p8,{255,255,255});

	drawWorldRay2(p1,p7); //far cross
	drawWorldRay2(p3,p5);

	drawPoint(toVec3(camera.pos),{255,255,255});
}

void drawFlatPoint(){
	vec2 screenpos = getMousePos();
	vec3 p1 = camera.screenToWorld(vec3(screenpos.x,screenpos.y,1.f),Z_IS_PLANE);
	vec3 p2 = camera.screenToWorld(camera.worldToScreen(p1,Z_IS_PLANE),Z_IS_PLANE);
	drawWorldRay(tovec3(p1),tovec3(p2),{0,0,0});
	drawPoint(p1,{0,255,0});
	drawPoint(p2,{255,0,0});
}

void drawCurvyPoint(){
	vec2 screenpos = getMousePos();
	vec3 p1 = camera.screenToWorld(vec3(screenpos.x,screenpos.y,1.f),Z_IS_DISTANCE);
	vec3 p2 = camera.screenToWorld(camera.worldToScreen(p1,Z_IS_DISTANCE),Z_IS_DISTANCE);
	drawWorldRay(tovec3(p1),tovec3(p2),{0,0,0});
	drawPoint(p1,{0,255,0});
	drawPoint(p2,{255,0,0});
}
*/