#pragma once

#ifdef WIN32
	#define DLLEXPORT __declspec(dllexport)
	#define DLLIMPORT __declspec(dllimport)

	#ifdef DLL_RESOURCE
		#define DLLAPI_RESOURCE DLLEXPORT
	#else
		#define DLLAPI_RESOURCE DLLIMPORT
	#endif
#endif
#ifdef LINUX
	#define DLLEXPORT
	#define DLLIMPORT
	#define DLLAPI_RESOURCE
#endif
#ifndef WIN32
	#ifndef LINUX
		#error "Must define global switch WIN32 or LINUX"
	#endif
#endif
