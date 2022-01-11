#include "stringutils.h"
#include "stdio.h"
#include "globals.h"

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
#include "vec.h"
string toString(vec3f V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f,%.3f)",V.x,V.y,V.z);
	return string(buff);
}

string toString(vec3ub V){
	char buff[80];
	snprintf(buff,79,"(%d,%.3d,%.3d)",V.r,V.g,V.b);
	return string(buff);
}

string toString(vec2i V){
	char buff[80];
	snprintf(buff,79,"(%d,%d)",V.x,V.y);
	return string(buff);
}

string toString(vec2f V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f)",V.x,V.y);
	return string(buff);
}

string toString(rect R){
	char buff[80];
	snprintf(buff,79,"(%d,%d + %d,%d)",R.start.x,R.start.y,R.size.x,R.size.y);
	return string(buff);
}

string toString(rectf R){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f + %.3f,%.3f)",R.start.x,R.start.y,R.size.x,R.size.y);
	return string(buff);
}





