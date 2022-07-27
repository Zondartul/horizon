#pragma once

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#ifdef DLL_PROGRAM
	#define DLLAPI_PROGRAM DLLEXPORT
#else
	#define DLLAPI_PROGRAM DLLIMPORT
#endif

