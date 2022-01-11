#include "stdio.h"
#include "stdlib.h"
#include "GL/gl.h"
#include "SDL2/SDL.h"
#include "math.h"
#include "vec.h"
#include "globals.h"
#include <string>
using std::string;
#include <vector>
using std::vector;

SDL_Window *mainWindow;

#include "camera.h"
#include "inputController.h"
#include "paint.h"
#include "textureLoader.h"
#include "modelLoader.h"
#include "terrain.h"

#include "fonts.h"
#include "atlas.h"
#include "simplemath.h"
#include "stringUtils.h"
#include "event.h"
#include "GUI.h"
#include "globals.h"
#include "btBulletDynamicsCommon.h"
#include "physics.h"
#include "selfaware.h"
#include "scene.h"

GUIbase *GUI = 0;
int ticks = 0;
vector<int> frametimes; //in microseconds
bool physics = true;

void renderPhysbody(physbody *p){
	pushRenderOptions();
	setModel(p->M);
	drawModel3D(p->getPos(),p->getRot(),p->getScale());
	popRenderOptions();
}

void renderFrametimes(vec2i pos){
	vec2i size = {50,30};
	pushRenderOptions();
	setAlpha(255);
	setColor({0,0,0});
	drawRectOutline(rect().setStart(pos).setSize({frametimes.size()+1,size.y+2}));
	//setAlpha(128);
	setColor({255,0,0});
	int usWhenFpsIs30 = 1000000 / 30;
	for(int I = 0; I < frametimes.size(); I++){
		float timeratio = (float)frametimes[I] / usWhenFpsIs30;
		float fps = 1000000.0/frametimes[I];
		if(fps >= 60){setColor({0,255,0});}
		else if(fps >= 30){setColor({255,255,0});}
		else{setColor({255,0,0});}
		drawLine2D({pos.x+1+I,pos.y+1},{pos.x+1+I,pos.y+1+(0.5+(float)size.y*timeratio)});
	}
	popRenderOptions();
}

void renderFPS(vec2i pos){
	pushRenderOptions();
	setAlpha(255);
	int time = frametimes[frametimes.size()-1];
	float fps = 1000000.0/time;
	if(fps >= 60){setColor({0,255,0});}
	else if(fps >= 30){setColor({255,255,0});}
	else{setColor({255,0,0});}
	setTextPos(pos);
	printw("%.1f FPS",fps);
	popRenderOptions();
}

void renderTriangle(){
	//glTranslatef(0.0,0.0,(double)ticks/1000.0);
	glBegin(GL_TRIANGLES);
		//glColor3ub(255,0,0);
		glColor3ub(255,0,0);	//up, red
		//glVertex3f(1.0,0,0.5);
		glVertex3f(0.5,0,0);
		
		glColor3ub(0,255,0);	//right, green
		//glVertex3f(1.0,-0.5,-0.5);
		glVertex3f(-0.5,-0.5,0);
		
		glColor3ub(0,0,255);	//left, blue
		//glVertex3f(1.0,0.5,-0.5);
		glVertex3f(-0.5,0.5,0);
	glEnd();
}

void renderGrid(){
	float length = 100;
	float width = 100;
	float z = 0;
	vec3f min = {-length/2,-width/2,z};
	vec3f max = {length/2,width/2,z};
	int alpha1 = clamp((float)12.5*255/(camera.pos.z),0,255);
	int alpha2 = clamp((float)2.5*255/(camera.pos.z),0,255);
	int alpha3 = clamp((float)0.5*255/(camera.pos.z),0,128);
	float thick1 = 1;//sqrt(camera.pos.z);
	float thick2 = 1;//sqrt(camera.pos.z);
	float thick3 = 1;//sqrt(camera.pos.z)/2;
	pushRenderOptions();
	setColor({255,255,255});
	float div = 10;
	for(int I = 0; I < div; I++){
		setAlpha(alpha1);
		setThickness(thick1);
		float x = min.x+I*length/div;
		float y = min.y+I*width/div;
		drawLine3D({x,min.y,z},{x,max.y,z});
		drawLine3D({min.x,y,z},{max.x,y,z});
		
		for(int J = 0; J < div; J++){
			setAlpha(alpha2);
			setThickness(thick2);
			float x2 = x+J*length/div/div;
			float y2 = y+J*width/div/div;
			drawLine3D({x2,min.y,z},{x2,max.y,z});
			drawLine3D({min.x,y2,z},{max.x,y2,z});
			
			for(int K = 0; K < div; K++){
				setAlpha(alpha3);
				setThickness(thick3);
				float x3 = x2+K*length/div/div/div;
				float y3 = y2+K*width/div/div/div;
				drawLine3D({x3,min.y,z},{x3,max.y,z});
				drawLine3D({min.x,y3,z},{max.x,y3,z});
			}
		}
	}	
	setAlpha(alpha1);
	setThickness(thick1);
	drawLine3D({max.x,min.y,z},{max.x,max.y,z});
	drawLine3D({min.x,max.y,z},{max.x,max.y,z});
	/* for(float y = min.y; y < max.y; y+=width/10){
		setAlpha(255);
		setThickness(3);
		drawLine3D({min.x,y,z},{max.x,y,z});
		for(float y2 = y; y2 < y+width/10; y2+=width/100){
			setAlpha(128);
			setThickness(2);
		drawLine3D({min.x,y2,z},{max.x,y2,z});
			for(float y3 = y2; y3 < y2+width/100; y3+=width/1000){
				setAlpha(64);
				setThickness(1);
				drawLine3D({min.x,y3,z},{max.x,y3,z});
			}
		}
	}	
 */
		
	popRenderOptions();
}

void renderFeeler(){
	vec3f start = camera.pos;
	vec3f dir = camera.forward();
	vec3f end = start+dir*1000;
	physbody *b;
	float dist;
	vec2i pos = {20,200};
	setFont(getFont("calibri",18));
	if(raytrace(start,end,&b,&dist)){
		end = start+dir*dist;
		setColor({0,255,0});
		setTextPos(pos);
		printw("dist = %f",dist);
		
		setTextPos(pos+(vec2i){0,15});
		printw("pos = %f,%f,%f",end.x,end.y,end.z);
		
		setTextPos(pos+(vec2i){0,15}*2);
		if(b){
			setColor({0,255,0});
			printw("body = %p",b);
		}else{
			setColor({255,0,0});
			printw("no body");
		}
	}else{
		setColor({255,0,0});
		setTextPos(pos);
		printw("no hit");
	}
}

void renderCrosshair(){
	setColor({0,0,0});
	vec2i scr = getScreenSize();
	drawLine2D(scr/2+(vec2i){0,-10},scr/2+(vec2i){0,10});
	drawLine2D(scr/2+(vec2i){-10,0},scr/2+(vec2i){10,0});
}
struct r_atlas{
	atlas A;
	texture t;
};

r_atlas Atl[3];
string dbgAtlasTextureName;
atlas dbgAtlas;

void renderAtlas(r_atlas A, vec2i pos){
	setColor({255,0,0});
	for(int I = 0; I < A.A.UVs.size(); I++){
		rect UV = A.A.UVs[I];
		drawRectOutline(UV.moveBy(pos));
	}
	setColor({255,255,255});
	texture t = A.t;
	setTexture(t);
	drawTexturedRect(rect(t.size()).moveTo(pos));
	drawRectOutline(rect(t.size()).moveTo(pos));
	
}
void render3D(){
	setColor({255,0,0},{0,255,0},{0,0,255});
	//drawTriangle3D({1.0,0,0.5},{1.0,-0.5,-0.5},{1.0,0.5,-0.5});
	//renderGrid();
	drawTriangle3D({0.5,0,-0.2},{-0.5,-0.5,-0.2},{-0.5,0.5,-0.2});
	
	setColor({255,255,255});
	setModel(getModel("box"));
	//setColor({255,0,0});
	drawModel3D({0,0,0},{0,0,0},((vec3f){1,1,1})*0.8);
	//setColor({0,255,0});
	drawModel3D({0.5,1.5,0},{0,0,0},((vec3f){1,1,1})*0.4);
	//setColor({0,0,255});
	drawModel3D({-0.75,1.75,0},{45,0,0},((vec3f){1,1,1})*0.2);
	
	setModel(getModel("terrain"));
	//drawModel3D({-10,-10,0},{0,0,0},{1,1,1});
	drawModel3D({0,0,0},{0,0,0},{1,1,1});
	
	for(int I = 0; I < physbodies.size(); I++){
		renderPhysbody(physbodies[I]);
	}
	renderPhysics();

	scene.render();
	//setModel(getModel("chair"));
	//drawModel3D({0,1,1},{0,0,0},((vec3f){1,1,1})*0.1);
}
void render2D(){
	setColor({255,0,0});
	drawRectOutline(rect({20,20},{40,100}));
	drawRect(rect({40,100},{20,60-80*clamp(camera.rot.y/180.0,-0.5,0.5)}));
	
	setColor({255,255,255});
	setTexture(getTexture("milkyway"));
	drawTexturedRect(rect(64,64).moveTo({20,120}));
	
	setColor({255,0,0});
	int height;
	int width;
	SDL_GetWindowSize(mainWindow, &width, &height);
	setFont(getFont("calibri",18));
	string S = "window size: ";
	S = S+itoa(width)+" x "+itoa(height);
	setTextPos({50,15});
	printText2D(S.c_str());
	
	setTextScale(1.0);
	setFont(getFont("calibri",11));
	setColor({0,255,0});
	setTextPos({50,30});
	printw("pos x:%.3f, y:%.3f, z:%.3f\n",camera.pos.x,camera.pos.y,camera.pos.z);
	setTextPos({50,45});
	printw("rot x:%.0f, y:%.0f, z:%.0f\n",camera.rot.x,camera.rot.y,camera.rot.z);
	renderFPS({50,60});
	renderFrametimes({50,75});
	renderFeeler();
	setTextPos({20,250});
	if(physics){setColor({0,255,0});printw("physics on");}else{setColor({255,0,0});printw("physics off");}
	renderCrosshair();
	//rect R1 = rect().setStart({300,150}).setSize({100,100});
	//rect R2 = rect().setStart({350,200}).setSize({150,100});
	//rect R3 = R1.clamp(R2);
	// setAlpha(128);
	// setColor({255,0,0});
	// drawRectOutline(R1);
	// setColor({0,0,255});
	// drawRectOutline(R2);
	// setColor({0,255,0});
	// drawRectOutline(R3);
	// setAlpha(255);
	
}
void rendertick(){
	setColor({125,206,250});
	clearScreen();
	initRenderOptions();
	//firstn(15,printf("----------frame %d----------\n",firstn));
	//onceon(16,printf("---frame count suppressed---\n"););
	//once(printf("once-test\n"));
	//onceon(5,printf("once-on-5th\n"));
	
	camera.go3D();
	
	glEnable(GL_DEPTH_TEST);
	//reposition the lights
	
	render3D();
	
	camera.go2D();
	glDisable(GL_DEPTH_TEST);
	render2D();
	if(GUI){GUI->renderLogic();}
	
	SDL_GL_SwapWindow(mainWindow);
}
void sysMessageTick(){
	SDL_Event sdl_event;
	while(SDL_PollEvent(&sdl_event)){
		eventKind event;
		switch(sdl_event.type){
		case(SDL_QUIT):
			exit(0);
			break;
		case(SDL_KEYDOWN):
			event.type = EVENT_KEY_DOWN;
			event.keyboard.keycode = sdl_event.key.keysym.sym;
			event.keyboard.key = SDL_GetKeyName(sdl_event.key.keysym.sym);
			goto dispatchEvent;
			break;
		case(SDL_KEYUP):
			event.type = EVENT_KEY_UP;
			event.keyboard.keycode = sdl_event.key.keysym.sym;
			event.keyboard.key = SDL_GetKeyName(sdl_event.key.keysym.sym);
			goto dispatchEvent;
			break;
		case(SDL_MOUSEMOTION):
			event.type = EVENT_MOUSE_MOVE;
			event.mousemove.diff = {sdl_event.motion.xrel,sdl_event.motion.yrel};
			event.mousemove.pos = {sdl_event.motion.x,sdl_event.motion.y};
			goto dispatchEvent;
			break;
		case(SDL_MOUSEBUTTONDOWN):
			event.type = EVENT_MOUSE_BUTTON_DOWN;
			event.mousebutton.button = sdl_event.button.button;
			goto dispatchEvent;
			break;
		case(SDL_MOUSEBUTTONUP):
			event.type = EVENT_MOUSE_BUTTON_UP;
			event.mousebutton.button = sdl_event.button.button;
			goto dispatchEvent;
			break;
		case(SDL_MOUSEWHEEL):
			event.type = EVENT_MOUSE_WHEEL;
			event.mousewheel.x = sdl_event.wheel.x;
			event.mousewheel.y = sdl_event.wheel.y;
			goto dispatchEvent;
			break;
		default:
			//unknown event:
			//ignore.
			break;
		}
		continue;
		dispatchEvent:
		inputController.onEvent(event);
		if(GUI){GUI->onEvent(event);}
	}
}

void thinktick(){
	inputController.think();
}
void initGL();
void initFonts();
void openGUI();

// =================================== MAIN =====================
int main(int argc, char **argv){
	debuginit();
	initGL();
	initFonts();
	initRenderOptions();
	setbuf(stdout, 0);
	
	printf("\nbeep boop\n");
	if(argc){printf("program called from [%s]\n",argv[0]);}else{printf("argc=0\n");}
	printf("sizeof(bitmap) = %d\n",sizeof(bitmap));
	printf("sizeof(texture) = %d\n",sizeof(texture));
	printf("sizeof(textureHandle) = %d\n",sizeof(textureHandle));
	printf("sizeof(font) = %d\n",sizeof(font));
	
	for(int I = 0; I < 60; I++){
		frametimes.push_back(0);
	}
	//openGUI();
	vec3f tscale = {50,50,5};
	vec3f tpos = {0,0,-2.5};
	initPhysics();
	printPhysics();
	spawnTerrain("heightmap128","bricks",tscale,tpos);
	//spawnPlayer({0,0,1});
	//getModel("chair");
	//printf("init done, %d kb used\n",debug_mem_allocated/1000);
	scene.setColors({{255,0,0},{0,255,0},{0,0,255},{255,0,0},{0,0,255}});
	scene.setVertices({{0,0,1},{0,1,1},{0,1,2},{0,0,1},{0,1,2},{0,0,2}});
	scene.drawTriangles();
	scene.setColors({{128,0,255}});
	scene.setVertices({{1,0,1},{2,0,1},{1,0,2},{2,0,2}});
	scene.drawLines();
	while(1){
		//once(profileStart());
		//onceon(2,profileStart());
		sysMessageTick();
		thinktick();
		if(physics){physicstick();}
		profileStart();
		rendertick();
		frametimes.push_back(profileEnd().time);
		frametimes.erase(frametimes.begin());
		ticks++;
		
		//once(debugProfile P = profileEnd();printf("tick done in %ds, %dms, %dus\n", P.seconds, P.milliseconds, P.microseconds));
		//onceon(12,debugProfile P = profileEnd();printf("10 ticks done in %ds, %dms, %dus\n", P.seconds, P.milliseconds, P.microseconds));
		//return 0;		
		SDL_Delay(15);
	}
		
	return 0;
}
// =================================== /MAIN ====================
void initFonts(){
	initFreeType();
	//working sizes: 10,11,18,24
	getFont("calibri",11);
	getFont("calibri",18);
	getFont("calibri",24);
	//setFont(getFont("resource/calibri.ttf",18));
	
	//loadFont("resource/calibri.ttf", 24);
}
void initGL(){
	
	
	
	//==============================
	//debug("SDL: [%s]\n", SDL_GetError());
	SDL_Init(SDL_INIT_VIDEO);
	
	//debug("SDL: [%s]\n", SDL_GetError());
	//debug("hello\n");
	atexit(SDL_Quit);
	SDL_GLContext mainContext;
	
	mainWindow = SDL_CreateWindow("Hai",
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		512, 512, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	//debug("SDL: [%s]\n", SDL_GetError());
	mainContext = SDL_GL_CreateContext(mainWindow);
	//debug("SDL: [%s]\n", SDL_GetError());
	//we have GL context
	//glewExperimental = GL_TRUE;
	//glewInit();
	//debug("SDL: [%s]\n", SDL_GetError());
	
	SDL_GL_SetSwapInterval(1);
	
	glClearColor(0.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	
	const unsigned char *str = glGetString(GL_VERSION);
	//debug("SDL: %s", SDL_GetError());
	printf("\nOpenGL version: [%s]\n",str);
	
	int profile;
	const char *profileStr = "???";
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);
	switch(profile){
		case(SDL_GL_CONTEXT_PROFILE_CORE):
			profileStr = "CORE";
			break;
		case(SDL_GL_CONTEXT_PROFILE_COMPATIBILITY):
			profileStr = "COMPATIBILITY";
			break;
		case(SDL_GL_CONTEXT_PROFILE_ES):
			profileStr = "ES";
			break;
	}
	printf("OpenGL profile: %s (%d)\n",profileStr, profile);
}
void openGUI(){
	GUI = new GUIbase();
	GUI->addChild((new GUIframe())\
		->setSize({200,300})\
		.moveTo({100,20})\
		.addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			.setInnerSize({300,600})\
			.setHorizontal(false)\
			.addChild((new GUIlabel())->setText("Hello World").sizeToContents().moveTo({20,20}))\
			.addChild((new GUIimage())->setImage(getTexture("printer")).setSize({128,128}).moveTo({20,64}))\
			.addChild((new GUIbutton())->setFunction([](){GUI->moveTo(GUI->area.start+(vec2i){5,0});})\
						.setText("boop").sizeToContents().moveTo({20,200}))\
			)\
		)\
	.addChild((new GUIwindow())\
		->setSize({150,100})\
		.moveTo({350,20})\
		.addChild((new GUIlabel())->setText("Yer a wizzard Harry!"))\
	);
}

/*
	todo:
	- make lights toggle-able, and make sure 3D2D elements are
	drawn without lights if they end up gray/black.
	
	
*/

/*
	problems:
	- if I ask for a specific OpenGL version using SDL_GL_SetAttribute(), I get segfault once glGenVertexArrays(1, &VertexArrayID) is called.
	
	- immediately after SDL_Init(), SDL_GetError() gives:
	Failed loading SHCORE.DLL: ╨¥╨╡ ╨╜╨░╨╣╨┤╨╡╨╜ ╤â╨║╨░╨╖╨░╨╜╨╜╤ï╨╣╨╝╨╛╨┤╤â╨╗╤î
	
	- mutator semantics are ambiguous and cause bugs (bitmap.convert vs bitmap = bitmap.convert)
	
*/
//bitmap and atlas test
	/* bitmap BMP1,BMP2,BMP3,BMP4,BMP5,BMP6;
	BMP1 = loadImage("resource/milkyway.jpg");
	BMP2 = loadImage("resource/spitfire.jpg");
	BMP3 = loadImage("resource/helicopter.jpg");
	BMP4 = generateGlyphBitmap('A');
	makeTexture("font_calibri",uploadImage(BMP4));
	printf("font_calibri dims:%s\n",getTexture("font_calibri").UV.toString());
	
	vector<bitmap> BMPs;
	for(int I = 0; I < 255; I++){
		if(isprint(I)){
			bitmap BMP7 = generateGlyphBitmap(I);
			BMPs.push_back(BMP7);
		}
	}
	//BMPs.push_back(BMP1);
	//BMPs.push_back(BMP2);
	//BMPs.push_back(BMP3);
	//BMPs.push_back(BMP4);
	BMP5 = blankBitmap(96,96,TL_RGBA);
	Atl[0].A = makeAtlas(BMPs,BMP5,false);
	makeTexture("atlas",uploadImage(Atl[0].A.BMP));
	Atl[0].t = getTexture("atlas");
	printf("atlas dims:%s\n",getTexture("atlas").UV.toString());
	
	
	BMP6 = blankBitmap(96,96,TL_RGBA);
	Atl[1].A = makeAtlas(BMPs,BMP6,true);
	makeTexture("atlas2",uploadImage(Atl[1].A.BMP));
	Atl[1].t = getTexture("atlas2");
	
	Atl[2].A = makeAtlasPOT(BMPs,true);
	makeTexture("atlas3",uploadImage(Atl[2].A.BMP));
	Atl[2].t = getTexture("atlas3"); */
	//end test (ok good enough)
	
	

