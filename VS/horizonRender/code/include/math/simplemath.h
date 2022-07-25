#ifndef SIMPLEMATH_GUARD
#define SIMPLEMATH_GUARD
#include "util/globals_render.h"

#define d2r  0.01745329251f
#define r2d 57.29577951308f
//raises 2 to the power of name
DLLAPI_RENDER int exp2(int n);
//returns closest power-of-two
DLLAPI_RENDER int potceil(int n);
//returns log2 of a number, rounded up
DLLAPI_RENDER int log2ceil(int n);

//returns B if A < B, C if A > C, or A otherwise.
DLLAPI_RENDER float clamp(float A, float B, float C);
DLLAPI_RENDER int clampi(int A, int B, int C);
//returns the integer closest to the number (-0.5 to 0.5 -> 0, 0.6 -> 1)
DLLAPI_RENDER int roundi(float A);
//rounds down (-0.5 -> -1, 0.5 -> 0, 1.5 -> 1)
DLLAPI_RENDER int floori(float A);
//returns the smaller of the two numbers.
DLLAPI_RENDER float min(float A, float B);
//returns the greater of the two numbers.
DLLAPI_RENDER float max(float A, float B);
//returns A+(B-A)*R
DLLAPI_RENDER float mix(float A, float B, float R);
//rounds to n decimal places.
DLLAPI_RENDER float round2(float A, float n);
#define mix2(a,b,n) (a*(1.0f-n)+b*n)

DLLAPI_RENDER float random(float A, float B);

DLLAPI_RENDER int randint(int A, int B);

#endif
