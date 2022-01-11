#include "simplemath.h"
//#include "globals.h"

int exp2(int n){
	int r = 1;
	while(n--){r *= 2;}
	return r;
}
int potceil(int n){
	int r = 1;
	while(r < n){r *= 2;}
	return r;
}
int log2ceil(int n){
	int r = 1;
	int I = 0;
	while(r < n){r *= 2; I++;}
	return I;
}
float clamp(float A, float B, float C){
	if(A < B){return B;}
	if(A > C){return C;}
	return A;
}
float min(float A, float B){return A < B? A : B;}
float max(float A, float B){return A > B? A : B;}