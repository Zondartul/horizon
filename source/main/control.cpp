#include "main/control.h"
#include <windows.h>
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
vec SomeVec1; //cam pos.
vec SomeVec2;
vec2i deltaMouse;
vec2i windowCorner;
vec2i windowSize;
vec2i windowCenter;
quat CamAngle;
quat CamAngTest;
double Camnorm;
double camSpeed;
bool renderWireframe;
//int EntLookAt;
bool mouseCapture;
bool camRotOn;


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
}
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
	GUI5->setPos({200,200}).setSize({500,500}).setDebug(true);
	GUI5->subscribeToMessageChannel(&input.channel,"");
	GUI5tabgroup *tab = new GUI5tabgroup();
	tab->setTitle(0,"text").addElement((*(new GUI5label)).setText("hello").setPos({50,50}).setDebug(true));
	tab->setTitle(1,"btn").addElement((*(new GUI5button)).setText("[PRESS ME]").setPos({50,100}).setDebug(true));
	tab->addElement((*(new GUI5image)).setImage("../resource/textures/other/error.png").setPos({50,200}).setDebug(true)); //todo: make a resource path resolver
	tab->addElement((*(new GUI5window)).setTitle("Window!").setPos({200,25}).setSize({200,200}).setDebug(false));
	tab->setDebug(true);
	GUI5->addElement(*tab);
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
			case 5://whenever windows tells us
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
			case 5://whenever windows tells us
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
			ParseKey(msg->name[0]);
		}
		if(msg->type == "key_up"){
			ParseKey(-msg->name[0]);
		}
	}
} Binder;

void camRotYCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(15,{0,1,0});}
void camRotYCCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(-15,{0,1,0});}
void camRotXCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(15,{1,0,0});}
void camRotXCCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(-15,{1,0,0});}
void camRotZCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(15,{0,0,1});}
void camRotZCCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(-15,{0,0,1});}
/*
void camRotYCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(15,{0,1,0});}
void camRotYCCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(-15,{0,1,0});}
void camRotXCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(15,{0,1,0});}
void camRotXCCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(-15,{0,1,0});}
void camRotZCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(15,{0,1,0});}
void camRotZCCW(void* arg){CamAngle = CamAngle*quat::fromAngleAxis(-15,{0,1,0});}
*/

// x = right
// y = forward
// z = up

void camForward(void* arg){SomeVec1 = SomeVec1+CamAngle.rotateVector(((vec){0,.1,0})*convars["camSpeed"]);}
void camBack(void* arg){SomeVec1 = SomeVec1+CamAngle.rotateVector(((vec){0,-.1,0})*convars["camSpeed"]);}
void camLeft(void* arg){SomeVec1 = SomeVec1+CamAngle.rotateVector(((vec){-.1,0,0})*convars["camSpeed"]);}
void camRight(void* arg){SomeVec1 = SomeVec1+CamAngle.rotateVector(((vec){.1,0,0})*convars["camSpeed"]);}
void camSlow(void* arg){camSpeed = 0.2;}
void camFast(void* arg){camSpeed = 1;}
void camUp(void* arg){SomeVec1 = SomeVec1+((vec){0,0,.1})*camSpeed;}
void camDown(void* arg){SomeVec1 = SomeVec1+((vec){0,0,-.1})*camSpeed;}
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
	debug("freetype version = %d.%d.%d",FF_version_major, FF_version_minor, FF_version_patch);
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
	SomeVec1 = {0,0,0};
	SomeVec2 = {2,5,5};
	//bindKey('t',&Test1,NULL,1);
	//bindKey('w',&camForward,NULL,4);
	//bindKey('a',&camLeft,NULL,4);
	//bindKey('s',&camBack,NULL,4);
	//bindKey('d',&camRight,NULL,4);
	//bindKey(32,&camUp,NULL,4);
	//bindKey('c',&camDown,NULL,4);
	//bindKey(27,&ToggleMouseCapture,NULL,1);
	//bindKey(16,&camSlow,&camFast,1);
	
	//numpad
	// 103 104 105
	// 100 101 102
	//  97  98  99
	//bindKey(100,&camRotYCCW,NULL,1);
	//bindKey(102,&camRotYCW,NULL,1);
	//bindKey(104,&camRotXCCW,NULL,1);
	//bindKey(98,&camRotXCW,NULL,1);
	//bindKey(103,&camRotZCCW,NULL,1);
	//bindKey(105,&camRotZCW,NULL,1);
	//bindKey(97, &ToggleCamRot,NULL,1);
	mouseCapture = false;
	camRotOn = true;
	camSpeed = 1;
	CamAngle = quat::fromAngleAxis(0,{0,0,1});//{0,{0,0,1}};
	CamAngTest = {1,{0,0,1}};
	//EntLookAt = 0;
	renderWireframe = false;
	bground.r = 142;
	bground.g = 187;
	bground.b = 255;
	bground.a = 255;

	
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
	confuncs["forwardonce"] = camForward;
	confuncs["backwardonce"] = camBack;
	//KeyBinds["esc"] = "camera_mouse_capture 0";
	KeyBinds["esc"] = "toggle_mouse";
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



void RenderGUI()
{
	mouseP = mousePos;
	static int twidth = 0;
	glColor3f(0.2f,0.7f,0.7f);
	paintRect(32,30,32+twidth,52);
	glColor3f(1.0f,1.0f,1.0f);
	string version("Version ");
	string vnumber = "101";
	string vdate = " of 15th September 2015";	//" of 8th August 2015"; 
	twidth = printw(32,32,-1,-1,version+vnumber+vdate);
	
	vec2i pack[3]= {mousePos, (vec2i){0,0}, (vec2i){(int)width, (int)height}};
    vec4i windowrect = {0,0,(int)width,(int)height};
	
	GUI2base::propagateMouseOver(GUI,(void*)(pack), 0);
	GUI2base::propagateRender(GUI,(void*)(&windowrect),0);
	
	if(GUI4){GUI4->think(); GUI4->render();}
	if(GUI5){GUI5->renderlogic();}
	//GUI3rendertick();
	
	//printw(256,256,-1,-1,"crect.x = %d, y = %d, \nbtn.pos.x = %d, y = %d", myFrame->crect.x1,myFrame->crect.y1,myFrame->CloseButton->pos.x, myFrame->CloseButton->pos.y);
	
	//GUIM.render(NULL);
	//GUIM.checkMouseOver(NULL, mousePos.x, mousePos.y);
}





void ProcessMouseclick(int mb)
{
   /*
   if(!GUI2base::propagateClick(GUI,(void*)(&mb),0))
   {
		printf("mb = %d\n",mb);
		if(mb==1){
			printf("click void\n");
			ParseKey(1);
			//convars["camera_mouse_capture"] = 1;
			GUI->unsubscribeFromMessageChannel(&input.channel, "");
			//input.channel.unsubscribe("", GUI);
			//input.channel.subscribe("", &Binder);
			//input.channel.subscribe("", &Binder2);
			Binder.subscribeToMessageChannel(&input.channel, "");
			Binder2.subscribeToMessageChannel(&input.channel, "");
		}//Binds[1].keyDown = true;}
		if(mb==2){ParseKey(2);}//Binds[2].keyDown = true;}
		if(mb==0){ParseKey(-1);
		printf("unclick void\n");}//Binds[1].keyDown = false;}
		if(mb==-1){ParseKey(-2);}//Binds[2].keyDown= false;}
   }else{
		if(mb == 1){
			//input.channel.unsubscribe("", &Binder);
			//input.channel.unsubscribe("", &Binder2);
			//input.channel.subscribe("", GUI);
			
			Binder.unsubscribeFromMessageChannel(&input.channel, "");
			Binder2.unsubscribeFromMessageChannel(&input.channel, "");
			GUI->subscribeToMessageChannel(&input.channel, "");
			printf("click window\n");
			convars["camera_mouse_capture"] = 0;
		}else
			printf("unclick window\n");
	}
	*/
   // GUIM.click(NULL, mb);
}

void InputTick()
{
	for(int I = 0;I<256;I++)
	{
		if(Binds[I].keyDown&&(Binds[I].mode==4)&&Binds[I].onPress)
		{
			Binds[I].onPress(NULL);
		}
	}
	//also lower 16-ish keys are non-mapped, lets use em for mouse
	//1 - mouse1
	//2 - mouse2
	//3 - mouse3
	//4 - mouse wheel up
	//5 - mouse wheel down
	//6 - mouseY updated
	//7 - mouseX updated
	//now do mouse!
	if(convars["camera_mouse_capture"])//if(mouseCapture)
	{
		//CamAngle = camera.angle;
		//get delta from center and then re-center!
		camera.mouseAim();
		
		//CamAngle = CamAngle.norm();
		//CamAngle = CamAngle.addRotation(deltaMouse.x/width,{0,0,1}).addRotation(deltaMouse.y/height,{1,0,0});
		//if(CamAngle.w>360){CamAngle.w=0;}
		//if(CamAngle.w<0){CamAngle.w=360;}
		SetCursorPos(windowCenter.x,windowCenter.y);
	}
}

void keyThing(UINT umsg, WPARAM wParam, LPARAM lParam)
{
	input.keyThing(umsg, wParam, lParam);
}

void ProcessKeyboard(int kb)
{
	/* it's been ages since this was even a thing
	return;
	int letter = 0;
	if(kb>0)
	{
		letter = 'a';
		MapVirtualKey((unsigned int)kb,2);
		if(!(GetKeyState(VK_SHIFT)&(0x8000))){letter = tolower(letter);}
		else
		{
		switch(letter)
		{
			case ',':
				letter = '<';
				break;
			case '.':
				letter = '>';
				break;
			case '/':
				letter = '?';
				break;
			case ';':
				letter = ':';
				break;
			case '\'':
				letter = '"';
				break;
			case '1':
				letter = '!';
				break;
			case '2':
				letter = '@';
				break;
			case '3':
				letter = '#';
				break;
			case '4':
				letter = '$';
				break;
			case '5':
				letter = '%';
				break;
			case '6':
				letter = '^';
				break;
			case '7':
				letter = '&';
				break;
			case '8':
				letter = '*';
				break;
			case '9':
				letter = '(';
				break;
			case '0':
				letter = ')';
				break;
			case '-':
				letter = '_';
				break;
			case '=':
				letter = '+';
				break;
			case '[':
				letter = '{';
				break;
			case ']':
				letter = '}';
				break;
			case '|':
				letter = '\\';
				break;
		}
		}
	}
	bool b = false;
	//bool b = GUIbase::propagateKeyboard(letter*kb/abs(kb));
	if(!b&&!ParseKey(kb))
	{
		//string mystring = "";
		char str[64];
		 //std::ostringstream stringStream;
		//stringStream << "Unbound key: " << kb << " = [" <<(char)kb <<"]/[" <<(char)letter << "]";
		
		sprintf(str,"Unbound key: %d ",kb);//= [%c]/[%c]",kb,(char)kb,(char)letter);
		printf("sending string to parse: \"%s\"\n",str);
		//ConsoleParse((string)str);
	}
	//GUIM.keyboard(kb); */
}

void Render2D()
{
    /*
	glColor3f(0.0f,1.0f,0.0f);
    paintRectOutline(1,1,128,128);
    glColor3f(0.9f, 0.5f, 0.0f);
    setFont(Tahoma16);

    int len = printw(32,256, "I like ponies, theta = %d c:", (int)theta);
    printw(32,128,"%c = %d", (char)((int)(theta/10)%128),((int)(theta/10)%128));
    glColor3f(1.0f,1.0f,1.0f);
    setFont(Calibri20);
    printw(32,32, "length of that line is %d pixels", len);

    printw(32,64, "MouseX: %d, MouseY: %d", mouseX, mouseY);
    */
	RenderGUI();
}
//vector<physBody*> TranslucentBodies;
//bool BodyDistanceComparator(physBody* i, physBody* j){return (i->pos-SomeVec1).length()>(j->pos-SomeVec1).length();}
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
    // glBegin(GL_TRIANGLES);

        // glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
        // glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
        // glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

    // glEnd();
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

/*
void BuildPerspProjMat(float *m, float fov, float aspect,
 float znear, float zfar)
 {
  float ymax = znear * tan(fov * PI_OVER_360);
  float ymin = -ymax;
  float xmax = ymax * aspect;
  float xmin = ymin * aspect;

  float width = xymax - xmin;
  float height = xymax - ymin;

  float depth = zfar - znear;
  float q = -(zfar + znear) / depth;
  float qn = -2 * (zfar * znear) / depth;

  float w = 2 * znear / width;
  w = w / aspect;
  float h = 2 * znear / height;

  m[0]  = w;
  m[1]  = 0;
  m[2]  = 0;
  m[3]  = 0;

  m[4]  = 0;
  m[5]  = h;
  m[6]  = 0;
  m[7]  = 0;

  m[8]  = 0;
  m[9]  = 0;
  m[10] = q;
  m[11] = -1;

  m[12] = 0;
  m[13] = 0;
  m[14] = qn;
  m[15] = 0;
 }
 */
void RenderTick(HDC hDC)
{
    glClearColor(bground.r/255.0f,bground.g/255.0f,bground.b/255.0f,bground.a/255.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	hook.run("onRender");
    Render3D();

    camera.go2D();
    Render2D();
    camera.go3D();

    SwapBuffers(hDC);

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

void ProgramTick(HWND hwnd, HDC hDC)
{
	input.hwnd = hwnd;
    RECT rect;
	POINT cursorPos;
	GetWindowRect(hwnd, &rect);
    GetCursorPos(&cursorPos);
    windowCorner.x = rect.left+7;
	windowCorner.y = rect.top+29;
	
	mousePos.x = cursorPos.x-rect.left-7;
    mousePos.y = cursorPos.y-rect.top-29;

	
    GetClientRect(hwnd, &rect);
	width = rect.right;
	height = rect.bottom;
	windowSize.x = width;
	windowSize.y = height;
	screen.setStart({0,0});
	screen.setSize({(int)width,(int)height});
	windowCenter.x = windowCorner.x+width/2;
	windowCenter.y = windowCorner.y+height/2;
	InputTick();
	ThinkTick();
    RenderTick(hDC);
	//vec coords = camera.worldtoscreen({0,0,0});
	scene.push_back(new point(camera.pos, 200));
	//PhysicsTick();
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
