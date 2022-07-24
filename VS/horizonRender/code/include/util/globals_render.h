#pragma once

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#ifdef DLL_RENDER
#define DLLAPI_RENDER DLLEXPORT
#else
#define DLLAPI_RENDER DLLIMPORT
#endif
