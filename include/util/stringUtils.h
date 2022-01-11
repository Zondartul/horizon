#ifndef STRINGUTILS_GUARD
#define STRINGUTILS_GUARD

#include <string>
using std::string;
string itoa(int N);
string ftoa(double N);
string ftoa(double N, int prec);

string operator+(const string S, const int N);
string operator+(const string S, const double N);

#ifdef VEC_GUARD
string toString(vec3f);
string toString(vec3ub);
string toString(vec2i);
string toString(vec2f);
string toString(rect);
string toString(rectf);
string toString(vec2 V);
string toString(vec3 V);
string toString(mat4 M);
#endif

string toString(void *p);
string toString(bool B);
string toString(int I);
string toString(float f);
string toString(string S);

#endif