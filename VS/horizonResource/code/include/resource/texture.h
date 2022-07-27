#ifndef TEXTURE_GUARD
#define TEXTURE_GUARD
#include "math/vec.h"
#include "util/globals_resource.h"
#include <string>
using std::string;

struct bitmap;

struct DLLAPI_RESOURCE texture{
	string name;
	texture();
	int w();
	int h();
	vec2 size();
	rect UV;				//A texture need not be the entire image, but it can be some sub-image.
	rect getRect();
	bitmap *bmp;
};

#endif