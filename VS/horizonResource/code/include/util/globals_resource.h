#pragma once

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#ifdef DLL_RESOURCE
	#define DLLAPI_RESOURCE DLLEXPORT
#else
	#define DLLAPI_RESOURCE DLLIMPORT
#endif
