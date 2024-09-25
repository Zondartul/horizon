#ifndef STRINGUTILS_GUARD
#define STRINGUTILS_GUARD
#include "util/globals_util.h"
#include <cstdarg>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
using std::string;
using std::vector;

/// stringstream formatter for strings
#define SS(x) [&]()->std::string{std::stringstream ss; ss << x; return ss.str();}()

string DLLAPI_UTIL itoa(int N);
string DLLAPI_UTIL ftoa(double N);
string DLLAPI_UTIL ftoa(double N, int prec);

string DLLAPI_UTIL operator+(const string S, const int N);
string DLLAPI_UTIL operator+(const string S, const double N);

#define printfify(format,str) \
	va_list ap;\
	va_start(ap, format);\
	char cstr[256];\
	vsnprintf(cstr,256,format,ap);\
	str = string(cstr);

DLLAPI_UTIL string fstring(const char *format, ...);

void DLLAPI_UTIL stringUtils_error(string S);
template<typename T> T fromString(const string S){
	stringUtils_error("unimplemented variant of fromString<> called ");
	return T();
}
//#ifdef VEC_GUARD
//#endif

//camera.h:
//#ifdef CAMERA_GUARD
//#endif
DLLAPI_UTIL string toString(void *p);		template<> DLLAPI_UTIL void*  fromString<void*>(const string S);
DLLAPI_UTIL string toString(bool B);		template<> DLLAPI_UTIL bool   fromString<bool>(const string S);
DLLAPI_UTIL string toString(int I);			template<> DLLAPI_UTIL int    fromString<int>(const string S);
DLLAPI_UTIL string toString(float f);		template<> DLLAPI_UTIL float  fromString<float>(const string S);
DLLAPI_UTIL string toString(string S);		template<> DLLAPI_UTIL string fromString<string>(const string S);
struct texture;
DLLAPI_UTIL string toString(texture *t);	template<> DLLAPI_UTIL texture* fromString<texture*>(const string S);
struct font;
DLLAPI_UTIL string toString(font *f);		template<> DLLAPI_UTIL font*  fromString<font*>(const string S);
//#ifdef GUI_INTERNAL_GUARD
//string toString(GUIe_alignment al); template<> GUIe_alignment fromString<GUIe_alignment>(const string S);
//#endif

#define toCString(x) toString(x).c_str()
#define fromCString(T,x) fromString<T>(string(x))
DLLAPI_UTIL vector<string> explode(string S, char del);

//returns argv as char**
DLLAPI_UTIL char **explode(const char *str, char del);

//prints contents of argc/argv
void DLLAPI_UTIL argcargvtest(int argc, char **argv);

//returns argc from argv
int DLLAPI_UTIL countargs(char **argv);

//replaces special characters with escape sequences
string DLLAPI_UTIL escapeString(string S);

//replaces escape sequences with special characters
string DLLAPI_UTIL unescapeString(string S);

//copies a C-string to heap.
char DLLAPI_UTIL *stralloc(const char *str);

//prints a very long string S
void DLLAPI_UTIL printString(string S);

#endif