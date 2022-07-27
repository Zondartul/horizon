#include "math/simplemath.h"
#include "util/globals_util.h"
#include <cstdlib>

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
int clampi(int A, int B, int C){
	if(A < B){return B;}
	if(A > C){return C;}
	return A;
}
int roundi(float A){
	if(A > 0){return (int)(A+0.5f);}
	else{return (int)(A-0.5f);}
}
int floori(float A){
	if(A > 0){return (int)A;}
	else{return ((int)A - 1);}
}
float min(float A, float B){return A < B? A : B;}
float max(float A, float B){return A > B? A : B;}
float mix(float A, float B, float R){return A+(B-A)*R;}
float round2(float A, float n){if(n>0){return round2(10*A,n-1)/10;}else{return (int)A;}}


float random(float A, float B){
	return A+(B-A)*rand()/RAND_MAX;
}

int randint(int A, int B){
    return A+(rand()%(B-A+1));
}
