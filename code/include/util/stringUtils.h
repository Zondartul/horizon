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
string toString(vec3);	template<> vec3 	fromString<vec3>(const string S);
string toString(vec2);	template<> vec2		fromString<vec2>(const string S);
string toString(rect);	template<> rect		fromString<rect>(const string S);
string toString(AABB);	template<> AABB		fromString<AABB>(const string S);
string toString(mat4);	template<> mat4		fromString<mat4>(const string S);
#endif


#ifdef CAMERA_GUARD
string toString(camprojection);	template<> camprojection fromString<camprojection>(const string S);
#endif


string toString(void *p);		template<> void*	fromString<void*>(const string S);
string toString(bool B);		template<> bool		fromString<bool>(const string S);
string toString(int I);			template<> int		fromString<int>(const string S);
string toString(float f);		template<> float	fromString<float>(const string S);
string toString(string S);		template<> string	fromString<string>(const string S);



struct texture;
string toString(texture *t);	template<> texture*	fromString<texture*>(const string S);




struct font;
string toString(font *f);		template<> font*	fromString<font*>(const string S);



#ifdef GUI_INTERNAL_GUARD
string toString(GUIe_alignment al); template<> GUIe_alignment fromString<GUIe_alignment>(const string S);
#endif

#define toCString(x) toString(x).c_str()
#define fromCString(T,x) fromString<T>(string(x))



vector<string> explode(string S, char del);


char **explode(const char *str, char del);


void argcargvtest(int argc, char **argv);


int countargs(char **argv);


string escapeString(string S);


string unescapeString(string S);


char *stralloc(const char *str);


void printString(string S);

#endif