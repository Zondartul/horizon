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
#endif


#endif