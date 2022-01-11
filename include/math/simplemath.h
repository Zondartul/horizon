#ifndef SIMPLEMATH_GUARD
#define SIMPLEMATH_GUARD

#define d2r  0.01745329251f
#define r2d 57.29577951308f
//raises 2 to the power of name
int exp2(int n);
//returns closest power-of-two
int potceil(int n);
//returns log2 of a number, rounded up
int log2ceil(int n);

//returns B if A < B, C if A > C, or A otherwise.
float clamp(float A, float B, float C);
int clampi(int A, int B, int C);
//returns the integer closest to the number (-0.5 to 0.5 -> 0, 0.6 -> 1)
int roundi(float A);
//rounds down (-0.5 -> -1, 0.5 -> 0, 1.5 -> 1)
int floori(float A);
//returns the smaller of the two numbers.
float min(float A, float B);
//returns the greater of the two numbers.
float max(float A, float B);
//returns A+(B-A)*R
float mix(float A, float B, float R);
//rounds to n decimal places.
float round2(float A, float n);
#define mix2(a,b,n) (a*(1.0f-n)+b*n)

float random(float A, float B);

int randint(int A, int B);

#endif
