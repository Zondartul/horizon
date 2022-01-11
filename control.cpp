#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <math.h>
#include <vector>
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

struct color4i
{
	int r;
	int g;
	int b;
	int a;
};
struct color3f
{
	float r;
	float g;
	float b;
};
struct color4f
{
	float r;
	float g;
	float b;
	float a;
};


int ms = 0;
int freq = 66;
float theta;
int counter = 0;
float width = 640.0f;
float height = 640.0f;
color4f bground;
glyphkind *Tahoma16;
glyphkind *Calibri20;

    int mouseX;
    int mouseY;




GUIManager GUIM;

void OnProgramStart()
{
    theta = 1.0f;
    //if(MessageBox(0,"V: 44. Continue?", "Version", MB_OKCANCEL|MB_ICONQUESTION )==2){PostQuitMessage(0); return;}
    Tahoma16 = GenerateFont("C:/Stride/tahoma.ttf", 16);
    Calibri20 = GenerateFont("C:/Stride/calibri.ttf", 20);
	setFont(Calibri20);
	
	bground.r = 142/255.0f;
	bground.g = 187/255.0f;
	bground.b = 255/255.0f;
	bground.a = 1.0f;
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
	Frame2->pos = {64,64};
	Frame2->size = {128,196};
	Frame2->color = {196,196,196};
	GUIM.activate(Frame2);
   //myFrame.parent
    //MessageBox(0, "FreeType: done generating textures","info", MB_OK);
}


void RenderGUI()
{
	static int twidth = 0;
	glColor3f(0.2f,0.7f,0.7f);
	paintRect(32,30,32+twidth,52);
	glColor3f(1.0f,1.0f,1.0f);
	twidth = printw(32,32,"Version: 46");
    GUIM.render(NULL);
	GUIM.checkMouseOver(NULL, mouseX, mouseY);
}

void ProcessMouseclick(int mb)
{
    GUIM.click(NULL, mb);
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
    glOrtho(0, width-16, height-32, 0, -1, 1);
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
    glClearColor(bground.r,bground.g,bground.b,bground.a);
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
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    mouseX = cursorPos.x-7;
    mouseY = cursorPos.y-29;

    RECT rect;
    GetWindowRect(hwnd, &rect);
    mouseX -= rect.left;
    mouseY -= rect.top;
    RenderTick(hDC);
}

void CallDestructor()
{
    //GUI vectors call destructors in their elements
    fontFree(Tahoma16);
    fontFree(Calibri20);
}
