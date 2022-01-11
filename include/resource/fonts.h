#ifndef FONTS_GUARD
#define FONTS_GUARD
#include "bitmap.h"
#include <map>
using std::map;
#include "textureLoader.h"
struct glyph{
	texture t;
	int bearingX;
	int bearingY;
	int advance;
};
struct font{
	map<char,glyph> charmap;
};

font getFont(const char *fontpath, int size);

void initFreeType();
void loadFont(const char *fontpath, int size);
bitmap generateGlyphBitmap(char C);
font generateFont(const char *fontpath, int size);

#endif
