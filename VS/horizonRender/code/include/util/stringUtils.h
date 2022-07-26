#ifndef STRINGUTILS_GUARD
#define STRINGUTILS_GUARD
#include "util/globals_render.h"
#include <cstdarg>
#include <string>
#include <vector>
#include <cstdio>
using std::string;
using std::vector;

string DLLAPI_RENDER itoa(int N);
string DLLAPI_RENDER ftoa(double N);
string DLLAPI_RENDER ftoa(double N, int prec);

string DLLAPI_RENDER operator+(const string S, const int N);
string DLLAPI_RENDER operator+(const string S, const double N);

#define printfify(format,str) \
	va_list ap;\
	va_start(ap, format);\
	char cstr[256];\
	vsnprintf(cstr,256,format,ap);\
	str = string(cstr);

DLLAPI_RENDER string fstring(const char *format, ...);

void DLLAPI_RENDER stringUtils_error(string S);
template<typename T> T fromString(const string S){
	stringUtils_error("unimplemented variant of fromString<> called ");
	return T();
}
//#ifdef VEC_GUARD
//#endif

//camera.h:
//#ifdef CAMERA_GUARD
//#endif
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
void DLLAPI_RENDER argcargvtest(int argc, char **argv);

//returns argc from argv
int DLLAPI_RENDER countargs(char **argv);

//replaces special characters with escape sequences
string DLLAPI_RENDER escapeString(string S);

//replaces escape sequences with special characters
string DLLAPI_RENDER unescapeString(string S);

//copies a C-string to heap.
char DLLAPI_RENDER *stralloc(const char *str);

//prints a very long string S
void DLLAPI_RENDER printString(string S);

#endif