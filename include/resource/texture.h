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
	vec2i size();
	rect rect();
	rectf UV;				//A texture need not be the entire image, but it can be some sub-image.
	bitmap *bmp;
};

#endif