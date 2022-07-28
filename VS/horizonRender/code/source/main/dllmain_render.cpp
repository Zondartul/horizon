// dllmain.cpp : Defines the entry point for the DLL application.
//#include "pch.h"
#include "main/framework_render.h"
#include "main/main_render.h"
#include "main/main_resource.h"
#include "util/global_vars_render.h"
#include "render/camera.h"
#include "render/renderLayer.h"
//for renderInit
#include "util/global_vars_program.h"
#include "program/window.h"
#include "render/renderLow.h"
#include "render/paint.h"
#include <iostream>
using namespace std;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void initHorizonRender() {
    static bool initialized = false;
    if (initialized) { return; }
    initialized = true;

    initHorizonResource();

    Gr = new GlobalsRender();

    renderInit();
    setupLayers();

    auto& camera = Gr->gs_camera->g_camera;
    camera.setPos({ -0.5,0,0 });
}

void renderInit() {
    auto& width = Gp->gs_window->g_width;
    auto& height = Gp->gs_window->g_height;
    g_renderLow = new renderLowKind();
    g_renderLow->renderLowInit();
    g_renderLow->setViewportSize(width, height);
    initLayers();
    //printf("-------- render init done -----\n");
    cout << "-------- render init done -----" << endl;
}