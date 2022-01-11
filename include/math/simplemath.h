#ifndef SIMPLEMATH_GUARD
#define SIMPLEMATH_GUARD

#define d2r 0.01745329251f
//raises 2 to the power of name
int exp2(int n);
//returns closest power-of-two
int potceil(int n);
//returns log2 of a number, rounded up
int log2ceil(int n);

//returns B if A < B, C if A > C, or A otherwise.
float clamp(float A, float B, float C);
//returns the smaller of the two numbers.
float min(float A, float B);
//returns the greater of the two numbers.
float max(float A, float B);
//rounds to n decimal places.
float round2(float A, float n);
#define mix2(a,b,n) (a*(1.0f-n)+b*n)

float random(float A, float B);

#endif