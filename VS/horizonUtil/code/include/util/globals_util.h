#pragma once

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#ifdef DLL_UTIL
	#define DLLAPI_UTIL DLLEXPORT
#else
	#define DLLAPI_UTIL DLLIMPORT
#endif
