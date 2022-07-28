// dllmain.cpp : Defines the entry point for the DLL application.
#include "main/framework_program.h"
#include "main/main_program.h"
#include "main/main_util.h"
#include "util/global_vars_program.h"
#include "program/window.h"

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

void initHorizonProgram() {
    static bool initialized = false;
    if (initialized) { return; }
    initialized = true;

    initHorizonUtil();

    Gp = new GlobalsProgram();
    window_init(512, 512);
}

