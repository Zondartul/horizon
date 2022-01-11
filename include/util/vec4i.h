#ifndef VEC4I_GUARD
#define VEC4I_GUARD
struct vec4i{
	int x1;
	int y1;
	int x2;
	int y2;
	bool contains(vec2i A);
};
bool operator == (vec4i A, vec4i B);
#endif