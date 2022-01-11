#include "globals.h"
#include "stdio.h"
#include "window.h"
#include "renderLow.h"
#include "SDL2/SDL.h"
#include "modelLoader.h"
#include "fontLoader.h"
#include "inputController.h"
#include "camera.h"
#include "fonts.h"
#include "printw.h"
#include "model.h"
#include "resource.h"
#include "gl/gl.h"
#include "paint.h"
#include "GUI.h"
#include "stringUtils.h"
#include "simplemath.h"
#include "editor2.h"
#include "timer.h"
#include "frameprinter.h"
#include "modelprimitives.h"
#include "entity.h"
// void renderLayer::setColoring(bool b){queue.push_back(new rcmd_coloring(b));}
// void renderLayer::setTransparency(bool b){queue.push_back(new rcmd_transparency(b));}
// void renderLayer::setTexturing(bool b){queue.push_back(new rcmd_texturing(b));}
// void renderLayer::setDebugShader(bool b){queue.push_back(new rcmd_debug_shader(b));}
// void renderLayer::setProjection(mat4 proj){queue.push_back(new rcmd_projection(proj));}
// void renderLayer::uploadTexture(texture *t){queue.push_back(new rcmd_texture_upload(t));}
// void renderLayer::selectTexture(texture *t){queue.push_back(new rcmd_texture_select(t));}
// void renderLayer::uploadRmodel(rmodel *rm){queue.push_back(new rcmd_rmodel_upload(rm));}
// void renderLayer::renderRmodel(rmodel *rm, int mode){queue.push_back(new rcmd_rmodel_render(rm,mode));}
// void renderLayer::deleteRmodel(rmodel *rm){queue.push_back(new rcmd_rmodel_delete(rm));}
// void renderLayer::printText2D(const char *str){printText2D(&queue, str);}

renderLayer *loadLayer;
renderLayer *layer3D;
renderLayer *layer2D;
renderLayer *deleteLayer;
//renderLayer *currentLayer;

model *m;
GUIbase *GUI = 0;
float fps = 0;
float frametime = 0;
int t = 0;
timer T1([&](){t++;},60,1);

void openGUI();

eventChannel inputChannel;
eventChannel globalChannel;

frameprinter *fprint;
//
// NEXT:
//  get an FPS counter
//  see if GUI is fast enough
//  fix it if not
//
//  maybe implement correct input
//  with a chain-of-responsibility
//  (someone noms a message)
//
//  if/when sufficiently fast,
//  make a blocky room
//  do block-sphere collision
//	derp physics
//	derp char controller

//rmodel *rm2;
//texture *rm2t;
entity *E;

int main(int argc, char **argv){
	printf("Hello World!\n");
	debuginit();
	printf("-------- debug init done ----------\n");
	window_init(512,512);
	OpenGL_printVersion();
	printf("-------- window init done ---------\n");
	renderLowInit();
	printf("-------- render low init done -----\n");
	initFreeType();
	printf("-------- FreeType init done -------\n");
	loadLayer = addNewLayer();
	layer3D = addNewLayer();
	layer2D = addNewLayer();
	deleteLayer = addNewLayer();
	
	setFramePrinter(new frameprinter());
	//openGUI();
	//openEditor2();
	
	setLayer(loadLayer);
	
	setFont(getFont("calibri 18"));
	//renderTest1();
	camera.setPos({-0.5,0,0});
	//renderTest();
	setbuf(stdout,0);
	m = getModel("box");
	//uploadModel(mBox);
	m->rm = new rmodel(m);
	m->rm->finalize();
	uploadTexture(m->t);
	uploadRmodel(m->rm);
	
	//rm2 = generateBox(vec3(1,1,10));
	//rm2t = getTexture("materials/brick2");
	//uploadTexture(rm2t);
	//uploadRmodel(rm2);
	E = new entity();
	addComponent(E,position,vec3(0,0,0));
	addComponent(E,texture,getTexture("materials/brick2"));
	addComponent(E,rmodel,generateBox(vec3(1,1,10)));
	
	printf("size of mat4: %d\n",sizeof(mat4));
	printf("size of void*: %d\n",sizeof(void*));
	printf("size of int: %d\n",sizeof(int));
	printf("size of char: %d\n",sizeof(char));
	printf("-------- frames begin ----------\n");
	
	while(1){
		//renderTest1Tick();
		//inputController.think();
		eventKind e; e.type = EVENT_FRAME;
		globalChannel.publishEvent(e);
		profile(renderTick(),frametime);
		if(frametime){fps = 1.0f/frametime;}
		timersTick();
		sysMessageTick();
		//SDL_Delay(1);
	}
	return 0;
}

void openGUI(){
	texture *img1 = getTexture("art/printer"); uploadTexture(img1);

	GUI = new GUIbase();
	inputChannel.addListenerFront(GUI);
	/*
	GUI->addChild((new GUIframe())\
		->setSize({200,300})\
		->moveTo({100,20})\
		->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({300,600})\
			->setHorizontal(false)\
			->addChild((new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20}))\
			->addChild((new GUIimage())->setImage(img1)->setSize({128,128})->moveTo({20,64}))\
			->addChild((new GUIbutton())->setFunction([](){GUI->moveTo(GUI->area.start+(vec2i){5,0});})\
						->setText("boop")->sizeToContents()->moveTo({20,200}))\
			)\
		)\
	->addChild((new GUIwindow())\
		->setSize({150,100})\
		->moveTo({350,20})\
		->addChild((new GUIlabel())->setText("Yer a wizzard Harry!")->sizeToContents())\
	);
	*/
	auto TE = new GUItextEntry();
	TE->setFunction([=](){printf("Text:[%s]\n",TE->text.c_str());})->setSize({128,24})->moveTo({20,220});
	
	GUI->addChild((new GUIwindow())\
		->setSize({200,300})\
		->moveTo({100,20})\
		->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({300,600})\
			->setHorizontal(false)\
			->addChild((new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20}))\
			->addChild((new GUIimage())->setImage(img1)->setSize({128,128})->moveTo({20,64}))\
			->addChild((new GUIbutton())->setFunction([](){GUI->moveTo(GUI->area.start+(vec2i){5,0});})\
							->setText("boop")->sizeToContents()->moveTo({20,200}))\
			->addChild(TE)\
			)\
		);
}

// void renderTriangle(){
	// rmodel *rm = new rmodel();

	// rm->vertices->push_back(vec3(0,0,0));
	// rm->vertices->push_back(vec3(0.5,0,0));
	// rm->vertices->push_back(vec3(0.5,0.5,0));
	// rm->vertices->push_back(vec3(0.5,0,0));
	// rm->vertices->push_back(vec3(0.5,0.5,0));
	// rm->vertices->push_back(vec3(1,0.5,0));
	
	// rm->colors->push_back(vec3(1,0,0));
	// rm->colors->push_back(vec3(0,1,0));
	// rm->colors->push_back(vec3(0,0,1));
	// rm->colors->push_back(vec3(1,0,0));
	// rm->colors->push_back(vec3(0,1,0));
	// rm->colors->push_back(vec3(0,0,1));
	
	// rm->finalize();
	
	// loadLayer.uploadRmodel(rm);
	// layer3D.setColoring(true);
	// layer3D.setTexturing(false);
	// layer3D.renderRmodel(rm);
	// deleteLayer.deleteRmodel(rm);
// }


void renderTick(){
	//glClearColor(0.3,0.7,0.9,1.0);
	//glClearColor(0.1,0.23,0.3,1.0);
	//========================= 3D ==============================
	setLayer(layer3D);
	clearScreen();
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	camera.go3D();
	setProjection(camera.mProjection*camera.mView);
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setDepthTest(true);
	//renderTriangle();
	
	setColoring(true);
	setTexturing(false);
	setScissoring(false);
	setTransparency(false);
	setAlpha(255);
	setColor({255,255,255});
	drawTriangle(vec3(0,-0.67,-0.5),vec3(0,0,0.5),vec3(0,0.67,-0.5));
	setColoring(true);
	setTextPos(vec2(0,0));
	printText("Hello World [ ijk XYZ o_O ] ");
	
	setColoring(false);
	setTexturing(true);
	setTexture(m->t);
	drawRmodel(m->rm);
	renderComment("comment: layer3D done\n");
	
	//setColoring(false);
	//setTexturing(true);
	//setTexture(rm2t);
	
	//setPosition(vec3(0,0,t));
	//setScale(vec3(1,1,10));
	//drawRmodel(rm2);
	getComponent(E,position)->val = vec3(t,0,0);
	ecs_render_system.render();
	//========================= 2D ==============================
	setLayer(layer2D);
	camera.go2D();
	setProjection(camera.mProjection*camera.mView);
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setDepthTest(false);
	setColoring(false);
	setTexturing(false);
	setColor({255,255,255});
	drawRmodel(m->rm);
	//printw("Hello World!");
	//textPos = {0.0f,25.0f};//{100.0f,100.0f};
	//renderCmd(RCMD::DEBUG,b=true);
	setColoring(false);
	//setTextPos(vec2(0,0));
	//printText("Hello World [ ijk XYZ o_O ] ");
	frameprint("Hello World [ ijk XYZ o_O ] ");
	
	//setTextPos(vec2(0,20));
	static float fps_filtered = 60.f;
	static float frametime_filtered = 0.015f;
	fps_filtered = mix2(fps_filtered,fps,1.0f/200.0f);
	frametime_filtered = mix2(frametime_filtered,frametime,1.0f/200.0f);
	//printText(string("FPS: ")+ftoa(fps_filtered,1)+", frametime: "+ftoa(1000*frametime_filtered,3)+"ms");//(int)(1000*round2(frametime_filtered,4))
	frameprint(string("FPS: ")+ftoa(fps_filtered,1)+", frametime: "+ftoa(1000*frametime_filtered,3)+"ms");//(int)(1000*round2(frametime_filtered,4))
	
	//setTextPos(vec2(0,40));
	//printText(string("T1: ")+t);
	//setTextPos(vec2(0,60));
	//printText(string("cam speed: ")+inputController.velocity.length());
	frameprint(string("T1: ")+t);
	frameprint(string("cam speed: ")+inputController.velocity.length());
	if(GUI){GUI->renderLogic();}
	// renderLayer(loadLayer); durrr, renderLayer(renderLayer)
	// renderLayer(layer3D);
	// renderLayer(layer2D);
	// renderLayer(deleteLayer);
	
	// clearLayer(loadLayer);
	// clearLayer(layer3D);
	// clearLayer(layer2D);
	// clearLayer(deleteLayer);
	renderAllLayers();
	//loadLayer->render();
	//layer3D->render();
	//layer2D->render();
	//deleteLayer->render();
	
	loadLayer->clear();
	layer3D->clear();
	layer2D->clear();
	deleteLayer->clear();
	
	OpenGL_swap();	
}
