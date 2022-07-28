// dllmain.cpp : Defines the entry point for the DLL application.
//#include "pch.h"
#include "main/framework_resource.h"
#include "main/main_resource.h"
#include "main/main_program.h"
#include "loaders/fontLoader.h" //to initFreeType
#include "resource/resource.h"
//#include "util/global_vars_resource.h"

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

void initHorizonResource() {
    static bool initialized = false;
    if (initialized) { return; }
    initialized = true;

    initHorizonProgram();
    initFreeType();
    loadAssets();
}

