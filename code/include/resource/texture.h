#ifndef TEXTURE_GUARD
#define TEXTURE_GUARD
#include <string>
using std::string;
#include "vec.h"
struct bitmap;
struct texture{
	string name;
	texture();
	int w();
	int h();
	vec2 size();
	rect UV;				
	rect getRect();
	bitmap *bmp;
};
#endif