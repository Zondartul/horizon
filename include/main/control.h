#ifndef CONTROL_GUARD
#define CONTROL_GUARD
#include <windows.h>
#include "util/globals.h"
void OnProgramStart();
void paintRect(int x1, int y1, int x2, int y2);
void Render2D();
void Render3D();
void Render_go2D();
void Render_go3D();
void RenderTick(HDC hDC);
void keyThing(UINT umsg, WPARAM wParam, LPARAM lParam);

void ProgramTick(HWND hwnd, HDC hDC);
void ProcessMouseclick(int mb);
void ProcessKeyboard(int kb);
void CallDestructor();
rect getScreenRect();
#include "../util/globals.h"
void bindKey(unsigned char key, funcptr onPress, funcptr onRelease, int mode);
#include "../gui/Gui2.h" //this should prob. go away.
#include "../input/input.h"
extern inputKind input;
#include "../math/quaternions.h"
extern GUI2base *GUI;
extern GUI2frame *myFrame;
//model *myModel;
extern vec SomeVec1; //cam pos.
extern vec SomeVec2;
extern vec2i deltaMouse;
extern vec2i windowCorner;
extern vec2i windowSize;
extern vec2i windowCenter;
extern quat CamAngle;
extern quat CamAngTest;
extern double Camnorm;
extern double camSpeed;
extern bool renderWireframe;
//int EntLookAt;
extern bool mouseCapture;
extern bool camRotOn;
#include "../display/renderable.h"
extern vector<renderable*> scene;
extern HWND hwnd;
#endif
