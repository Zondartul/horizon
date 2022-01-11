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
#include "globals.h"
#include "messanger.h"
#include "input.h"
inputKind input;
PSchannel GUI_PS;
#include "textureloader.h"
#include "fonts.h"
//#include "Gui.h"
#include "paint.h"
#include "Gui2.h"
#include "Gui3.h"
#include "vectors.h"
#include "models.h"
#include "quaternions.h"
#include "newconsole.h"
#include "keybinds.h"
#include "camera.h"
#include "renderable.h"
//global vars go here
//LETS DO QUATERNIONS LIKE A BOSS

//GUIManager GUIM;

GUIbase *GUI;
GUIframe *myFrame;
model *myModel;
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
	message newMsg;
	newMsg.type = "stuff";
	newMsg.push<string>("apples"); // almost never do this, use .str instead
	newMsg.push(arg);
	GUI_PS.publish(newMsg);
}

void OpenMenu1()
{	
	//myModel = NULL;
	myFrame = new GUIframe;
	myFrame->setPos(0,0);
	myFrame->setSize(256,512);
	myFrame->title = "Frame 1";
	myFrame->setParent(GUI);
	
	GUIframe* myFrame2 = new GUIframe;
	myFrame2->setPos(0,0);
	myFrame2->setSize(96,96);
	myFrame2->title = "Frame 2";
	myFrame2->setParent((GUIbase*)myFrame);
	
	
	GUIbutton* Button = new GUIbutton;
	Button->pos.x = 200;
	Button->pos.y = 64;
	Button->size.x = 64;
	Button->size.y = 64;
	Button->color_panel = {0,64,255};
	Button->setParent((GUIbase*)myFrame2);
	
	GUIlabel* Label = new GUIlabel;
	Label->pos = {4,32};
	Label->size = {256-8,128-34};
	Label->setParent((GUIbase*)myFrame2);
   
	GUItextEntry* Text = new GUItextEntry;
	Text->pos = {4, 128};
	Text->setParent((GUIbase*)myFrame);
	
	GUIcheckbox* check = new GUIcheckbox;
	check->pos = {4,196};
	check->setParent((GUIbase*)myFrame);
	
	GUIspinner* spin = new GUIspinner;
	spin->pos = {4, 154};
	//spin->vals = {-10,0,10,3};
	spin->vals[0]=-10;spin->vals[1]=0;spin->vals[2]=10;spin->vals[3]=3;
	spin->setParent((GUIbase*)myFrame);
	
	GUIradiobutton* radio1 = new GUIradiobutton;
	radio1->pos = {4,256};
	radio1->setParent((GUIbase*)myFrame);
	
	GUIradiobutton* radio2 = new GUIradiobutton;
	radio2->pos = {32,256};
	radio2->setParent((GUIbase*)myFrame);
	
	GUIradiogroup* group = new GUIradiogroup;
	group->addButton((void*)radio1);
	group->addButton((void*)radio2);
	
	GUIlistbox* list = new GUIlistbox;
	list->addOption("pony",NULL,NULL);
	list->addOption("cupcake",NULL,NULL);
	list->addOption("shipping",NULL,NULL);
	list->setPos(4,290);
	list->setParent((GUIbase*)myFrame);
	
	GUIdropdownlist* ddlist = new GUIdropdownlist;
	ddlist->addOption("Pinkie", NULL,NULL);
	ddlist->addOption("Rainbow", NULL,NULL);
	ddlist->addOption("Applejack", NULL,NULL);
	ddlist->setPos(96,290);
	ddlist->setParent((GUIbase*)myFrame);
	
	GUIslider* slide = new GUIslider;
	slide->setPos(4,400);
	slide->setParent((GUIbase*)myFrame);
	
	GUIcolorbox* cbox = new GUIcolorbox;
	cbox->setPos(4,430);
	cbox->setParent((GUIbase*)myFrame);
	
	GUIbutton* btn1 = new GUIbutton;
	btn1->setPos(16,32);
	btn1->setSize(64,64);
	btn1->func = &sendMsg1;
	btn1->arg = (void *)btn1;
	btn1->setParent(myFrame);

	GUIbutton* btn2 = new GUIbutton;
	btn2->setPos(16+64,32);
	btn2->setSize(64,64);
	btn2->func = &sendMsg1;
	btn2->arg = (void *)btn2;
	btn2->setParent(myFrame);
}

void OpenVals()
{
	GUIframe* valframe = new GUIframe;
	valframe->setSize(256,256);
	valframe->setPos(768,32);
	valframe->setParent(GUI);
	
	GUIvaluedisplay* valx = new GUIvaluedisplay;	GUIvaluedisplay* val2x = new GUIvaluedisplay;
	valx->setPos(0,32);								val2x->setPos(128,32);
	valx->val = (void*)(&CamAngle.w);				val2x->val = (void*)(&CamAngTest.w);
	valx->mode = 'f';								val2x->mode = 'f';
	valx->setParent(valframe);						val2x->setParent(valframe);
	
	GUIvaluedisplay* valy = new GUIvaluedisplay;	GUIvaluedisplay* val2y = new GUIvaluedisplay;
	valy->setPos(0,64);								val2y->setPos(128,64);
	valy->val = (void*)(&CamAngle.v.x);				val2y->val = (void*)(&CamAngTest.v.x);
	valy->mode = 'f';								val2y->mode = 'f';
	valy->setParent(valframe);						val2y->setParent(valframe);
	
	GUIvaluedisplay* valz = new GUIvaluedisplay;	GUIvaluedisplay* val2z = new GUIvaluedisplay;
	valz->setPos(0,96);								val2z->setPos(128,96);
	valz->val = (void*)(&CamAngle.v.y);				val2z->val = (void*)(&CamAngTest.v.y);
	valz->mode = 'f';								val2z->mode = 'f';
	valz->setParent(valframe);						val2z->setParent(valframe);
	
	GUIvaluedisplay* valmx = new GUIvaluedisplay;	GUIvaluedisplay* val2my = new GUIvaluedisplay;
	valmx->setPos(0,128);							val2my->setPos(128,128);
	valmx->val = (void*)(&CamAngle.v.z);				val2my->val = (void*)(&CamAngTest.v.z);
	valmx->mode = 'f';								val2my->mode = 'f';
	valmx->setParent(valframe);						val2my->setParent(valframe);
	
	GUIvaluedisplay* valdmx = new GUIvaluedisplay;	GUIvaluedisplay* val2dmy = new GUIvaluedisplay;
	valdmx->setPos(0,128+32);							val2dmy->setPos(128,128+32);
	valdmx->val = (void*)(&deltaMouse.x);				val2dmy->val = (void*)(&Camnorm);
	valdmx->mode = 'd';								val2dmy->mode = 'f';
	valdmx->setParent(valframe);					val2dmy->setParent(valframe);
}

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
void ToggleMouseCapture(void* arg){mouseCapture = !mouseCapture; SetCursorPos(windowCenter.x,windowCenter.y);}
void ToggleCamRot(void* arg){camRotOn = !camRotOn;}
class BinderKind;

vector<renderable*> scene;
vec old = {0,0,0};
void testfunc(void* arg){
	frustum frust = camera.getFrustum();
	printf("point A: [%f, %f, %f], H: [%f, %f, %f]\n", frust.A.x, frust.A.y, frust.A.z, frust.H.x, frust.H.y, frust.H.z);
	scene.push_back(new point(frust.A));
	scene.push_back(new point(frust.B));
	scene.push_back(new point(frust.C));
	scene.push_back(new point(frust.D));
	scene.push_back(new point(frust.E));
	scene.push_back(new point(frust.F));
	scene.push_back(new point(frust.G));
	scene.push_back(new point(frust.H));
	
	scene.push_back(new line(frust.A, frust.B));
	scene.push_back(new line(frust.B, frust.C));
	scene.push_back(new line(frust.C, frust.D));
	scene.push_back(new line(frust.D, frust.A));
	scene.push_back(new line(frust.E, frust.F));
	scene.push_back(new line(frust.F, frust.G));
	scene.push_back(new line(frust.G, frust.H));
	scene.push_back(new line(frust.H, frust.E));
	scene.push_back(new line(frust.A, frust.E));
	scene.push_back(new line(frust.B, frust.F));
	scene.push_back(new line(frust.C, frust.G));
	scene.push_back(new line(frust.D, frust.H));
	scene.push_back(new line(frust.E, frust.G));
	scene.push_back(new line(frust.F, frust.H));
	printf("point E distance: %f\n", (frust.H-camera.pos).length());
	
}

void OnProgramStart()
{
    theta = 1.0f;
    //if(MessageBox(0,"V: 44. Continue?", "Version", MB_OKCANCEL|MB_ICONQUESTION )==2){PostQuitMessage(0); return;}
    Tahoma8 = (void*)GenerateFont("C:/Stride/tahoma.ttf",8,true);//no aa
	Tahoma12= (void*)GenerateFont("C:/Stride/tahoma.ttf",12,false);//no aa
	Tahoma18= (void*)GenerateFont("C:/Stride/tahoma.ttf",18,true);
	Tahoma20= (void*)GenerateFont("C:/Stride/tahoma.ttf",20,true);
	Tahoma22= (void*)GenerateFont("C:/Stride/tahoma.ttf",22,true);
	Calibri8= (void*)GenerateFont("C:/Stride/calibri.ttf",8,true);
	Calibri12= (void*)GenerateFont("C:/Stride/calibri.ttf",12,true);
	Calibri18= (void*)GenerateFont("C:/Stride/calibri.ttf",18,true);
	Calibri20= (void*)GenerateFont("C:/Stride/calibri.ttf",20,true);
	Calibri22= (void*)GenerateFont("C:/Stride/calibri.ttf",22,true);
	CourierNew8= (void*)GenerateFont("C:/Stride/cour.ttf",8,true);
	CourierNew12= (void*)GenerateFont("C:/Stride/cour.ttf",12,true);
	CourierNew18= (void*)GenerateFont("C:/Stride/cour.ttf",18,true);
	CourierNew20= (void*)GenerateFont("C:/Stride/cour.ttf",20,true);
	CourierNew22= (void*)GenerateFont("C:/Stride/cour.ttf",22,true);
	
	setFont(Calibri18);
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

	
	
	GUI = new GUIbase;
	GUI->setPos(0,0);
	GUI->setSize(1024,1024);
	GUI->recalculateClientRect();
	GUI->visible=false;
	input.channel.subscribe("", GUI);
	//GUI3start();
	
	//OpenMenu1();
	//initConCommands();
	//OpenMenuConsole();
	//OpenVals();
	//OpenMenuToolbox();
	//myFrame.parent
    //MessageBox(0, "FreeType: done generating textures","info", MB_OK);
	OpenNewConsole(GUI);
	
	KeyBinds["b"] = "echo butts";
	KeyBinds["w"] = "+camforward";
	KeyBinds["s"] = "+cambackward";
	KeyBinds["a"] = "+camleft";
	KeyBinds["d"] = "+camright";
	KeyBinds["r"] = "camreset";
	KeyBinds["space"] = "+camup";
	KeyBinds["ctrl"] = "+camdown";
	confuncs["forwardonce"] = camForward;
	confuncs["backwardonce"] = camBack;
	KeyBinds["esc"] = "camera_mouse_capture 0";
	KeyBinds["t"] = "test";
	confuncs["test"] = testfunc;
	
	scene.push_back(new point({0,0,0}));
}



void RenderGUI()
{
	mouseP = mousePos;
	static int twidth = 0;
	glColor3f(0.2f,0.7f,0.7f);
	paintRect(32,30,32+twidth,52);
	glColor3f(1.0f,1.0f,1.0f);
	string version("Version ");
	string vnumber = "93";
	twidth = printw(32,32,-1,-1,version+vnumber);
	
	vec2i pack[3]= {mousePos, (vec2i){0,0}, (vec2i){(int)width, (int)height}};
    vec4i windowrect = {0,0,(int)width,(int)height};
	
	GUIbase::propagateMouseOver(GUI,(void*)(pack), 0);
	GUIbase::propagateRender(GUI,(void*)(&windowrect),0);
	
	//GUI3rendertick();
	
	//printw(256,256,-1,-1,"crect.x = %d, y = %d, \nbtn.pos.x = %d, y = %d", myFrame->crect.x1,myFrame->crect.y1,myFrame->CloseButton->pos.x, myFrame->CloseButton->pos.y);
	
	//GUIM.render(NULL);
	//GUIM.checkMouseOver(NULL, mousePos.x, mousePos.y);
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

class BinderKind: public PSsubscriber{
	void PSreceive(message msg){
		if(msg.type == "key_down"){
			ParseKey(msg.str[0]);
		}
		if(msg.type == "key_up"){
			ParseKey(-msg.str[0]);
		}
	}
} Binder;

void ProcessMouseclick(int mb)
{
   if(!GUIbase::propagateClick(GUI,(void*)(&mb),0))
   {
		printf("mb = %d\n",mb);
		if(mb==1){
			printf("click void\n");
			ParseKey(1);
			convars["camera_mouse_capture"] = 1;
			input.channel.unsubscribe("", GUI);
			input.channel.subscribe("", &Binder);
			input.channel.subscribe("", &Binder2);
		}//Binds[1].keyDown = true;}
		if(mb==2){ParseKey(2);}//Binds[2].keyDown = true;}
		if(mb==0){ParseKey(-1);
		printf("unclick void\n");}//Binds[1].keyDown = false;}
		if(mb==-1){ParseKey(-2);}//Binds[2].keyDown= false;}
   }else{
		if(mb == 1){
			input.channel.unsubscribe("", &Binder);
			input.channel.unsubscribe("", &Binder2);
			input.channel.subscribe("", GUI);
			printf("click window\n");
		}else
			printf("unclick window\n");
	}
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
	//GUIM.keyboard(kb);
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
	int Errc = 0;
					//Errc = glGetError();if(Errc){printf("<1=%s>\n",gluErrorString(Errc));}
    glPushMatrix();
	
	glRotatef(-90,1,0,0);
	
	//glRotatef(-CamAngle.w, CamAngle.v.x, CamAngle.v.z, CamAngle.v.y);
	//double w = CamAngle.getRotationAngle();
	double w = camera.angle.getRotationAngle();
	//vec v = CamAngle.getRotationAxis();
	vec v = camera.angle.getRotationAxis();
	glBegin(GL_LINES); //cordinate helper
		vec a = {0,2,-1};//SomeVec1+CamAngle.rotateVector({0,0,-2});
		//quat qc = CamAngle; qc.w = -qc.w;
		quat qc = camera.angle; qc.w = -qc.w;
		vec b = a+qc.rotateVector({0.5,0,0});
		glColor3f(1.0f,0.0f,0.0f); glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z);
		
		b = a+qc.rotateVector({0,0.5,0});
		glColor3f(0.0f,1.0f,0.0f); glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z);
		
		b = a+qc.rotateVector({0,0,0.5});
		glColor3f(0.0f,0.0f,1.0f); glVertex3f(a.x,a.y,a.z); glVertex3f(b.x,b.y,b.z);	
	glEnd();
	//Errc = glGetError();if(Errc){printf("<2=%s>\n",gluErrorString(Errc));}
	if(camRotOn){glRotatef(-w,v.x,v.y,v.z);}
	//glTranslated(-SomeVec1.x,-SomeVec1.y,-SomeVec1.z);
    glTranslated(-camera.pos.x, -camera.pos.y, -camera.pos.z);
	//glRotatef(theta, 0.0f, 0.0f, 1.0f);
	//glScalef(0.1f,0.1f,0.1f);
    glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
        glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

    glEnd();
	for(std::vector<renderable*>::iterator I = scene.begin(); I != scene.end(); I++){
	//for(int I = 0; I < scene.size(); I++){
		if((*I)->lifetime == 0){
			scene.erase(I);
			I--;
		}else{
			(*I)->render();
			(*I)->lifetime--;
		}
	}
	//Errc = glGetError();if(Errc){printf("<3=%s>\n",gluErrorString(Errc));}
	/*
	for(int i = 0;i<AllPhysBodies.size();i++)
	{
	glPushMatrix();
	physBody *Body = AllPhysBodies[i];
	//while(!Body){i++; Body = AllPhysBodies[i];}
	glTranslatef(Body->pos.x,Body->pos.y,Body->pos.z);
	glRotatef(Body->orient.getAngle(),Body->orient.getX(),Body->orient.getY(),Body->orient.getZ());
	glScalef(Body->scale,Body->scale,Body->scale);
	myModel = Body->mdl;
	if(myModel!=NULL)
	{
		vec3i col = Body->color;
		glColor3ub(col.x,col.y,col.z);
			
		//apply orientation here
		if(renderWireframe)
		{
			glBegin(GL_LINES);
			
			for(int I=0;I<myModel->numtris;I++)
			{
				triangle T = myModel->mesh[I];
				
				glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);
				glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);
				glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);
			}
			glEnd();
		}
		else
		{
		if(myModel->numtextures)
		{
		int prevTex = 0;
		if(myModel->blendmode>1){TranslucentBodies.push_back(Body); glPopMatrix(); continue;}
		
		glEnable(GL_TEXTURE_2D);
		
		if(myModel->blendmode==1)
		{
			glAlphaFunc(GL_GREATER, 0.5);
			glEnable(GL_ALPHA_TEST);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
		}
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glBindTexture( GL_TEXTURE_2D, myModel->textures[0].t );
		for(int I=0;I<myModel->numtris;I++)
		{
			triangle T = myModel->mesh[I];
			textriangle tT = myModel->texmap[I];
			// render translucent stuff last, and in order.
			
			if(tT.texid!=prevTex)
			{
										//Errc = glGetError();if(Errc){printf("<7=%s>\n",gluErrorString(Errc));}
				//glEnd();//invalid operation here?
										//Errc = glGetError();if(Errc){printf("<7.5=%s>\n",gluErrorString(Errc));}
				glBindTexture( GL_TEXTURE_2D, myModel->textures[tT.texid].t );
										//Errc = glGetError();if(Errc){printf("<8=%s>\n",gluErrorString(Errc));}
				//glBegin(GL_TRIANGLES);
				//printf("|ts");
				prevTex = tT.texid;
										//Errc = glGetError();if(Errc){printf("<9=%s>\n",gluErrorString(Errc));}
				
			}
			glBegin(GL_TRIANGLES);
			glTexCoord2f(tT.v[0].x,tT.v[0].y); glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);
			glTexCoord2f(tT.v[1].x,tT.v[1].y); glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);
			glTexCoord2f(tT.v[2].x,tT.v[2].y); glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);
			glEnd();
		}
		
						//Errc = glGetError();if(Errc){printf("<9.5=%s>\n",gluErrorString(Errc));}
		glEnd();
						//Errc = glGetError();if(Errc){printf("<10=%s>\n",gluErrorString(Errc));}
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
						//Errc = glGetError();if(Errc){printf("<11=%s>\n",gluErrorString(Errc));}
		glDisable(GL_ALPHA_TEST);
		}
		else
		{
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glBegin(GL_TRIANGLES);
		glColor3ub(Body->color.x,Body->color.y,Body->color.z);
		for(int I=0;I<myModel->numtris;I++)
		{
			triangle T = myModel->mesh[I];
			/*
			glcolor3f(1.0f, 0.0f, 0.0f);   glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);
			glColor3f(0.0f, 1.0f, 0.0f);   glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);
			glColor3f(0.0f, 0.0f, 1.0f);   glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);
			*//*
			glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);
			glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);
			glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);
			
		}
		glEnd();
		glDisable(GL_CULL_FACE);
		glDisable(GL_ALPHA_TEST);
		}
		}
	}
	glPopMatrix();
	}*/
	//sort(TranslucentBodies.begin(), TranslucentBodies.end(), BodyDistanceComparator);
	//GL_DISABLE(GL_DEPTH_BUFFER_WRITE);
	//glDepthFunc(GL_ALWAYS);
	/*
	for(int i = 0;i<TranslucentBodies.size();i++)
	{
		glPushMatrix();
		physBody *Body = TranslucentBodies[i];
		//while(!Body){i++; Body = AllPhysBodies[i];}
		glTranslatef(Body->pos.x,Body->pos.y,Body->pos.z);
		glRotatef(Body->orient.getAngle(),Body->orient.getX(),Body->orient.getY(),Body->orient.getZ());
		glScalef(Body->scale,Body->scale,Body->scale);
		myModel = Body->mdl;
	
		if(myModel->numtextures)
		{
		int prevTex = 0;
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		/*
		if(myModel->blendmode==2)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			printf("Asking for shader\n");
			glPopMatrix(); continue; // don't have a shader yet
		}*//*
		vec3i col = Body->color;
		glColor3ub(col.x,col.y,col.z);
		
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glBindTexture( GL_TEXTURE_2D, myModel->textures[0].t );
		for(int I=0;I<myModel->numtris;I++)
		{
			triangle T = myModel->mesh[I];
			textriangle tT = myModel->texmap[I];
			// render translucent stuff last, and in order.
			
			if((tT.flags&R_DARKEN)&(tT.flags&R_LIGHTEN))
			{
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			}
			else
			{
				if(tT.flags&R_DARKEN)
				{
					glBlendFunc(GL_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA);
				}
				if(tT.flags&R_LIGHTEN)
				{
					glBlendFunc(GL_DST_COLOR,GL_ONE);
				}
				if(!((tT.flags&R_DARKEN)|(tT.flags&R_LIGHTEN)))
				{
					printf("no light nor darkness for triangle %d!\n", I);
					glDisable(GL_BLEND);
				}
			}
			
			//glBlendFunc(GL_DST_COLOR,GL_ONE);
			if(tT.texid!=prevTex)
			{
										//Errc = glGetError();if(Errc){printf("<7=%s>\n",gluErrorString(Errc));}
				//glEnd();//invalid operation here?
										//Errc = glGetError();if(Errc){printf("<7.5=%s>\n",gluErrorString(Errc));}
				glBindTexture( GL_TEXTURE_2D, myModel->textures[tT.texid].t );
										//Errc = glGetError();if(Errc){printf("<8=%s>\n",gluErrorString(Errc));}
				//glBegin(GL_TRIANGLES);
				//printf("|ts");
				prevTex = tT.texid;
										//Errc = glGetError();if(Errc){printf("<9=%s>\n",gluErrorString(Errc));}
				
			}
			glBegin(GL_TRIANGLES);
			glTexCoord2f(tT.v[0].x,tT.v[0].y); glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);
			glTexCoord2f(tT.v[1].x,tT.v[1].y); glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);
			glTexCoord2f(tT.v[2].x,tT.v[2].y); glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);
			glEnd();
		}
		
						//Errc = glGetError();if(Errc){printf("<9.5=%s>\n",gluErrorString(Errc));}
		glEnd();
						//Errc = glGetError();if(Errc){printf("<10=%s>\n",gluErrorString(Errc));}
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
						//Errc = glGetError();if(Errc){printf("<11=%s>\n",gluErrorString(Errc));}
		}
		else
		{
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glBegin(GL_TRIANGLES);
		glColor3ub(Body->color.x,Body->color.y,Body->color.z);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_CONSTANT_ALPHA,GL_CONSTANT_ALPHA);
		//glBlendColor(0,0,0,Body->alpha);
		//bluh, fuck alpha, I need to load extensions or something
		//glSet(GL_BLEND_COLOR)
		//glBlendColor(0,0,0,Body->alpha);
		for(int I=0;I<myModel->numtris;I++)
		{
			triangle T = myModel->mesh[I];
			/*
			glcolor3f(1.0f, 0.0f, 0.0f);   glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);
			glColor3f(0.0f, 1.0f, 0.0f);   glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);
			glColor3f(0.0f, 0.0f, 1.0f);   glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);
			*//*
			glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);
			glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);
			glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);
			
		}
		glEnd();
		glDisable(GL_CULL_FACE);
		glDisable(GL_ALPHA_TEST);
		}
	glPopMatrix();
	}*/
	//TranslucentBodies.clear();
	//glEnable(GL_DEPTH_BUFFER_WRITE);
	//glDepthFunc(GL_LESS);
		//Errc = glGetError();if(Errc){printf("<12=%s>\n",gluErrorString(Errc));}
    glPopMatrix();
}
void Render_go2D()
{
    glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
    //glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void Render_go3D()
{
    glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	glLoadIdentity();
	camera.setFrustum();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	
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
    Render3D();

    Render_go2D();
    Render2D();
    Render_go3D();

    SwapBuffers(hDC);

    theta += 1.0f;
    Sleep(1);
}

void ThinkTick()
{
	camera.tick();
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
