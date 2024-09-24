#pragma once

#ifdef WIN32
	#define DLLEXPORT __declspec(dllexport)
	#define DLLIMPORT __declspec(dllimport)

	#ifdef DLL_UTIL
		#define DLLAPI_UTIL DLLEXPORT
	#else
		#define DLLAPI_UTIL DLLIMPORT
	#endif
#endif
#ifdef LINUX
	#define DLLEXPORT
	#define DLLIMPORT
	#define DLLAPI_UTIL
#endif
#ifndef WIN32
	#ifndef LINUX
		#error "Must specify global switch WIN32 or LINUX" 
	#endif
#endif
