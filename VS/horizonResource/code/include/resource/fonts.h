#ifndef FONTS_GUARD
#define FONTS_GUARD
#include "math/vec.h"
#include "util/globals_resource.h"
#include <map>
#include <string>
using std::map;
using std::string;

struct texture;

struct DLLAPI_RESOURCE glyph{
	texture *t;
	int bearingX;
	int bearingY;
	int advance;
};

struct DLLAPI_RESOURCE font{
	string name;
	rect maxrect;
	map<char,glyph> charmap;
};

#endif
