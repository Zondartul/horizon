#ifndef FONTS_GUARD
#define FONTS_GUARD
#include <map>
using std::map;
#include <string>
using std::string;
#include "vec.h"
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
