#define EXCEPT_SUPPRESS //lol, have to suppress the macro in this particular
//translation unit, because that's where the REAL except is actually defined
#include "util/globals.h"
//#include "main/control.h"
#include "assert.h"
using namespace std;
/*
int clamp(int A, int B, int C)
{
	if(A<B)
	{
		return B;
	}
	else
	{
		if(A>C){return C;}
		else{return A;}
	}
}
*/
double clamp(double A, double B, double C){
	return A < B? B : A > C? C : A;
}
int ms = 0;
int freq = 66;
float theta;
int counter = 0;
//float width = 1024.0f; USE getScreenRect() INSTEAD
//float height = 640.0f;
//rect screen;
color4i bground;
//vec2i mousePos;		USE input.GetMousePos() INSTEAD
double d2r(double x){return x*M_PI/180.0;}
double r2d(double x){return x*180.0/M_PI;}
SDL_Window *window;
SDL_GLContext glcontext;

unsigned int explode(const string &txt, vector<string> &strs, char ch)
{
    unsigned int pos = txt.find( ch );
    unsigned int initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

string itoa(double I)
{
	stringstream str;
	str.str("");
	str<<I;
	return str.str();
}

//atoi already there



double random(double min, double max)
{
	return min+((double)rand()/RAND_MAX)*(max-min);
}

int randint(int min, int max)
{
	return (int)(min+((double)rand()/RAND_MAX)*(max-min));
}



except::except(const char *a, int b, const char *str){
	mystr = (string(str)+" ["+string(a)+", line "+b+"]").c_str();
}
const char* except::what() const throw()
{
	return mystr;
}

rect getScreenRect(){
	rect R;
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	R.setStart({0,0}).setSize({w,h});
	return R;
}
