#ifndef FONTS_GUARD
#define FONTS_GUARD
#include "math/vec.h"
#include <map>
#include <string>
using std::map;
using std::string;

struct texture;
struct glyph{
	texture *t;
	int bearingX;
	int bearingY;
	int advance;
};

struct font{
	string name;
	rect maxrect;
	map<char,glyph> charmap;
};
#endif
