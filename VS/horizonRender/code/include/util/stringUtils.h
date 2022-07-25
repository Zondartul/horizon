#ifndef STRINGUTILS_GUARD
#define STRINGUTILS_GUARD
#include "util/globals_render.h"
#include <cstdarg>
#include <string>
#include <vector>
#include <cstdio>
using std::string;
using std::vector;
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

DLLAPI_RENDER string fstring(const char *format, ...);

void stringUtils_error(string S);
template<typename T> T fromString(const string S){
	stringUtils_error("unimplemented variant of fromString<> called ");
	return T();
}
#ifdef VEC_GUARD
DLLAPI_RENDER string toString(vec3);	template<> DLLAPI_RENDER vec3	fromString<vec3>(const string S);
DLLAPI_RENDER string toString(vec2);	template<> DLLAPI_RENDER vec2	fromString<vec2>(const string S);
DLLAPI_RENDER string toString(rect);	template<> DLLAPI_RENDER rect	fromString<rect>(const string S);
DLLAPI_RENDER string toString(AABB);	template<> DLLAPI_RENDER AABB	fromString<AABB>(const string S);
DLLAPI_RENDER string toString(mat4);	template<> DLLAPI_RENDER mat4	fromString<mat4>(const string S);
#endif

//camera.h:
#ifdef CAMERA_GUARD
DLLAPI_RENDER string toString(camprojection);	template<> camprojection fromString<camprojection>(const string S);
#endif
DLLAPI_RENDER string toString(void *p);		template<> DLLAPI_RENDER void*  fromString<void*>(const string S);
DLLAPI_RENDER string toString(bool B);		template<> DLLAPI_RENDER bool   fromString<bool>(const string S);
DLLAPI_RENDER string toString(int I);			template<> DLLAPI_RENDER int    fromString<int>(const string S);
DLLAPI_RENDER string toString(float f);		template<> DLLAPI_RENDER float  fromString<float>(const string S);
DLLAPI_RENDER string toString(string S);		template<> DLLAPI_RENDER string fromString<string>(const string S);
struct texture;
DLLAPI_RENDER string toString(texture *t);	template<> DLLAPI_RENDER texture* fromString<texture*>(const string S);
struct font;
DLLAPI_RENDER string toString(font *f);		template<> DLLAPI_RENDER font*  fromString<font*>(const string S);
//#ifdef GUI_INTERNAL_GUARD
//string toString(GUIe_alignment al); template<> GUIe_alignment fromString<GUIe_alignment>(const string S);
//#endif

#define toCString(x) toString(x).c_str()
#define fromCString(T,x) fromString<T>(string(x))
DLLAPI_RENDER vector<string> explode(string S, char del);

//returns argv as char**
DLLAPI_RENDER char **explode(const char *str, char del);

//prints contents of argc/argv
void argcargvtest(int argc, char **argv);

//returns argc from argv
int countargs(char **argv);

//replaces special characters with escape sequences
string escapeString(string S);

//replaces escape sequences with special characters
string unescapeString(string S);

//copies a C-string to heap.
char *stralloc(const char *str);

//prints a very long string S
void printString(string S);

#endif