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
#include "ecs.h"
#include "keybinds.h"
#include "octree.h"
#include "mouse.h"
#include "hook.h"
#include "console.h"
#include "picker.h"
#include "physbox.h"
#include "editmodel.h"
#include "input.h"

void setupLayer3D();
void setupLayer2D();
//renderLayer *currentLayer;

model *m;
GUIbase *GUI = 0;
float fps = 0;
float frametime = 0;
void openGUI();
void tick();

frameprinter *fprint;
//entity *E;
octree_node *octree_root;
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
//
//  optimize physics using octrees
//	generateBox should return model or emodel
//  model MAY become a handle that associates textures with meshes
//  mesh then would be a condensed representation (rmodel or emodel with no non-essential data)
//
//  select sides of a box and/or texture them independently.
//  have "world" as an entity that can be referenced (contains all static mesh instances)
//  collide point (camera) with world.
//
//  yes/no collision detection vs distance collision detection
//  refine-able collision data? as continuation?

//rmodel *rm2;
//texture *rm2t;




void setupLayers(){	
	setupLayer3D();
	setupLayer2D();
}

void loadAssets(){
	getFont("cour 14");
	setFont(getFont("calibri 18"));
	
	m = getModel("box");
	//uploadModel(mBox);
	m->rm = new rmodel(m);
	m->rm->finalize();
	uploadTexture(m->t);
	uploadRmodel(m->rm);
	
}

void addKeybinds(){
	keybinds->binds["R"].keyDown = [](eventKind event){
		for(auto I = entities.begin(); I != entities.end();){
			if((*I)->body && (*I)->body->type == BODY_DYNAMIC){
				delete *I;
				I = entities.erase(I);
			}else{
				I++;
			}
		}
	};
	keybinds->binds["F"].keyDown = [](eventKind event){
		event.maskEvent();
		physbox("materials/brick2",1.f,1.f,0.2f,0.007f);
	};
	keybinds->binds["G"].keyDown = [](eventKind event){
		event.maskEvent();
		physbox("materials/steel",1.f,1.f,1.f,0.f);
	};
	keybinds->binds["H"].keyDown = [](eventKind event){
		event.maskEvent();
		physbox("materials/steel",0.5f,0.125f,1.f,0.f);
	};
}

void test1render(){
	setLayer(layer3D);
	resetLayer(layer3D);
	setLighting(false);
	setPosition(vec3(0,0,0));
	setPointSize(3);

	//draw 3 points in xy plane
	setColor(vec3(0,0,0));	//black for origin
	drawPoint(vec3(0,0,-10));

	setColor(vec3(255,0,0)); //red for x
	drawPoint(vec3(1,0,-10));

	setColor(vec3(0,255,0)); //green for y
	drawPoint(vec3(0,1,-10));
	
	setColor(vec3(0,0,255)); //blue for z
	drawPoint(vec3(0,0,-1-10));
	
	//make a 3x3x3 point-grid next to it
	setColor(vec3(255,255,255));
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			for(int k = 0; k < 4; k++){
				drawPoint(vec3(i,j,k-5));
			}
		}
	}
	//draw a 3x3x2 box in it
	vec3 start = vec3(0,0,-5);
	vec3 rot = vec3(0,0,0);
	
	vec3 scale = vec3(3,3,2);
	vec3 pos = start+scale/2.f;
	
	setColor(vec3(128,128,128));
	drawBoxWireframe(pos,rot,scale);
	
	//make a 2x2x2 textured cube
	scale = vec3(2,2,2);
	pos = start+scale/2.f;
	static rmodel *rm = 0;
	setLayer(loadLayer);
	if(!rm){
		e_model *em = generateBox(scale,1.f);
		rm = em->getRmodel();
		uploadRmodel(rm);
	}
	static texture *t = 0;
	if(!t){
		t = getTexture("materials/brick3");
		uploadTexture(t);
	}
	setLayer(layer3D);
	resetLayer(layer3D);
	setPosition(pos);
	setScale(vec3(1,1,1));
	setTexture(t);
	setRenderMode(3);
	setColoring(false);
	setTexturing(true);
	drawRmodel(rm);
}

int main(int argc, char **argv){
	printf("Hello World!\n");
	debuginit();
	printf("-------- debug init done ----------\n");
	initEvents();
	initTimers();
	printf("-------- util init done -----------\n");
	window_init(512,512);
	OpenGL_printVersion();
	printf("-------- window init done ---------\n");
	initInput();
	initKeybinds();
	printf("-------- input init done ----------\n");
	renderLowInit();
	initLayers();
	printf("-------- render low init done -----\n");
	initFreeType();
	printf("-------- FreeType init done -------\n");
	loadAssets();
	
	
	//openEditor2();
	
	
	octree_root = new octree_node();
	float worldsize = 500;
	octree_root->volume = AABB(-vec3(0.5,0.5,0.5)*worldsize,vec3(0.5,0.5,0.5)*worldsize);
	//renderTest1();
	camera.setPos({-0.5,0,0});
	//renderTest();
	setbuf(stdout,0);
	
	setupLayers();
	addKeybinds();
	
	
	initConsole();
	//cprint("Hello World!\n");
	//cprint("I am uberpone\n");
	//cprint("test test test\n");
	//cprint("beep boop\n");
	//string str = fstring("Hello, %d pi = %f mcguffins, %s\n", 2, 2*3.14f, "brah");
	//cprint(str.c_str());
	
	openGUI();
	setFramePrinter(new frameprinter());
	initPicker();
	
	//rm2 = generateBox(vec3(1,1,10));
	//rm2t = getTexture("materials/brick2");
	//uploadTexture(rm2t);
	//uploadRmodel(rm2);
	//obelisk();
	
	//makeScene1();
	//makeScene1(vec3(-50,-50,-50));
	
	makeScene2(vec3(-100,-100,-100));
	
	printf("-------- frames begin ----------\n");
	
	printf("\n");
	
	new ecs_render_system_kind();
	new ecs_physics_system_kind();
	new ecs_collision_system_kind();
	physicsOn = true;//false;
	while(1){
		profile(tick(),frametime);
		if(frametime){fps = 1.0f/frametime;}
	}
	return 0;
}

extern int num_rmodels;
extern int num_textures;
extern long bytes_rmodels;
extern long bytes_textures;
void tick(){
	eventKind e; e.type = EVENT_FRAME;
	globalChannel->publishEvent(e);
	frameprint(string()+"entities: "+(int)entities.size());
	frameprint(string()+"rmodels: "+num_rmodels+", "+((double)bytes_rmodels)/1024.f/1024.f+" MB");
	frameprint(string()+"textures: "+num_textures+", "+((double)bytes_textures)/1024.f/1024.f+" MB");
	renderTick();
	//renderTest1Tick();
	//inputController.think();
	sysMessageTick();
	//SDL_Delay(1);
}

void openTestWindow1(){
	texture *img1 = getTexture("art/printer"); uploadTexture(img1);
	GUI->addChild((new GUIframe())\
		->setSize({200,300})\
		->moveTo({100,20})\
		->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({300,600})\
			->setHorizontal(false)\
			->addChild((new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20}))\
			->addChild((new GUIimage())->setImage(img1)->setSize({128,128})->moveTo({20,64}))\
			->addChild((new GUIbutton())->setFunction([](){GUI->moveTo(GUI->area.start+(vec2){5,0});})\
						->setText("boop")->sizeToContents()->moveTo({20,200}))\
			)\
		)\
	->addChild((new GUIwindow())\
		->setSize({150,100})\
		->moveTo({350,20})\
		->addChild((new GUIlabel())->setText("Yer a wizzard Harry!")->sizeToContents())\
	);
}

void openTestWindow2(){
	texture *img1 = getTexture("art/printer"); uploadTexture(img1);
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
			->addChild((new GUIbutton())->setFunction([](){GUI->moveTo(GUI->area.start+(vec2){5,0});})\
							->setText("boop")->sizeToContents()->moveTo({20,200}))\
			->addChild(TE)\
			)\
		);
}

void openTestWindow3(){
	texture *tex1 = getTexture("art/printer"); uploadTexture(tex1);
	auto TE = new GUItextEntry();
	TE->setFunction([=](){printf("Text:[%s]\n",TE->text.c_str());})->setSize({96,24})->moveTo({20,220});

	GUIgrid *grid = new GUIgrid();
	GUIbase *lbl1 = (new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20});
	GUIbase *img1 = (new GUIimage())->setImage(tex1)->setSize({128,128})->moveTo({20,64});
	GUIbase *btn1 = (new GUIbutton())->setFunction(
							[](){
								GUI->moveTo(GUI->area.start+(vec2){5,0});
							}
						)->setText("boop")->sizeToContents()->moveTo({20,200});
	
	printf("------------ grid setup sterted\n");
	grid->addChild(lbl1);
	grid->addChild(img1);
	grid->addChild(btn1);
	grid->addChild(TE);
	
	grid->grid(lbl1,0,0,2);
	grid->grid(img1,1,0,2);
	grid->grid(btn1,2,1);
	grid->grid(TE,  2,0);
	printf("------------ grid setup done\n");
	
	GUIbase *window = (new GUIwindow())\
		->setSize({100,100})\
		->moveTo({100,20})\
		->addChild(grid);

	GUI->addChild(window);
	GUI->invalidateTree();
	printf("---- sizing window\n");
	window->sizeToContents();
	printf("---- window sized\n");
	GUI->invalidateTree();
		//->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({300,600})\
			->setHorizontal(false)\
			->addChild(grid)\
		)\
	);
	printf("------------ GUI setup done\n");
}

void openTestWindow4(){
	renderLayer *test4layerSetup = new renderLayer();
	renderLayer *test4layer = new renderLayer();
	addLayer(layer3D,test4layerSetup);
	addLayer(test4layerSetup,test4layer);
	setLayer(test4layerSetup);
	setTexturing(false);
	setColoring(true);
	setTransparency(false);
	setLighting(false);
	setRenderMode(2);
	setColor(vec3(255,255,0));
	setLayer(test4layer);
	
	GUIwindow *win = new GUIwindow();
	win->moveTo({250,40});
	
	GUItextEntry *te_x = new GUItextEntry();
	te_x->setSize({50,20});
	te_x->setText("1");
	GUItextEntry *te_y = new GUItextEntry();
	te_y->setSize({50,20});
	te_y->setText("1");
	GUItextEntry *te_z = new GUItextEntry();
	te_z->setSize({50,20});
	te_z->setText("1");
	
	GUIbutton *btn_box = new GUIbutton();
	btn_box->setText("box");
	btn_box->setSize({150,30});
	GUIbutton *btn_cyl = new GUIbutton();
	btn_cyl->setText("cyllinder");
	btn_cyl->setSize({150,30});
	GUIbutton *btn_sph = new GUIbutton();
	btn_sph->setText("sphere");
	btn_sph->setSize({150,30});
	GUIbutton *btn_con = new GUIbutton();
	btn_con->setText("cone");
	btn_con->setSize({150,30});
	GUIbutton *btn_clear = new GUIbutton();
	btn_clear->setText("CLEAR\n");
	btn_clear->setSize({150,30});
	
	GUIgrid *grid = new GUIgrid();
	grid->addChild(te_x);
	grid->addChild(te_y);
	grid->addChild(te_z);
	grid->addChild(btn_box);
	grid->addChild(btn_cyl);
	grid->addChild(btn_sph);
	grid->addChild(btn_con);
	grid->addChild(btn_clear);
	grid->grid(te_x,0,0);
	grid->grid(te_y,0,1);
	grid->grid(te_z,0,2);
	grid->grid(btn_box,1,0,3);
	grid->grid(btn_cyl,2,0,3);
	grid->grid(btn_sph,3,0,3);
	grid->grid(btn_con,4,0,3);
	grid->grid(btn_clear,5,0,3);
		
	win->addChild(grid);
	win->sizeToContents();
	GUI->addChild(win);
	
	btn_box->F = [=](){
		float numx = te_x->getNumber();
		float numy = te_y->getNumber();
		float numz = te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make box %s\n",toString(size).c_str());
		rmodel *rm = generateBox(size)->getRmodel(1);
		setLayer(loadLayer);
		uploadRmodel(rm);
		setLayer(test4layer);
		drawRmodel(rm);
		printf("wut\n");
	};
	btn_cyl->F = [=](){
		float numx = te_x->getNumber();
		float numy = te_y->getNumber();
		float numz = te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make cyllinder %s\n",toString(size).c_str());
		rmodel *rm = generateCyllinder(size.x,size.z)->getRmodel(1);
		setLayer(loadLayer);
		uploadRmodel(rm);
		setLayer(test4layer);
		drawRmodel(rm);
		printf("wut\n");
	};
	btn_sph->F = [=](){
		float numx = te_x->getNumber();
		float numy = te_y->getNumber();
		float numz = te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make sphere %s\n",toString(size).c_str());
		rmodel *rm = generateSphere(size.x)->getRmodel(1);
		setLayer(loadLayer);
		uploadRmodel(rm);
		setLayer(test4layer);
		drawRmodel(rm);
		printf("wut\n");
	};
	btn_con->F = [=](){
		float numx = te_x->getNumber();
		float numy = te_y->getNumber();
		float numz = te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make cone %s\n",toString(size).c_str());
		rmodel *rm = generateCone(size.x,size.z)->getRmodel(1);
		setLayer(loadLayer);
		uploadRmodel(rm);
		setLayer(test4layer);
		drawRmodel(rm);
		printf("wut\n");
	};
	btn_clear->F = [=](){
		clearLayer(test4layer);
	};
}

renderLayer *layerGUI;
void openGUI(){
	layerGUI = new renderLayer();
	addLayer(layer2D,layerGUI);
	GUI = new GUIbase();
	inputChannel->addListenerFront(GUI);
	//openTestWindow3();
	//openTestWindow4();
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

void setupLayer3D(){
	setLayer(layer3D->resetLayer);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setDepthTest(true);
	setDepthMask(true);
	setLighting(true);
	setSunPos(vec3(0.5,0.75,1));
	setSunColor(0.9f*vec3(1,1,1));
	setAmbientColor(0.3f*vec3(1,1,1));
	
	setColoring(true);
	setTexturing(false);
	setScissoring(false);
	setTransparency(false);
	setAlpha(255);
	setColor({255,255,255});
}

void setupLayer2D(){
	setLayer(layer2D->resetLayer);
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setColoring(false);
	setTransparency(false);
	setTexturing(false);
	setScissoring(false);
	vec2 scr = getScreenSize();
	setScissor(rect(vec2(0,0),scr));
	setDepthTest(false);
	setDepthMask(true);
	setLighting(false);
}

void go3D(){
	camera.go3D();
	setProjection(camera.getProjection());
}

void go2D(){
	camera.go2D();
	setProjection(camera.getProjection());
}

void renderTick(){
	//glClearColor(0.3,0.7,0.9,1.0);
	//glClearColor(0.1,0.23,0.3,1.0);
	//========================= 3D ==============================
	
	setLayer(layer3D);
	go3D();
	clearScreen();
	drawTriangle(vec3(0,-0.67,-0.5),vec3(0,0,0.5),vec3(0,0.67,-0.5));
	setColoring(true);
	setTextPos(vec2(0,0));
	printText("Hello World [ ijk XYZ o_O ] ");
	
	setColoring(false);
	setTexturing(true);
	setTexture(m->t);
	drawRmodel(m->rm);
	renderComment("comment: layer3D done\n");
	//octreeRender(octree_root);
	
	//done in ecs_collision_system now
	//broadphaseinfo *bp = checkCollisionBroadphase(octree_root);
	//frameprint(fstring("bp: %p",bp));
	//if(bp){frameprint(fstring("%d bodies, %d pairs",bp->bodies.size(),bp->pairs.size()));}
	//broadphaseRender(bp);
	//delete bp;
	//test1render();
	//octreePrint(octree_root);
	
	//getComponent(E,position)->val = vec3(0,1,3+0.25*sin(t2));
	//why the fuck is this in render
	
	//========================= 2D ==============================
	//setColor({255,255,255});
	//drawRmodel(m->rm);
	//printw("Hello World!");
	//textPos = {0.0f,25.0f};//{100.0f,100.0f};
	//renderCmd(RCMD::DEBUG,b=true);
	//setColoring(false);
	//setTextPos(vec2(0,0));
	//printText("Hello World [ ijk XYZ o_O ] ");
	setLayer(layer2D);
	go2D();
	frameprint("Hello World [ ijk XYZ o_O ] ");
	
	//setTextPos(vec2(0,20));
	static float fps_filtered = 60.f;
	static float frametime_filtered = 0.015f;
	fps_filtered = mix2(fps_filtered,fps,1.0f/200.0f);
	frametime_filtered = mix2(frametime_filtered,frametime,1.0f/200.0f);
	//printText(string("FPS: ")+ftoa(fps_filtered,1)+", frametime: "+ftoa(1000*frametime_filtered,3)+"ms");//(int)(1000*round2(frametime_filtered,4))
	frameprint(string("FPS (filtered): ")+ftoa(fps_filtered,1)+", frametime (filtered): "+ftoa(1000*frametime_filtered,3)+"ms");//(int)(1000*round2(frametime_filtered,4))
	frameprint(string("FPS: ")+ftoa(fps,1)+", frametime: "+ftoa(1000*frametime,3)+"ms");
	//setTextPos(vec2(0,40));
	//printText(string("T1: ")+t);
	//setTextPos(vec2(0,60));
	//printText(string("cam speed: ")+inputController.velocity.length());
	frameprint(fstring("T1: %d",getGameTicks()));
	frameprint(string("cam speed: ")+length(inputController->velocity));
	if(GUI){GUI->renderLogic();} //should be event-driven
	
	
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
	loadLayer->reset();
	layer3D->clear();
	layer3D->reset();
	layer2D->clear();
	layer2D->reset();
	deleteLayer->clear();	//this should be either in paint or a layer's option
	deleteLayer->reset();
	
	OpenGL_swap();	//this should be moved to renderLow
}
