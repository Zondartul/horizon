#ifndef GLOBALS_GUARD
#define GLOBALS_GUARD
//quick and dirty way to add something to all source files
//don't do drugs kids

//STL includes
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
//using std::function;

//ordinary global stuff
#define once(x) for(static bool once=true;once;once=false){x;}
#define onceon(x,y) for(static int onceon=0;onceon<=x;){if(onceon==x){y;} onceon++; break;}
#define firstn(x,y) for(static int firstn=0;firstn<=x;){{y;} firstn++; break;}
#define errorNotImplemented()	error((string("function ")+__func__+" not implemented\n").c_str())
#include "debug.h"

#define DLLAPI __declspec(dllexport)

#endif
