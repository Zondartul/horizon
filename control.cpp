#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <math.h>
#include <ctype.h>
//#include <iostream>
//#include <string>
//#include <sstream> 
//using namespace std;
//#include <vector>

#include "globals.h"
#include "fonts.h"
//#include "Gui.h"
#include "Gui2.h"
#include "vectors.h"
#include "models.h"
#include "quaternions.h"
//global vars go here
//LETS DO QUATERNIONS LIKE A BOSS

//GUIManager GUIM;
typedef void (*funcptr)(void *arg); // let "funcptr" be the "pointer to a void-returning funtion that takes
									                      // a pointer-to-void" type.

GUIbase *GUI;
#include "console.h"
GUIframe *myFrame;
model *myModel;
vector SomeVec1;
vector SomeVec2;
vec2i deltaMouse;
vec2i windowCorner;
vec2i windowSize;
vec2i windowCenter;
quat CamAngle;
bool mouseCapture;
int pie = 3.14;
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



void Test1(void* arg)
{
	ConsoleParse("I LIKE TURTLES");
}


void OpenMenuModel()
{
	GUIframe* ModelMenu = new GUIframe;
	ModelMenu->setPos(128,128);
	ModelMenu->setSize(196,128);
	ModelMenu->title = "Model info";
	ModelMenu->setParent(GUI);
	
	GUIlabel* Mtext = new GUIlabel;
	Mtext->setPos(4,32);
	char text[128];
	sprintf(text, "Model: %p\nVerticles: %d\nTriangles: %d\nTextures: %d", myModel, myModel->numtris*3,
																		myModel->numtris, myModel->numtextures);
	Mtext->text = text;
	Mtext->setParent((GUIbase*) ModelMenu);
}
void genCube(void *arg)
{
	double l = 	((GUIspinner*)(((void**)arg)[0]))->vals[1];
	double w = 	((GUIspinner*)(((void**)arg)[1]))->vals[1];
	double h = 	((GUIspinner*)(((void**)arg)[2]))->vals[1];
	vector V = {l,w,h};
	model* M = new model;
	((GUIspinner*)(((void**)arg)[0]))->vals[1] = V.length();
	
	M->mesh = new triangle[12];
	/*
	  F-----G
	 /|    /|
	E-+---H |
	| |   | |
	| B---+-C
	|/    |/
	A-----D
	
	AB = +y
	AD = +x
	AE = +z
	*/ 
	vector A = {0,0,0};
	vector B = {0,1,0};
	vector C = {1,1,0};
	vector D = {1,0,0};
	vector E = {0,0,1};
	vector F = {0,1,1};
	vector G = {1,1,1};
	vector H = {1,0,1};
	
	//CW culling.
	//front
	M->mesh[0] = (triangle){A,E,H};
	M->mesh[1] = (triangle){H,D,A};
	//right
	M->mesh[2] = (triangle){D,H,G};
	M->mesh[3] = (triangle){G,C,D};
	//back
	M->mesh[4] = (triangle){C,G,F};
	M->mesh[5] = (triangle){F,B,C};
	//left
	M->mesh[6] = (triangle){B,F,E};
	M->mesh[7] = (triangle){E,A,B};
	//up
	M->mesh[8] = (triangle){E,F,G};
	M->mesh[9] = (triangle){G,H,E};
	//down
	M->mesh[10] = (triangle){B,A,D};
	M->mesh[11] = (triangle){D,C,B};
	//
	M->numtris = 12;
	myModel = M;
	OpenMenuModel();
}
void OpenMenuGen()
{
	GUIframe* GenMenu = new GUIframe;
	GenMenu->setPos(128,128);
	GenMenu->setSize(256,128);
	GenMenu->title = "Generator";
	GenMenu->setParent(GUI);
	
	GUIspinner* spinx = new GUIspinner;		GUIlabel* spinxtext = new GUIlabel;
	spinx->setPos(4,32);					spinxtext->setPos(96,32);
	spinx->vals = {-10,0,10,0.5,2};			spinxtext->text = "length";
	spinx->setParent((GUIbase*) GenMenu);	spinxtext->setParent((GUIbase*) GenMenu);

	GUIspinner* spiny = new GUIspinner;		GUIlabel* spinytext = new GUIlabel;
	spiny->setPos(4,48);					spinytext->setPos(96,48);
	spiny->vals = {-10,0,10,0.5,2};			spinytext->text = "width";
	spiny->setParent((GUIbase*) GenMenu);	spinytext->setParent((GUIbase*) GenMenu);
	
	GUIspinner* spinz = new GUIspinner;		GUIlabel* spinztext = new GUIlabel;
	spinz->setPos(4,64);					spinztext->setPos(96,64);
	spinz->vals = {-10,0,10,0.5,2};			spinztext->text = "height";
	spinz->setParent((GUIbase*) GenMenu);	spinztext->setParent((GUIbase*) GenMenu);
	
	void **arg = new void*[3];
	arg[0] = (void*)spinx;
	arg[1] = (void*)spiny;
	arg[2] = (void*)spinz;
	GUIbutton* btnGen = new GUIbutton;
	btnGen->setPos(4,96);
	btnGen->setSize(96,16);
	btnGen->text = "Generate";
	btnGen->func = &genCube;
	btnGen->arg = (void*)arg;
	btnGen->setParent((GUIbase*) GenMenu);
}

void designerMenu()
{
	GUIframe* designerFrame = new GUIframe;
	designerFrame->setPos(64,64);
	designerFrame->setSize(128,512);
	designerFrame->title = "GUI designer";
	designerFrame->setParent(GUI);
	
	GUIImage* image1 = new GUIImage;
	image1->setPos(32,32);
	image1->setSize(32,32);
	image1->setImage("C:/Stride/grass3.raw");
	image1->setParent((GUIbase*)designerFrame);
}

void OpenMenu1()
{	
	myModel = NULL;
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
	spin->vals = {-10,0,10,3};
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
}

void OpenVals()
{
	GUIframe* valframe = new GUIframe;
	valframe->setSize(256,256);
	valframe->setPos(768,32);
	valframe->setParent(GUI);
	
	GUIvaluedisplay* valx = new GUIvaluedisplay;	GUIvaluedisplay* val2x = new GUIvaluedisplay;
	valx->setPos(0,32);								val2x->setPos(128,32);
	valx->val = (void*)(&CamAngle.w);				val2x->val = (void*)(&SomeVec2.x);
	valx->mode = 'f';								val2x->mode = 'f';
	valx->setParent(valframe);						val2x->setParent(valframe);
	
	GUIvaluedisplay* valy = new GUIvaluedisplay;	GUIvaluedisplay* val2y = new GUIvaluedisplay;
	valy->setPos(0,64);								val2y->setPos(128,64);
	valy->val = (void*)(&CamAngle.v.x);				val2y->val = (void*)(&SomeVec2.y);
	valy->mode = 'f';								val2y->mode = 'f';
	valy->setParent(valframe);						val2y->setParent(valframe);
	
	GUIvaluedisplay* valz = new GUIvaluedisplay;	GUIvaluedisplay* val2z = new GUIvaluedisplay;
	valz->setPos(0,96);								val2z->setPos(128,96);
	valz->val = (void*)(&CamAngle.v.y);				val2z->val = (void*)(&SomeVec2.z);
	valz->mode = 'f';								val2z->mode = 'f';
	valz->setParent(valframe);						val2z->setParent(valframe);
	
	GUIvaluedisplay* valmx = new GUIvaluedisplay;	GUIvaluedisplay* val2my = new GUIvaluedisplay;
	valmx->setPos(0,128);							val2my->setPos(128,128);
	valmx->val = (void*)(&CamAngle.v.z);				val2my->val = (void*)(&mousePos.y);
	valmx->mode = 'f';								val2my->mode = 'd';
	valmx->setParent(valframe);						val2my->setParent(valframe);
	
	GUIvaluedisplay* valdmx = new GUIvaluedisplay;	GUIvaluedisplay* val2dmy = new GUIvaluedisplay;
	valdmx->setPos(0,128+32);							val2dmy->setPos(128,128+32);
	valdmx->val = (void*)(&deltaMouse.x);				val2dmy->val = (void*)(&deltaMouse.y);
	valdmx->mode = 'd';								val2dmy->mode = 'd';
	valdmx->setParent(valframe);					val2dmy->setParent(valframe);
}

void camForward(void* arg){SomeVec1 = SomeVec1+(vector){0,.1,0};}
void camBack(void* arg){SomeVec1 = SomeVec1+(vector){0,-.1,0};}
void camLeft(void* arg){SomeVec1 = SomeVec1+(vector){-.1,0,0};}
void camRight(void* arg){SomeVec1 = SomeVec1+(vector){.1,0,0};}
void ToggleMouseCapture(void* arg){mouseCapture = !mouseCapture;}

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
	bindKey('T',&Test1,NULL,1);
	bindKey('W',&camForward,NULL,4);
	bindKey('A',&camRight,NULL,4);
	bindKey('S',&camBack,NULL,4);
	bindKey('D',&camLeft,NULL,4);
	bindKey(27,&ToggleMouseCapture,NULL,1);
	mouseCapture = false;
	CamAngle = {0,{0,0,1}};
	
	bground.r = 142;
	bground.g = 187;
	bground.b = 255;
	bground.a = 255;

	GUI = new GUIbase;
	GUI->setPos(0,0);
	GUI->setSize(1024,1024);
	GUI->recalculateClientRect();
	GUI->visible=false;
	
	//OpenMenu1();
	OpenMenuGen();
	OpenMenuConsole();
	OpenVals();
	designerMenu();
	//myFrame.parent
    //MessageBox(0, "FreeType: done generating textures","info", MB_OK);
}



void RenderGUI()
{
	mouseP = mousePos;
	static int twidth = 0;
	glColor3f(0.2f,0.7f,0.7f);
	paintRect(32,30,32+twidth,52);
	glColor3f(1.0f,1.0f,1.0f);
	string version("Version ");
	string vnumber = "53";
	twidth = printw(32,32,-1,-1,version+vnumber);
	
	vec2i pack[3]= {mousePos, (vec2i){0,0}, (vec2i){(int)width, (int)height}};
    vec4i windowrect = {0,0,(int)width,(int)height};
	
	GUIbase::propagateMouseOver(GUI,(void*)(pack), 0);
	GUIbase::propagateRender(GUI,(void*)(&windowrect),0);
	
	
	//printw(256,256,-1,-1,"crect.x = %d, y = %d, \nbtn.pos.x = %d, y = %d", myFrame->crect.x1,myFrame->crect.y1,myFrame->CloseButton->pos.x, myFrame->CloseButton->pos.y);
	
	//GUIM.render(NULL);
	//GUIM.checkMouseOver(NULL, mousePos.x, mousePos.y);
}

void ProcessMouseclick(int mb)
{
   GUIbase::propagateClick(GUI,(void*)(&mb),0);
   // GUIM.click(NULL, mb);
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
	if(mouseCapture)
	{
		//get delta from center and then re-center!
		deltaMouse = mousePos-(vec2i){(int)width/2,(int)height/2};
		SomeVec2 = SomeVec2 + (vector){(double)deltaMouse.x/width,(double)deltaMouse.y/height,0};
		CamAngle = CamAngle.addRotation(deltaMouse.x/width,{0,0,1}).addRotation(deltaMouse.y/height,{1,0,0});
		if(CamAngle.w>360){CamAngle.w=0;}
		if(CamAngle.w<0){CamAngle.w=360;}
		SetCursorPos(windowCenter.x,windowCenter.y);
	}
}

void ProcessKeyboard(int kb)
{
	int letter = 0;
	if(kb>0)
	{
		letter = MapVirtualKey((unsigned int)kb,2);
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
	bool b = GUIbase::propagateKeyboard(letter*kb/abs(kb));
	if(!b&&!ParseKey(kb))
	{
		//string mystring = "";
		char str[64];
		 //std::ostringstream stringStream;
		//stringStream << "Unbound key: " << kb << " = [" <<(char)kb <<"]/[" <<(char)letter << "]";
		
		sprintf(str,"Unbound key: %d ",kb);//= [%c]/[%c]",kb,(char)kb,(char)letter);
		printf("sending string to parse: \"%s\"\n",str);
		ConsoleParse((string)str);
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
void Render3D()
{
    glPushMatrix();
	glRotatef(-CamAngle.w, CamAngle.v.x, CamAngle.v.z, CamAngle.v.y);
	glTranslated(SomeVec1.x,SomeVec1.z,SomeVec1.y);
    //glRotatef(theta, 0.0f, 0.0f, 1.0f);
	glScalef(0.1f,0.1f,0.1f);
    glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
        glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

    glEnd();
	if(myModel!=NULL)
	{
		//apply orientation here
		glBegin(GL_TRIANGLES);
		for(int I=0;I<myModel->numtris;I++)
		{
			triangle T = myModel->mesh[I];
			glColor3f(1.0f, 0.0f, 0.0f);   glVertex3f(T.v[0].x,T.v[0].y,T.v[0].z);
			glColor3f(0.0f, 1.0f, 0.0f);   glVertex3f(T.v[1].x,T.v[1].y,T.v[1].z);
			glColor3f(0.0f, 0.0f, 1.0f);   glVertex3f(T.v[2].x,T.v[2].y,T.v[2].z);
		}
		glEnd();
	}

    glPopMatrix();
}
void Render_go2D()
{
    glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void Render_go3D()
{
    glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	glLoadIdentity();
	double top;
	double bottom;
	double left;
	double right;
	double fov = 90;
	double znear = 0.1;
	double zfar = 100;
	double aspect = 1;
	
	top = tan(fov*3.14159/360.0) * znear;
	bottom = -top;
	left = aspect * bottom;
	right = aspect * top;

    glFrustum(left,right,bottom,top,znear,zfar);
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


void ProgramTick(HWND hwnd, HDC hDC)
{
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
    RenderTick(hDC);
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
