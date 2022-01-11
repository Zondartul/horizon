#ifndef VEC3I_GUARD
#define VEC3I_GUARD
#include "util/color3i.h"
struct vec3i{
	operator color3i();
    int x;
    int y;
    int z;
};
vec3i operator + (vec3i A, vec3i B);
vec3i operator * (vec3i A, int B);
vec3i operator / (vec3i A, int B);
#endif