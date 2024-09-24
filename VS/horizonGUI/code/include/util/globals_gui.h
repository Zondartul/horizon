#pragma once

#ifdef WIN32
    #define DLLEXPORT __declspec(dllexport)
    #define DLLIMPORT __declspec(dllimport)

    #ifdef DLL_GUI
    #define DLLAPI_GUI DLLEXPORT
    #else
    #define DLLAPI_GUI DLLIMPORT
    #endif
#endif
#ifdef LINUX
	#define DLLEXPORT
	#define DLLIMPORT
	#define DLLAPI_GUI
#endif
#ifndef WIN32
	#ifndef LINUX
		#error "Must define global switch WIN32 or LINUX"
	#endif
#endif
