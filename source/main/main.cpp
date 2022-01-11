#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <cstdio>
//#include <ft2build.h>
//#include FT_FREETYPE_H

#include "main/control.h"
#include "gui/newconsole.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void ReSizeGLScene(GLsizei with, GLsizei height);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
//    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1024,
                          640,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    //SetWindowText(hwnd, "hi");
	::hwnd = hwnd; //save the window handle for the rest of the program
    OnProgramStart();
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            ProgramTick(hwnd, hDC);
        }
    }

    CallDestructor();
    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	keyThing(uMsg, wParam, lParam);
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;
		break;
		/*
		case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
			//ProcessKeyboard((int)wParam); all the buttons should go through the keything
        }
		break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			//ProcessKeyboard(-(int)wParam);
		}
        break;

        case WM_LBUTTONDOWN:
            //ProcessMouseclick(1);
        break;
		case WM_LBUTTONUP:
			//ProcessMouseclick(0);
		break;
		case WM_RBUTTONDOWN:
            //ProcessMouseclick(2);
        break;
		case WM_RBUTTONUP:
			//ProcessMouseclick(-1);
		break;
		*/
		case WM_SIZE:                           // Resize The OpenGL Window
		{
			
			RECT rect;
			GetClientRect(hwnd, &rect);
			
			ReSizeGLScene(rect.right,rect.bottom);//LOWORD(lParam),HIWORD(lParam));       // LoWord=Width, HiWord=Height
			return 0;                       // Jump Back
		}
		break;
		case WM_SETFOCUS:
			printf("Got the focus\n");
		break;
		case WM_KILLFOCUS:
			printf("Lost the focus\n");
			convars["camera_mouse_capture"] = 0;
		break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);
	
	
    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)             // Resize And Initialize The GL Window
{
    // if (height==0)                              // Prevent A Divide By Zero By
    // {
        // height=1;                           // Making Height Equal One
    // }
     glViewport(0, 0, width, height);                    // Reset The Current Viewport
	// glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    // glLoadIdentity();                           // Reset The Projection Matrix
 
    ////Calculate The Aspect Ratio Of The Window
    // gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
 
    // glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
    // glLoadIdentity();                           // Reset The Modelview Matrix
}
