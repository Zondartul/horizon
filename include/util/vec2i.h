#ifndef VEC2I_GUARD
#define VEC2I_GUARD

struct vec2i{
    int x;
    int y;
};
vec2i clamp(vec2i A, vec2i B, vec2i C);
vec2i operator + (vec2i A, vec2i B);
vec2i operator - (vec2i A, vec2i B);
vec2i operator - (vec2i A);
bool operator == (vec2i A, vec2i B);
bool operator != (vec2i A, vec2i B);
vec2i operator / (vec2i A, int B);
vec2i operator * (vec2i A, int B);

#endif