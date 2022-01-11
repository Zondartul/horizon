#include "util/globals.h"

vec3i operator + (vec3i A, vec3i B)
{
	return {A.x+B.x, A.y+B.y, A.z+B.z};
}
vec3i operator * (vec3i A, int B)
{
	return {A.x*B,A.y*B,A.z*B};
}
vec3i operator / (vec3i A, int B)
{
	return {A.x/B,A.y/B,A.z/B};
}
vec3i::operator color3i()
{
	return {(uint8_t)clamp(x,0,255),(uint8_t)clamp(y,0,255),(uint8_t)clamp(z,0,255)};
}
