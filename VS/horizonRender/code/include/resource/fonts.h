#ifndef FONTS_GUARD
#define FONTS_GUARD
#include "math/vec.h"
#include "util/globals_render.h"
#include <map>
#include <string>
using std::map;
using std::string;

struct texture;

struct DLLAPI_RENDER glyph{
	texture *t;
	int bearingX;
	int bearingY;
	int advance;
};

struct DLLAPI_RENDER font{
	string name;
	rect maxrect;
	map<char,glyph> charmap;
};

#endif
