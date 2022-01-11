#include "stringutils.h"
#include "stdio.h"
#include "globals.h"
#include "vec.h"

string itoa(int N){
	char buff[20];
	snprintf(buff,19,"%d",N);
	return string(buff);
}
string ftoa(double N){
	char buff[20];
	snprintf(buff,19,"%f",N);
	return string(buff);
}
string ftoa(double N, int prec){
	char format[20];
	snprintf(format,19,"%%.%df",prec);
	char buff[20];
	snprintf(buff,19,format,N);
	return string(buff);
}

#include "vec.h"
string toString(vec3 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f,%.3f)",V.x,V.y,V.z);
	return string(buff);
}

// string toString(vec3 V){
	// char buff[80];
	// snprintf(buff,79,"(%d,%.3d,%.3d)",V.r,V.g,V.b);
	// return string(buff);
// }

// string toString(vec2 V){
	// char buff[80];
	// snprintf(buff,79,"(%d,%d)",V.x,V.y);
	// return string(buff);
// }

string toString(vec2 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f)",V.x,V.y);
	return string(buff);
}

// string toString(rect R){
	// char buff[80];
	// snprintf(buff,79,"(%d,%d + %d,%d)",R.start.x,R.start.y,R.size.x,R.size.y);
	// return string(buff);
// }

string toString(rect R){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f + %.3f,%.3f)",R.start.x,R.start.y,R.size.x,R.size.y);
	return string(buff);
}

string toString(void *p){
	char buff[80];
	snprintf(buff,79,"0x%p",p);
	return string(buff);
}

string toString(AABB aabb){
	return fstring("(%.3f,%.3f,%.3f + %.3f,%.3f,%.3f)",aabb.start.x,aabb.start.y,aabb.start.y,aabb.size.x,aabb.size.y,aabb.size.z);
}
string toString(bool B){return itoa(B);}
string toString(string S){return S;}
string toString(int I){return itoa(I);}
string toString(float f){return ftoa(f);}

//string toString(vec2 V){return toString(tovec2(V));}
//string toString(vec3 V){return toString(tovec3(V));}
string toString(mat4 M){return "<mat4>";}
string operator+(const string S, const int N){return S+itoa(N);}
string operator+(const string S, const double N){return S+ftoa(N);}
	
string fstring(const char *format, ...){
	string str;
	printfify(format,str);
	return str;
}

#include "camera.h"
string toString(camprojection cpj){return toString(cpj.MVP)+":"+toString(cpj.pos);}