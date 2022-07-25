#ifndef GLOBALS_GUARD
#define GLOBALS_GUARD
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <functional>
#include <memory>
#include <typeinfo>
#include <algorithm>
using std::vector;
using std::string;
using std::map;
using std::set;
using std::list;
#define once(x) for(static bool once=true;once;once=false){x;}
#define onceon(x,y) for(static int onceon=0;onceon<=x;){if(onceon==x){y;} onceon++; break;}
#define firstn(x,y) for(static int firstn=0;firstn<=x;){{y;} firstn++; break;}
#define errorNotImplemented()	error((string("function ")+__func__+" not implemented\n").c_str())
//#include "debug.h"
//#define DLLAPI __declspec(dllimport)
//#define DLLAPI __declspec(dllexport)

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#ifdef DLL_GUI
#define DLLAPI_GUI DLLEXPORT
#else
#define DLLAPI_GUI DLLIMPORT
#endif

#ifdef DLL_RENDER
#define DLLAPI_RENDER DLLEXPORT
#else
#define DLLAPI_RENDER DLLIMPORT
#endif


//we are going fuckdangle this file too
#endif
