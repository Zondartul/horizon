//-#include "main/control.h"
#include <gl/gl.h>
#include <cstdio>
#include <math.h>
#include <ctype.h>
#include <algorithm>
#include <iostream>
//#include <string>
//#include <sstream> 
//using namespace std;
//#include <vector>
#include "util/globals.h"
#include "util/messenger.h"
#include "resource/textureloader.h"
#include "resource/fonts.h"
#include "util/debug.h"
#include "input/input.h"
#include "input/inputMessage.h"
//inputKind input;
//PSchannel GUI_PS;
//MessageChannel GUI_MC;
//#include "Gui.h"
#include "display/paint.h"
#include "gui/Gui2.h"
#include "gui/Gui3.h"
#include "gui/Gui4.h"
#include "gui/Gui5.h"
#include "math/vectors.h"
#include "resource/models.h"
#include "math/quaternions.h"
#include "util/hook.h"
#include "gui/newconsole.h"
//#include "gui/console.h"
#include "gui/valscreen.h"
#include "input/keybinds.h"
#include "game/camera.h"
#include "display/renderable.h"
#include "gui/window_modeller.h"
#include "gui/window_gui_editor.h"
//global vars go here
//LETS DO QUATERNIONS LIKE A BOSS

//GUIManager GUIM;

GUI2base *GUI;
GUI2frame *myFrame;
GUI5base *GUI5;
//model *myModel;
//vec SomeVec1; //cam pos.
//vec SomeVec2;
//vec2i deltaMouse;
//vec2i windowCorner;
//vec2i windowSize;
//vec2i windowCenter;
//quat CamAngle;
//quat CamAngTest;
//double Camnorm;
//double camSpeed;
bool renderWireframe;
//int EntLookAt;
bool mouseCapture;
bool camRotOn;
//HWND hwnd;
SDL_Window *window;
SDL_GLContext glcontext;
//#include "physics.h"
//#include "console.h"
/*
void myButton(void *holder)
{
	GUIM.axe(NULL);
}
*/

struct KeyBind
{
	funcptr onPress;
	funcptr onRelease;
	int mode; //1 - press/release, 2-rising edge, 3-falling edge, 4-every frame, 5-windowsy.
	bool keyDown;
	KeyBind()
	{
		onPress = NULL;
		onRelease = NULL;
		keyDown = false;
		mode = 1;
	}
};

KeyBind Binds[256];
void bindKey(unsigned char key, funcptr onPress, funcptr onRelease, int mode)
{
	Binds[key].onPress = onPress;
	Binds[key].onRelease = onRelease;
	Binds[key].mode = mode;
}

//#include "toolbox.h"


void Test1(void* arg)
{
	cprint("I like turtles!");
	//ConsoleParse("I LIKE TURTLES");
}


void OpenMenuModel()
{
	/*
	GUIframe* ModelMenu = new GUIframe;
	ModelMenu->setPos(128,128);
	ModelMenu->setSize(196,128);
	ModelMenu->title = "Model info";
	ModelMenu->setParent(GUI);
	
	GUIlabel* Mtext = new GUIlabel;
	Mtext->setPos(4,32);
	char text[128];
	sprintf(text, "Model: %p\nVerticles: %d\nTriangles: %d\nTextures: %d", myModel, myModel->numtris*3,
																		myModel->numtris, 0);//myModel->numtextures);
	Mtext->text = text;
	Mtext->setParent((GUIbase*) ModelMenu);
	*/
}




void sendMsg1(void* arg)
{
	/* message newMsg;
	newMsg.type = "stuff";
	//newMsg.push<string>("apples"); // almost never do this, use .str instead
	newMsg.set(0,string("apples"));
	//newMsg.push(arg);
	newMsg.set(1,arg);
	GUI_PS.publish(newMsg); */
}

void OpenMenu1()
{	
	//myModel = NULL;
	myFrame = new GUI2frame;
	myFrame->setPos(0,0);
	myFrame->setSize(256,512);
	myFrame->title = "Frame 1";
	myFrame->setParent(GUI);
	
	
	GUI2frame* myFrame2 = new GUI2frame;
	myFrame2->setPos(0,0);
	myFrame2->setSize(96,96);
	myFrame2->title = "Frame 2";
	myFrame2->setParent((GUI2base*)myFrame);
	
	
	GUI2button* Button = new GUI2button;
	Button->pos.x = 200;
	Button->pos.y = 64;
	Button->size.x = 64;
	Button->size.y = 64;
	Button->color_panel = {0,64,255};
	Button->setParent((GUI2base*)myFrame2);
	
	GUI2label* Label = new GUI2label;
	Label->pos = {4,32};
	Label->size = {256-8,128-34};
	Label->setParent((GUI2base*)myFrame2);
   
	GUI2textEntry* Text = new GUI2textEntry;
	Text->pos = {4, 128};
	Text->setParent((GUI2base*)myFrame);
	
	GUI2checkbox* check = new GUI2checkbox;
	check->pos = {4,196};
	check->setParent((GUI2base*)myFrame);
	
	GUI2spinner* spin = new GUI2spinner;
	spin->pos = {4, 154};
	//spin->vals = {-10,0,10,3};
	spin->vals[0]=-10;spin->vals[1]=0;spin->vals[2]=10;spin->vals[3]=3;
	spin->setParent((GUI2base*)myFrame);
	
	GUI2radiobutton* radio1 = new GUI2radiobutton;
	radio1->pos = {4,256};
	radio1->setParent((GUI2base*)myFrame);
	
	GUI2radiobutton* radio2 = new GUI2radiobutton;
	radio2->pos = {32,256};
	radio2->setParent((GUI2base*)myFrame);
	
	GUI2radiogroup* group = new GUI2radiogroup;
	group->addButton((void*)radio1);
	group->addButton((void*)radio2);
	
	GUI2listbox* list = new GUI2listbox;
	list->addOption("pony",NULL,NULL);
	list->addOption("cupcake",NULL,NULL);
	list->addOption("shipping",NULL,NULL);
	list->setPos(4,290);
	list->setParent((GUI2base*)myFrame);
	
	GUI2dropdownlist* ddlist = new GUI2dropdownlist;
	ddlist->addOption("Pinkie", NULL,NULL);
	ddlist->addOption("Rainbow", NULL,NULL);
	ddlist->addOption("Applejack", NULL,NULL);
	ddlist->setPos(96,290);
	ddlist->setParent((GUI2base*)myFrame);
	
	GUI2slider* slide = new GUI2slider;
	slide->setPos(4,400);
	slide->setParent((GUI2base*)myFrame);
	
	GUI2colorbox* cbox = new GUI2colorbox;
	cbox->setPos(4,430);
	cbox->setParent((GUI2base*)myFrame);
	
	GUI2button* btn1 = new GUI2button;
	btn1->setPos(16,32);
	btn1->setSize(64,64);
	btn1->func = &sendMsg1;
	btn1->arg = (void *)btn1;
	btn1->setParent(myFrame);

	GUI2button* btn2 = new GUI2button;
	btn2->setPos(16+64,32);
	btn2->setSize(64,64);
	btn2->func = &sendMsg1;
	btn2->arg = (void *)btn2;
	btn2->setParent(myFrame);
}
/* 
void OpenVals()
{
	GUI2frame* valframe = new GUI2frame;
	valframe->setSize(256,256);
	valframe->setPos(768,32);
	valframe->setParent(GUI);
	
	GUI2valuedisplay* valx = new GUI2valuedisplay;	GUI2valuedisplay* val2x = new GUI2valuedisplay;
	valx->setPos(0,32);								val2x->setPos(128,32);
	valx->val = (void*)(&CamAngle.w);				val2x->val = (void*)(&CamAngTest.w);
	valx->mode = 'f';								val2x->mode = 'f';
	valx->setParent(valframe);						val2x->setParent(valframe);
	
	GUI2valuedisplay* valy = new GUI2valuedisplay;	GUI2valuedisplay* val2y = new GUI2valuedisplay;
	valy->setPos(0,64);								val2y->setPos(128,64);
	valy->val = (void*)(&CamAngle.v.x);				val2y->val = (void*)(&CamAngTest.v.x);
	valy->mode = 'f';								val2y->mode = 'f';
	valy->setParent(valframe);						val2y->setParent(valframe);
	
	GUI2valuedisplay* valz = new GUI2valuedisplay;	GUI2valuedisplay* val2z = new GUI2valuedisplay;
	valz->setPos(0,96);								val2z->setPos(128,96);
	valz->val = (void*)(&CamAngle.v.y);				val2z->val = (void*)(&CamAngTest.v.y);
	valz->mode = 'f';								val2z->mode = 'f';
	valz->setParent(valframe);						val2z->setParent(valframe);
	
	GUI2valuedisplay* valmx = new GUI2valuedisplay;	GUI2valuedisplay* val2my = new GUI2valuedisplay;
	valmx->setPos(0,128);							val2my->setPos(128,128);
	valmx->val = (void*)(&CamAngle.v.z);			val2my->val = (void*)(&CamAngTest.v.z);
	valmx->mode = 'f';								val2my->mode = 'f';
	valmx->setParent(valframe);						val2my->setParent(valframe);
	
	GUI2valuedisplay* valdmx = new GUI2valuedisplay;GUI2valuedisplay* val2dmy = new GUI2valuedisplay;
	valdmx->setPos(0,128+32);						val2dmy->setPos(128,128+32);
	valdmx->val = (void*)(&deltaMouse.x);			val2dmy->val = (void*)(&Camnorm);
	valdmx->mode = 'd';								val2dmy->mode = 'f';
	valdmx->setParent(valframe);					val2dmy->setParent(valframe);
} */
GUI4base *GUI4;

void displaySizes(void *arg){
	printf("displaySizes called\n");
	vector<GUI4base*> bases = *((vector<GUI4base*>*)arg);
	string text;
	for(int I = 1; I < bases.size(); I++){
		char str[80];
		snprintf(str,80,"[%p]:size %d x %d (%d,%d - %d,%d)\n",
			bases[I],
			bases[I]->area.getw(),
			bases[I]->area.geth(),
			bases[I]->area.getx(),
			bases[I]->area.gety(),
			bases[I]->area.getx2(),
			bases[I]->area.gety2());
		text += str;
	}
	((GUI4label*)(bases[0]))->setText(text);
}

void OpenGUI4(){
	GUI4 = &((new GUI4frame())->setPos(200,100).setSize(600,400).setClientSize(600,400).setClientPos(0,0));
	GUI4base *element1 = &((new GUI4frame())->setPos(10,10).setSize(100,100).setClient(true).setMovable(false).setResizible(true));
	GUI4base *element2 = &((new GUI4label())->setText("Quick Brown Fox\nJumps Over\nThe Lazy Dog").setPos(70,10).setSize(50,50).setClient(true).setSizeToContents(true));
	GUI4base *element3 = &((new GUI4frame())->setPos(10,100).setSize(25,25).setClient(true).setMovable(false).setResizible(true).setAnchor(1,1));
	
	GUI4->addElement(element1);
	GUI4->addElement(element2);
	GUI4->addElement(element3);
	vector<GUI4base*>* bases = new vector<GUI4base*>;
	bases->push_back(element2);
	bases->push_back(GUI4);
	bases->push_back(element1);
	bases->push_back(element2);
	bases->push_back(element3);
	hook.add("onRender","GUI4 data update",new functor_from_funcptr(displaySizes,(void*)bases));
	GUI4->setSizeToContents(false).setClickable(true).setMovable(false).setResizible(true);//.setClientOrder(ORDER_VERTICAL);
	//GUI4->setMovable(true);
	//GUI4->setClickable(true);
}

void openGUI5(){
	GUI5 = new GUI5base();
	GUI5->setFill(true);
	GUI5base *window = &((new GUI5window())->setTitle("GUI5"));
	window->setPos({200,200}).setSize({500,500}).setDebug(false);
	GUI5tabgroup *tab = new GUI5tabgroup();
	tab->setTitle(0,"text").addElement((*(new GUI5label)).setText("hello").setPos({50,50}).setDebug(false));
	tab->setTitle(1,"btn").addElement((*(new GUI5button)).setText("[PRESS ME]").setPos({50,100}).setDebug(false));
		GUI5scrollbar *scroll = new GUI5scrollbar();
			GUI5frame *frame = new GUI5frame();
			frame->setSize({100,100}).setPos({50,50}).addElement((*(new GUI5image)).setImage("../resource/textures/other/error.png").setPos({50,200}).setDebug(false));
		scroll->addElement(*frame);
		scroll->setVerticalLimits(0,1000).setHorizontalLimits(0,1000);
		scroll->setVertical(true).setHorizontal(false);
	tab->addElement(*scroll);
	tab->addElement((*(new GUI5window)).setTitle("Window!").setPos({200,25}).setSize({200,200}).setDebug(false));
	tab->setDebug(true);
	window->addElement(*tab);
	GUI5->addElement(*window);
	GUI5->subscribeToMessageChannel(&input.channel,"");
}

bool ParseKey(int kb)
{
	if(kb>0)
	{
		switch(Binds[kb].mode)
		{
			case 1://press/release
				if(!Binds[kb].keyDown&&Binds[kb].onPress){Binds[kb].onPress(NULL);}
				break;
			case 2://rising edge
				if(!Binds[kb].keyDown)
				{
					if(Binds[kb].onPress){Binds[kb].onPress(NULL);}
					if(Binds[kb].onRelease){Binds[kb].onRelease(NULL);}
				}
				break;
			case 3://falling edge
			    //do nothing
				break;
			case 4://every tick
				//do nothing, handled elsewere.
				break;
			case 5://whenever?
				if(Binds[kb].onPress){Binds[kb].onPress(NULL);}
				break;
		}
		Binds[kb].keyDown = true;
	}
	else
	{
		kb = -kb;
		switch(Binds[kb].mode)
		{
			case 1://press/release
				if(Binds[kb].keyDown&&Binds[kb].onRelease){Binds[kb].onRelease(NULL);}
				break;
			case 2://rising edge
				//do nothing
				break;
			case 3://falling edge
			    if(Binds[kb].keyDown)
				{
					if(Binds[kb].onPress){Binds[kb].onPress(NULL);}
					if(Binds[kb].onRelease){Binds[kb].onRelease(NULL);}
				}
				break;
			case 4://every tick
				if(Binds[kb].keyDown&&Binds[kb].onRelease){Binds[kb].onRelease(NULL);}
				break;
			case 5://whenever?
				if(Binds[kb].onRelease){Binds[kb].onRelease(NULL);}
				break;
		}
		Binds[kb].keyDown = false;
	}
	if(Binds[kb].onPress||Binds[kb].onRelease){return true;}else{return false;}
}

class BinderKind: public messageReceiver{
	void receiveMessage(message *msg){
		if(msg->type == "key_down"){
			ParseKey(((message_key*)msg)->key[0]);
		}
		if(msg->type == "key_up"){
			ParseKey(-((message_key*)msg)->key[0]);
		}
	}
} Binder;

void ToggleMouseCapture(void* arg){convars["camera_mouse_capture"] = !convars["camera_mouse_capture"];}//{mouseCapture = !mouseCapture; SetCursorPos(windowCenter.x,windowCenter.y);}
void ToggleCamRot(void* arg){camRotOn = !camRotOn;}
class BinderKind;

vector<renderable*> scene;

#include "experimental/testfuncs.h" // that's where "push T and shit happens" goes
font Tahoma8;//no aa
font Tahoma12;//no aa
font Tahoma18;
font Tahoma20;
font Tahoma22;
font Calibri8;
font Calibri12;
font Calibri18;
font Calibri20;
font Calibri22;
font CourierNew8;
font CourierNew12;
font CourierNew18;
font CourierNew20;
font CourierNew22;
void OnProgramStart()
{
	//debug setup
	print_file = 1;
	print_line = 1;
	print_current = 1;
	debugcurrent.push("OnProgramStart");
	debug("hi\n");
	debug("freetype version = %d.%d.%d\n",FF_version_major, FF_version_minor, FF_version_patch);
	debug("OpenGL version: %s\n", glGetString(GL_VERSION));
    theta = 1.0f;
    //if(MessageBox(0,"V: 44. Continue?", "Version", MB_OKCANCEL|MB_ICONQUESTION )==2){PostQuitMessage(0); return;}
    Tahoma8 = GenerateFont("C:/Windows/Fonts/tahoma.ttf",8,true);//no aa
	Tahoma12= GenerateFont("C:/Windows/Fonts/tahoma.ttf",12,false);//no aa
	Tahoma18= GenerateFont("C:/Windows/Fonts/tahoma.ttf",18,true);
	Tahoma20= GenerateFont("C:/Windows/Fonts/tahoma.ttf",20,true);
	Tahoma22= GenerateFont("C:/Windows/Fonts/tahoma.ttf",22,true);
	Calibri8= GenerateFont("C:/Windows/Fonts/calibri.ttf",8,true);
	Calibri12= GenerateFont("C:/Windows/Fonts/calibri.ttf",12,true);
	Calibri18= GenerateFont("C:/Windows/Fonts/calibri.ttf",18,true);
	Calibri20= GenerateFont("C:/Windows/Fonts/calibri.ttf",20,true);
	Calibri22= GenerateFont("C:/Windows/Fonts/calibri.ttf",22,true);
	CourierNew8= GenerateFont("C:/Windows/Fonts/cour.ttf",8,true);
	CourierNew12= GenerateFont("C:/Windows/Fonts/cour.ttf",12,true);
	CourierNew18= GenerateFont("C:/Windows/Fonts/cour.ttf",18,true);
	CourierNew20= GenerateFont("C:/Windows/Fonts/cour.ttf",20,true);
	CourierNew22= GenerateFont("C:/Windows/Fonts/cour.ttf",22,true);
	
	setFont(Calibri18);
	debug("fonts done\n");
	paintInit();

	mouseCapture = false;
	camRotOn = true;

	renderWireframe = false;
	bground.r = 142;
	bground.g = 187;
	bground.b = 255;
	bground.a = 255;

	
	// vec2i v; //POD
	// rect S;  //non-POD
	// v = {0,0};
	// S = {NULL, v,v,v}; //error: no match for 'operator='
	// S = (rect){NULL, v,v,v}; //error: no matching function for call to 'rect::rect((brace-enclosed initializer list))'
	openGUI5();
	
	
	GUI = new GUI2base;
	GUI->setPos(0,0);
	GUI->setSize(1024,1024);
	GUI->recalculateClientRect();
	GUI->visible=false;
	GUI->subscribeToMessageChannel(&input.channel, "");
	
	Binder.subscribeToMessageChannel(&input.channel, "");
	Binder2.subscribeToMessageChannel(&input.channel, "");
	//input.channel.subscribe("", GUI);
	//GUI3start();
	
		//OpenMenu1();
	//initConCommands();
	//OpenMenuConsole();
	//OpenVals();
	//OpenMenuToolbox();
	//myFrame.parent
    //MessageBox(0, "FreeType: done generating textures","info", MB_OK);
	OpenNewConsole(GUI);
		//OpenValScreen(GUI);
	//OpenGUI4();
	startModellerSession();
	startGUIeditorSession();
	KeyBinds["b"] = "echo butts";
	KeyBinds["w"] = "+camforward";
	KeyBinds["s"] = "+cambackward";
	KeyBinds["a"] = "+camleft";
	KeyBinds["d"] = "+camright";
	KeyBinds["r"] = "camreset";
	KeyBinds["space"] = "+camup";
	KeyBinds["ctrl"] = "+camdown";
	KeyBinds["shift"] = "+camslow";
	//confuncs["forwardonce"] = camForward;
	//confuncs["backwardonce"] = camBack;
	//KeyBinds["esc"] = "camera_mouse_capture 0";
	KeyBinds["escape"] = "toggle_mouse";
	confuncs["toggle_mouse"] = ToggleMouseCapture;
	bindtests();
	camInit();
	scene.push_back(new point({0,0,0}));
	
		// glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
        // glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
        // glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);
	{
		vec A(0.0f, 1.0f, 0.0f);
		vec B(0.87f, -0.5f, 0.0f);
		vec C(-0.87f, -0.5f, 0.0f);
		rtriangle *T = new rtriangle(A,B,C);
		T->A.color = {255,0,0,255};
		T->B.color = {0,255,0,255};
		T->C.color = {0,0,255,255};
		scene.push_back(T);
	}
	
	scene.push_back(new floatingtext({0,1,0},"R"));
	scene.push_back(new floatingtext({0.87,-0.5,0},"G"));
	scene.push_back(new floatingtext({-0.87,-0.5,0},"B"));

	debugcurrent.pop();
}

void OnResize(){
	if(GUI5){GUI5->invalidate();}
}
void OnFocusLost(){
	convars["camera_mouse_capture"] = 0;
}
void RenderGUI()
{
	//mouseP = mousePos;
	static int twidth = 0;
	glColor3f(0.2f,0.7f,0.7f);
	paintRect(32,30,32+twidth,52);
	glColor3f(1.0f,1.0f,1.0f);
	string version("Version ");
	string vnumber = "102";
	string vdate = " of 24th December 2015"; //" of 15th September 2015";
	twidth = printw(32,32,-1,-1,version+vnumber+vdate);
	
	rect screen = getScreenRect();
	vec2i pack[3]= {input.getMousePos(), screen.getStart(), screen.getEnd()};
    vec4i windowrect = {screen.getx(),screen.gety(),screen.getx2(),screen.gety2()};
	
	GUI2base::propagateMouseOver(GUI,(void*)(pack), 0);
	GUI2base::propagateRender(GUI,(void*)(&windowrect),0);
	
	if(GUI4){GUI4->think(); GUI4->render();}
	if(GUI5){GUI5->renderlogic();}
}





/* void ProcessMouseclick(int mb)
{
} */

void InputTick()
{
	for(int I = 0;I<256;I++)
	{
		if(Binds[I].keyDown&&(Binds[I].mode==4)&&Binds[I].onPress)
		{
			Binds[I].onPress(NULL);
		}
	}
	if(convars["camera_mouse_capture"])//if(mouseCapture)
	{
		camera.mouseAim();
	}
}
void keyThing(SDL_Event event){
	input.keyThing(event);
}
/* void ProcessKeyboard(int kb)
{
} */

void Render2D()
{
	RenderGUI();
}

void Render3D()
{
	//NO ONE FUCKING TOUCHES THOSE MATRICES
	int Errc = 0;
					//Errc = glGetError();if(Errc){printf("<1=%s>\n",gluErrorString(Errc));}
    //glPushMatrix();
	
	glRotatef(-90,1,0,0);
	double w = camera.angle.getRotationAngle();
	vec v = camera.angle.getRotationAxis();
	glBegin(GL_LINES); //cordinate helper
		vec a = {0,2,-1};//SomeVec1+CamAngle.rotateVector({0,0,-2});
		quat qc = camera.angle; qc.w = -qc.w;
		vec b = a+qc.rotateVector({0.5,0,0});
		glColor3f(1.0f,0.0f,0.0f); glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z);
		
		b = a+qc.rotateVector({0,0.5,0});
		glColor3f(0.0f,1.0f,0.0f); glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z);
		
		b = a+qc.rotateVector({0,0,0.5});
		glColor3f(0.0f,0.0f,1.0f); glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z);	
	glEnd();
	int sw = -1;
	int sv = 1;
	int sc = -1;
	if(camRotOn){glRotatef(sw*w,sv*v.x,sv*v.y,sv*v.z);}
    glTranslated(sc*camera.pos.x, sc*camera.pos.y, sc*camera.pos.z);
	for(std::vector<renderable*>::iterator I = scene.begin(); I != scene.end(); I++){
		if((*I)->lifetime == 0){
			scene.erase(I);
			I--;
		}else{
			(*I)->render();
			(*I)->lifetime--;
		}
	}
	
    //glPopMatrix();
}


void RenderTick()
{
    glClearColor(bground.r/255.0f,bground.g/255.0f,bground.b/255.0f,bground.a/255.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	hook.run("onRender");
    Render3D();

    camera.go2D();
    Render2D();
    camera.go3D();

    //SwapBuffers(hDC);
	SDL_GL_SwapWindow(window);
	
    theta += 1.0f;
    Sleep(1);
}

int ticks = 0;
void ThinkTick()
{
	ticks++;
	//parse("test4");
	hook.run("onTick");
	printvals("ticks", "there have been "+itoa(ticks)+" ticks since the start.");
	camera.tick();
	if(GUI5){GUI5->think();}
/*
	for(int i = 0; i<AllPhysBodies.size(); i++)
	{
		physBody *B = AllPhysBodies[i];
		if(B->onThink){B->onThink((void*)B);}
	}*/
}


//
void ProgramTick(){
	InputTick();
	ThinkTick();
	RenderTick();
	scene.push_back(new point(camera.pos, 200));
}

void CallDestructor()
{
	//gotta call GUI destructor too
    fontFree((glyphkind*)Calibri8);
	fontFree((glyphkind*)Calibri12);
	fontFree((glyphkind*)Calibri18);
	fontFree((glyphkind*)Calibri20);
	fontFree((glyphkind*)Calibri22);
	fontFree((glyphkind*)Tahoma8);
	fontFree((glyphkind*)Tahoma12);
	fontFree((glyphkind*)Tahoma18);
	fontFree((glyphkind*)Tahoma20);
	fontFree((glyphkind*)Tahoma22);
	fontFree((glyphkind*)CourierNew8);
	fontFree((glyphkind*)CourierNew12);
	fontFree((glyphkind*)CourierNew18);
	fontFree((glyphkind*)CourierNew20);
	fontFree((glyphkind*)CourierNew22);
}

void Cleanup(){
	SDL_DestroyWindow(window);
	SDL_Quit();
}
