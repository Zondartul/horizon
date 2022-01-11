#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <math.h>
#include <vector>
#include "globals.h"
#include "fonts.h"
#include "Gui.h"
//global vars go here

/*
CURRENT TASK:
GUI:
    attempted storing frames in vectors. Sucks because it breaks pointers all the time.
    TODO: remove vector code and replace by linked list to store every frame.
    NOTE: obj = new cl means obj will not get auto-destroyed when leaving the scope.
*/


//unsigned char image[640*480];






GUIManager GUIM;

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
    GUIobj* myFrame = new GUIobj;
    myFrame->pos.x = 350;
    myFrame->pos.y = 32;
    myFrame->size.x = 256;
    myFrame->size.y = 256;
	myFrame->color.r = 255;
	myFrame->color.g = 96;
	myFrame->color.b = 255;
    GUIM.activate(myFrame);
	
	GUIobj* Button = new GUIobj;
	Button->pos.x = 400;
	Button->pos.y = 64;
	Button->size.x = 64;
	Button->size.y = 64;
	Button->color = {0,64,255};
	Button->parent = myFrame;
	GUIM.activate(Button);
	
	GUIobj* Frame2 = new GUIframe;
	//Frame2->pos = {64,64};
	Frame2->size = {256,128};
	Frame2->color = {196,196,196};
	GUIM.activate(Frame2);
	
	GUIobj* Text = new GUItextEntry;
	Text->pos = {4,32};
	Text->size = {256-8,128-34};
	//((GUItextEntry*)Text)->text = "pintos and bays";
	Text->parent = Frame2;
	GUIM.activate(Text);
   //myFrame.parent
    //MessageBox(0, "FreeType: done generating textures","info", MB_OK);
}


void RenderGUI()
{
	static int twidth = 0;
	glColor3f(0.2f,0.7f,0.7f);
	paintRect(32,30,32+twidth,52);
	glColor3f(1.0f,1.0f,1.0f);
	twidth = printw(32,32,"Version: 52");
    GUIM.render(NULL);
	GUIM.checkMouseOver(NULL, mousePos.x, mousePos.y);
}

void ProcessMouseclick(int mb)
{
    GUIM.click(NULL, mb);
}

void ProcessKeyboard(int kb)
{
	GUIM.keyboard(kb);
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
