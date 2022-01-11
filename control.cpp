#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <math.h>
#include <ctype.h>
//#include <vector>

#include "globals.h"
#include "fonts.h"
//#include "Gui.h"
#include "Gui2.h"

//global vars go here

//GUIManager GUIM;
GUIbase* GUI;
/*
void myButton(void *holder)
{
	GUIM.axe(NULL);
}
*/
GUIframe* myFrame;
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
	
	bground.r = 142;
	bground.g = 187;
	bground.b = 255;
	bground.a = 255;
	
	GUI = new GUIbase;
	GUI->setPos(0,0);
	GUI->setSize(128,128);
	GUI->recalculateClientRect();
	GUI->visible=true;
	
	myFrame = new GUIframe;
	myFrame->setPos(0,0);
	myFrame->setSize(96,96);
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
	twidth = printw(32,32,version+vnumber);
	
	vec2i pack[3]= {mousePos, (vec2i){0,0}, (vec2i){(int)width, (int)height}};
    vec4i windowrect = {0,0,(int)width,(int)height};
	
	GUIbase::propagateMouseOver(GUI,(void*)(pack), 0);
	GUIbase::propagateRender(GUI,(void*)(&windowrect),0);
	
	
	printw(256,256,"crect.x = %d, y = %d, btn.pos.x = %d, y = %d", myFrame->crect.x1,myFrame->crect.y1,myFrame->CloseButton->pos.x, myFrame->CloseButton->pos.y);
	
	//GUIM.render(NULL);
	//GUIM.checkMouseOver(NULL, mousePos.x, mousePos.y);
}

void ProcessMouseclick(int mb)
{
   GUIbase::propagateClick(GUI,(void*)(&mb),0);
   // GUIM.click(NULL, mb);
}

void ProcessKeyboard(int kb)
{
	kb = MapVirtualKey((unsigned int)kb,2);
	if(!(GetKeyState(VK_SHIFT)&(0x8000))){kb = tolower(kb);}
	GUIbase::propagateKeyboard(kb);
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
    glRotatef(theta, 0.0f, 0.0f, 1.0f);
	glScalef(0.1f,0.1f,0.1f);
    glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
        glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

    glEnd();

    glPopMatrix();
}
void Render_go2D()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void Render_go3D()
{
    glMatrixMode(GL_PROJECTION);
	glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void RenderTick(HDC hDC)
{
    glClearColor(bground.r/255.0f,bground.g/255.0f,bground.b/255.0f,bground.a/255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
    mousePos.x = cursorPos.x-rect.left-7;
    mousePos.y = cursorPos.y-rect.top-29;

    GetClientRect(hwnd, &rect);
	width = rect.right;
	height = rect.bottom;
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
