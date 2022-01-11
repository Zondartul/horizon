#ifndef STRINGUTILS_GUARD
#define STRINGUTILS_GUARD
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

string fstring(const char *format, ...);
void stringUtils_error(string S);

template<typename T> T fromString(const string S){
	stringUtils_error("unimplemented variant of fromString<> called ");
	return T();
}

#ifdef VEC_GUARD
string toString(vec3);
template<> vec3 fromString<vec3>(const string S);
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
template<> int fromString<int>(const string S);
string toString(float f);
string toString(string S);
template<> string fromString<string>(const string S);

#define toCString(x) toString(x).c_str()
#define fromCString(T,x) fromString<T>(string(x))

#endif

//returns argv as a vector<string>
vector<string> explode(string S, char del);

//returns argv as char**
char **explode(const char *str, char del);

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

