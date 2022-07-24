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
#include "editmodel.h"
#include "simplemath.h"
#include "input.h"
#include "global_vars_gui.h" //for layers

void editor2Kind::setupLayers(){
	auto& layer3D = Gb->gs_paint->g_layer3D;
	auto& layer2D = Gb->gs_paint->g_layer2D;
	printf("editor.setupLayers()\n");
	layers.l3D 			= new renderLayer("editor.l3D");
	layers.l3Dimmediate = new renderLayer("editor.l3Dimmediate");
	layers.l2D 			= new renderLayer("editor.l2D");
	layers.l2Dimmediate = new renderLayer("editor.l2Dimmedate");
	addLayer(layer3D,		layers.l3D);
	addLayer(layers.l3D,	layers.l3Dimmediate);
	addLayer(layer2D,		layers.l2D);
	addLayer(layers.l2D,	layers.l2Dimmediate);
}
#define if_first() static int first = 1; if(first-- > 0)
#define if_not_first() static int first = 1; if(!(first-- > 0))
void editor2Kind::resetLayer(renderLayer *L){
	if(L == layers.l3D){
		printf("editor.resetLayer(l3D)\n");
		layers.l3D->clear();
		setLayer(layers.l3D);
		setColoring(true);
		setTexturing(false);
		setTransparency(true);
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
		printf("editor.resetLayer(l2D)\n");
		layers.l2D->clear();
	}
	if(L == layers.l2Dimmediate){
		layers.l2Dimmediate->clear();
	}
}
void editor2Kind::printselection(){
	printf("selected %d verts, %d edges, %d tris\n",sel.verts.size(),sel.edges.size(),sel.tris.size());
}
void editor2Kind::constructTestModel(){
	printf("editor.constructTestModel()\n");
	EM = e_model();
	e_vertex *vO = new e_vertex({0,0,0},&EM);
	e_vertex *vA = new e_vertex({1,0,0},&EM);
	e_vertex *vB = new e_vertex({0,1,0},&EM);
	e_vertex *vC = new e_vertex({0,0,1},&EM);
	e_vertex *vD = new e_vertex({0,0,2},&EM);
	new e_vertex({1,1,2},&EM);
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
	auto& editor2 = Gt->gs_editor2->g_editor2;
	editor2 = new editor2Kind();
}
editor2Kind::editor2Kind():sel(&EM){
	auto& inputChannel = Gb->sysInput->inputChannel;//Gb->gs_input->g_inputChannel;
	auto& globalChannel = Gb->sysEvent->globalChannel;//Gb->gs_event->g_globalChannel;
	printf("editor.editorKind()\n");
	inputChannel.addListener(this);
	inputChannel.moveListenerToFront(this);
	globalChannel.addListener(this);
	setupLayers();
	constructTestModel();
	redraw();
}
void editor2Kind::boxSelect(vec2 boxStart, vec2 boxEnd){
	auto& camera = Gb->gs_camera->g_camera;
	printf("editor.boxSelect()\n");
	sel.clear();
	camera.go3D();
	string S1 = string("boxStart: ")+toString(boxStart)+"boxEnd: "+toString(boxEnd);
	for(auto I = EM.verts.begin(); I != EM.verts.end(); I++){
		vec3 vw = (*I)->pos;
		vec3 vs = camera.worldToScreen(vw);
		vec2 vsi = {vs.x,vs.y};
		if(rect(boxStart,boxEnd).repair().contains(vsi)){
			sel.verts.push_back(*I);
		}
	}
	redraw();
	printselection();
}
void editor2Kind::redraw(){
	printf("editor.redraw()\n");
	e_selection selAll = EM.selectAll();
	selAll.removeElements(sel);
	sel.colorVerts	= {0,1.f,1.f};
	sel.colorEdges	= {0,0,1.f};
	sel.colorTris	= {0,0.5f,1.f};
	sel.rainbowTris = false;
	selAll.rainbowTris = false;
	printf("selAll.verts = %d\nsel.verts = %d\n",selAll.verts.size(),sel.verts.size());
	sel.rebuildRmodel();
	selAll.rebuildRmodel();
	resetLayer(layers.l3D);
	setLayer(layers.l3D);
	setPointSize(3);
	selAll.render();
	setPointSize(5);
	sel.render();
}
void editor2Kind::think(){
	auto& camera = Gb->gs_camera->g_camera;
	auto& plane = Gt->gs_editor2->g_plane;
		vec3 p1;
		vec3 dir = camera.getMouseDir();
		bool has_hit = ray_plane_intersection(camera.pos,dir,plane[0],plane[1],plane[2],&p1);
		//float dist1 = length(p1-lastPoint);																	
		float dist2 = point_line_distance(camera.pos,camera.pos+dir,lastPoint);				
		//float dist3 = point_plane_distance(plane[0],plane[1],plane[2],camera.pos);				
		bool lastPointSelected = (dist2 < 0.05f);															
		if(hasBox){boxEnd = getMousePos();}
	resetLayer(layers.l3Dimmediate);
	resetLayer(layers.l2Dimmediate);
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
void splitTest(){
	static int step = 0;
	int maxstep = 10;
	printf("splitTest step %d/%d\n",step,maxstep);
	switch(step){
		case 0:
		break;
		case 1:
		break;
		default:
        break;
	}
	step++;
	if(step>maxstep){step = 0;}
}
void editor2Kind::onEvent(eventKind event){
	auto& camera = Gb->gs_camera->g_camera;
	auto& plane = Gt->gs_editor2->g_plane;
	vec2 screenpos;
	vec3 forward;
	string K;
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
			K = event.keyboard.key;
			if(K == "P"){
				event.maskEvent();
				printAllLayersNextRender();
			}
			if(K == "B"){
				event.maskEvent();
				boxSelectOn = !boxSelectOn;
				printf("\nboxSelectOn = %d\n",boxSelectOn);
			}
			if(K == "T"){
				event.maskEvent();
				splitTest();
				printf("\nsplit\n");
			}
			if(K == "1"){
				event.maskEvent();
				sel = sel.getImplicitVerts();
				printselection();
				redraw();
				printf("\nselected implicit verts\n");
			}
			if(K == "2"){
				event.maskEvent();
				sel = sel.getImplicitEdges();
				printselection();
				redraw();
				printf("\nselected implicit edges\n");
			}
			if(K == "3"){
				event.maskEvent();
				sel = sel.getImplicitTris();
				printselection();
				redraw();
				printf("\nselected implicit triangles\n");
			}
			if(K == "4"){
				event.maskEvent();
				sel.rotate({0,0,0},{1,0,0},15);
				redraw();
				printf("\nrotated by 15 degrees around Y\n");
			}
			if(K == "5"){
				event.maskEvent();
				const char *bork = EM.checkDegenerate(false);
				printf("bork = [%s]\n",bork);
				printf("EM: %d verts, %d edges, %d tris\n",EM.verts.size(),EM.edges.size(),EM.tris.size());
				printf("\nprinted selection properties\n");
			}
			if(K == "6"){
				event.maskEvent();
				printf("\nkey 6 not in use\n");
			}
			if(K == "7"){
				event.maskEvent();
				printf("\nkey 7 not in use\n");
			}
			if(K == "8"){
				event.maskEvent();
				printf("\nkey 8 not in use\n");
			}
			if(K == "9"){
				event.maskEvent();
				sel = sel.extrude()[0];
				sel.move({1,0,0});
				redraw();
				printf("\nextruded selection in X direction\n");
			}
			if(K == "0"){
				event.maskEvent();
				EM.verts.push_back(EM.verts.back());
				redraw();
				printf("\nduplicated last vertex\n");
			}
		break;
		default:
        break;
	}
}
