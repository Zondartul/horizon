#pragma once

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#ifdef DLL_GUI
#define DLLAPI_GUI DLLEXPORT
#else
#define DLLAPI_GUI DLLIMPORT
#endif
