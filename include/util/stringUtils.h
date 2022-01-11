#ifndef STRINGUTILS_GUARD
#define STRINGUTILS_GUARD
#include <cstdarg>
#include <string>
using std::string;
string itoa(int N);
string ftoa(double N);
string ftoa(double N, int prec);

string operator+(const string S, const int N);
string operator+(const string S, const double N);

#define printfify(format,str) \
	va_list ap;\
	va_start(ap, format);\
	char cstr[256];\
	vsnprintf(cstr,256,format,ap);\
	str = string(cstr);

string fstring(const char *format, ...);

#ifdef VEC_GUARD
string toString(vec3);
//string toString(vec3);
string toString(vec2);
//string toString(vec2);
string toString(rect);
string toString(AABB aabb);
//string toString(rectf);
//string toString(vec2 V);
//string toString(vec3 V);
string toString(mat4);
#endif
#ifdef CAMERA_GUARD
string toString(camprojection);
#endif
string toString(void *p);
string toString(bool B);
string toString(int I);
string toString(float f);
string toString(string S);
#define toCString(x) toString(x).c_str()
#endif