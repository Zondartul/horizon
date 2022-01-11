#ifndef GLOBALS_GUARD
#define GLOBALS_GUARD

#include <windows.h>
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
#define M_PI           3.14159265358979323846
#define listForwardI iterator I = subscribers.start(), E = subscribers.end(); I!=E;I++
#define BRDB 4
// border-border
#define BRDT 36
// border-title
using namespace std;
int clamp(int A, int B, int C);

struct vec2i{
    int x;
    int y;
};
vec2i clamp(vec2i A, vec2i B, vec2i C);
struct vec2f
{
    double x;
    double y;
};
vec2i operator + (vec2i A, vec2i B);
vec2i operator - (vec2i A, vec2i B);
vec2i operator - (vec2i A);
bool operator == (vec2i A, vec2i B);
bool operator != (vec2i A, vec2i B);
vec2i operator / (vec2i A, int B);
vec2i operator * (vec2i A, int B);
struct color3i{
	int r;
	int g;
	int b;
};
color3i operator + (color3i A, color3i B);
color3i operator * (color3i A, int B);
color3i operator / (color3i A, int B);
color3i operator * (color3i A, double B);
color3i operator / (color3i A, double B);
struct color4i{
	int r;
	int g;
	int b;
	int a;
	void setColor(color3i newColor);
};
color4i operator + (color4i A, color4i B);
color4i operator * (color4i A, int B);
color4i operator / (color4i A, int B);
color4i operator * (color4i A, double B);
color4i operator / (color4i A, double B);


struct vec3i{
	operator color3i();
    int x;
    int y;
    int z;
};
vec3i operator + (vec3i A, vec3i B);
vec3i operator * (vec3i A, int B);
vec3i operator / (vec3i A, int B);
struct vec4i{
	int x1;
	int y1;
	int x2;
	int y2;
	bool contains(vec2i A);
};
bool operator == (vec4i A, vec4i B);

//a 2d rectangle with integer coordinates.
class rect{
	private:
	vec2i start;	//top-left vertex
	vec2i end;		//bottom-right vertex
	vec2i size;		//width and height.
	rect *parent;	//the rectangle that this rectangle's coordinates are relative to.
	public:
	rect();
	rect(vec2i start, vec2i end);
	rect &setStart(vec2i A);			//change dimensions (resize)
	rect &setEnd(vec2i A);
	rect &setSize(vec2i A);
	rect &setParent(rect *parent);
	rect &moveBy(vec2i A);				//move without resizing
	rect &moveStart(vec2i A);			
	rect &moveEnd(vec2i A);				
	rect &setx(int x);					//change dimensions (resize)
	rect &sety(int y);
	rect &setw(int w);
	rect &seth(int h);
	rect &setx2(int x2);
	rect &sety2(int y2);
	vec2i getStart();
	vec2i getEnd();
	vec2i getSize();
	vec2i getTopLeft();
	vec2i getTopRight();
	vec2i getBottomLeft();
	vec2i getBottomRight();
	int getx();
	int gety();
	int getw();
	int geth();
	int getx2();
	int gety2();
	rect *getParent();
	bool contains(vec2i A); // is A inside of this?
	bool contains(vec4i A);
	bool contains(rect A);
	vec2i clamp(vec2i A);	// constrain A to be inside of this
	rect clamp(rect A);
	rect insert(rect A);	// move A inside of this
	rect toParent(rect A);	// convert to diffirent coordinate system
	rect toParent();
	rect toWorld(rect A);
	rect toWorld();
	rect fromParent(rect A);
	rect fromParent();
	rect fromWorld(rect A);
	rect fromWorld();
	vec2i toParent(vec2i A);
	vec2i toWorld(vec2i A);
	vec2i fromParent(vec2i A);
	vec2i fromWorld(vec2i A);
	string toString();
};
bool operator == (rect A, rect B);

struct listNode{
    listNode* next;
    void* thing;
};

extern int ms;
extern int freq;
extern float theta;
extern int counter;
extern float width;
extern float height;
extern color4i bground;
extern vec2i mousePos;
extern rect screen;
double d2r(double x);
double r2d(double x);
typedef void (*funcptr)(void *arg);
unsigned int explode(const string &txt, vector<string> &strs, char ch);
string itoa(double I);
#include "templates/ptrSniper.h"
#include "templates/sgn.h"
double random(double min, double max);
int randint(int min, int max);
#include "templates/toString.h"
#include "templates/bits.h"
class except:public exception{
	public:
	const char *mystr;
	except(const char *a, int b, const char *str);
	virtual const char* what() const throw();
};
std::string operator+(std::string const &a, int b);
std::string operator+(std::string const &a, double b);
string tolower(string A);
string toupper(string A);
string fstring(const char* fmt, ...);
#ifndef EXCEPT_SUPPRESS
#define except(a) except(__FILE__,__LINE__,a)
#endif

#endif
