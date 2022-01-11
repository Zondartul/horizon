#ifndef GLOBALS_GUARD
#define GLOBALS_GUARD

#include <gl/gl.h>
#include <gl/glu.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <typeinfo>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
using namespace std;
#include "SDL2/SDL.h"
#include "templates/ptrSniper.h"
#include "templates/sgn.h"
#include "templates/toString.h"
#include "templates/bits.h"
#include "util/vec2i.h"
#include "util/vec3i.h"
#include "util/vec4i.h"
#include "util/color3i.h"
#include "util/color4i.h"
#include "util/rect.h"
#include "util/moreStrings.h"
//#define M_PI           3.14159265358979323846
#define listForwardI iterator I = subscribers.start(), E = subscribers.end(); I!=E;I++
#define BRDB 4
// border-border
#define BRDT 36
// border-title
typedef uint8_t byte;
//int clamp(int A, int B, int C);
double clamp(double A, double B, double C);

struct vec2f
{
    double x;
    double y;
};


struct listNode{
    listNode* next;
    void* thing;
};

extern int ms;
extern int freq;
extern float theta;
extern int counter;
extern color4i bground;
extern SDL_Window *window;
extern SDL_GLContext glcontext;
double d2r(double x);
double r2d(double x);
typedef void (*funcptr)(void *arg);

double random(double min, double max);
int randint(int min, int max);
class except:public exception{
	public:
	const char *mystr;
	except(const char *a, int b, const char *str);
	virtual const char* what() const throw();
};
						
rect getScreenRect();
#ifndef EXCEPT_SUPPRESS
#define except(a) except(__FILE__,__LINE__,a)
#endif

#endif
