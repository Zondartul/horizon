#ifndef GLOBALS_GUARD
#define GLOBALS_GUARD
//quick and dirty way to add something to all source files
//don't do drugs kids

#define once(x) for(static bool once=true;once;once=false){x;}
#define onceon(x,y) for(static int onceon=0;onceon<=x;){if(onceon==x){y;} onceon++; break;}
#define firstn(x,y) for(static int firstn=0;firstn<=x;){{y;} firstn++; break;}
#include "debug.h"

#endif